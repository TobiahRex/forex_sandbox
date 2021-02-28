declare upper;
#==================================================================
#==========================Moving Averages=========================
#==================================================================
input fast = 8;
input medium = 21;
input slow = 55;
input slowest = 89;
input rsiHistory_threshold = 9;
input rsiHistory_length = 10;

def emaBullet = ExpAverage(close, fast);
def emaFast = ExpAverage(emaBullet);
def emaSlow = ExpAverage(close, medium);
def smaFast = SimpleMovingAvg(close, slow);
def smaSlow = SimpleMovingAvg(close, slowest);

def buyAvgs = emaFast > smaFast;
def sellAvgs = emaFast < smaFast;

#==================================================================
#========================= Raw RSI ================================
#==================================================================
def length = 14;
def price = close;
def averageType = AverageType.WILDERS;
def showBreakoutSignals = no;

def NetChgAvg = MovingAverage(averageType, price - price[1], length);
def TotChgAvg = MovingAverage(averageType, AbsValue(price - price[1]), length);
def ChgRatio = if TotChgAvg != 0 then NetChgAvg / TotChgAvg else 0;

def wilderRsi = 50 * (ChgRatio + 1);
def OverSold = 30;
def OverBought = 70;
def wilderRsi_fifty = 50;

def rsiGreenHistory = (fold i = 0 to rsiHistory_length with greenRsi = 0
  while 1 do (
        if wilderRsi[i] > wilderRsi_fifty
            then greenRsi + 1
        else greenRsi
    )
);
def rsiBuyHistory = if rsiGreenHistory >= rsiHistory_threshold then 1 else 0;
def rsiRedHistory = (fold j = 0 to rsiHistory_length with redRsi = 0
  while 1 do (
        if wilderRsi[j] < wilderRsi_fifty
            then redRsi + 1
        else redRsi
    )
);
def rsiSellHistory = if rsiRedHistory >= rsiHistory_threshold then 1 else 0;

#==================================================================
#========================= Stochastic ZZ ==========================
#==================================================================
def stoch_min_low = lowest(low, percentKLength);
def stoch_max_high = highest(high, percentKLength);
def stoch_rel_diff = close - (max_high + min_low)/2;
def stoch_stoch_diff = max_high - min_low;
def stoch_avgrel = expaverage(expaverage(rel_diff, percentDLength), percentDLength);
def stoch_avgdiff = expaverage(expaverage(stoch_diff, percentDLength), percentDLength);
def stoch_SlowK = reference StochasticFull(80,20,10,10,high,low,close,3,AverageType.EXPONENTIAL).FullK;

def stoch_D = reference StochasticFull(80,20,10,10,high,low,close,3,AverageType.EXPONENTIAL).FullD;
def stoch_zz = reference ZigZagHighLow(SlowK, SlowK, 0, reversalAmount, 1, 0);

def zz_stoch = if !IsNaN(stoch_zz) then SlowK else GetValue(zz_stoch, 1);
def stoch_chg = price - GetValue(zz_stoch, 1);
def stoch_isUp = stoch_chg >= 0;
def stoch_isConf = AbsValue(stoch_chg) >= reversalAmount or (IsNaN(GetValue(stoch_zz, 1)) and GetValue(stoch_isConf, 1));

plot stoch_eighty = 80;
plot stoch_twenty = 20;
plot stoch_oversold = 35;
plot stoch_overbought = 65;
plot stoch_fifty = 50;

#==================================================================
#========================= MACD ===================================
#==================================================================
input macd_fast = 12;
input macd_slow = 26;
input macd_length = 9;
input macd_avgtype = AverageType.EXPONENTIAL;
plot macd_val = MovingAverage(macd_avgtype, close, macd_fast) - MovingAverage(macd_avgtype, close, macd_slow);
plot macd_avg = MovingAverage(macd_avgtype, macd_val, macd_length);
plot macd_diff = macd_val - macd_avg;
plot ZeroLine = 0;
plot UpSignal = if macd_diff crosses above ZeroLine then ZeroLine else Double.NaN;
plot DownSignal = if macd_diff crosses below ZeroLine then ZeroLine else Double.NaN;
#==================================================================
#===================== Direction Calc =============================
#==================================================================

input direction_Time = 10;
input direction_rsiEma = 13;
def timeFrame = direction_Time;
def rsiEma = 13;
def timeFrameLength = timeFrame * 13;
def averageClose = (close + close[timeFrame]) / 2;
def avgNetChange = MovingAverage(AverageType.EXPONENTIAL, averageClose - averageClose[1], timeFrameLength);
def avgTotalChange = MovingAverage(AverageType.EXPONENTIAL, AbsValue(averageClose - averageClose[1]), timeFrameLength);
def dirChangeRatio = if avgTotalChange != 0 then (avgNetChange / avgTotalChange) else 0;
def _rsiDirection = 50 * (1 + dirChangeRatio) - 50;
def _rsiEmaDirection = ExpAverage(_rsiDirection, rsiEma);
def rsiValueEma = (
  MovingAverage(AverageType.EXPONENTIAL, _rsiDirection, 5 * timeFrame)
  - MovingAverage(AverageType.EXPONENTIAL, _rsiDirection, 55 * timeFrame)
);
def rsiValueEmaFast = MovingAverage(AverageType.EXPONENTIAL, rsiValueEma, 13 * timeFrame);
def rsiValueEmaSlow = MovingAverage(AverageType.EXPONENTIAL, rsiValueEma, 55 * timeFrame);
def rsiBear = ExpAverage(_rsiDirection, timeFrame * 5);
def zero = 0;
def upperBound = -20;
def lowerBound = 20;
def upperBound2 = -38;
def lowerBound2 = 38;
input d_ReversalAmount = 1.0;
def directionZZ = reference ZigZagHighLow(_rsiDirection, _rsiDirection, 0, d_ReversalAmount, 1, 0);
def directionZZSave = if !IsNaN(directionZZ) then _rsiDirection else GetValue(directionZZSave, 1);
def chg = _rsiDirection - GetValue(directionZZSave, 1);
def isUpDirection = chg >= 0;
def isConfDirection = AbsValue(chg) >= d_ReversalAmount or (IsNaN(GetValue(directionZZ, 1)) and GetValue(isConfDirection, 1));


#==================================================================
#===================== Triggers Calc ==============================
#==================================================================
input triggerOS = -50;
input triggerOB = 50;
input triggerBuyLine = -30;
input triggerSellLine = 30;
input Sensitivity = 5;
def netChange = MovingAverage(AverageType.EXPONENTIAL, close - close[1], 14);
def totalChange = MovingAverage(AverageType.EXPONENTIAL, AbsValue(close - close[1]), 14);
def changeRatio =
    if totalChange != 0
    then netChange / totalChange
    else 0;
def _rsiTrigger = 50 * (changeRatio + 1) ;
def _rsiEmaTrigger = ((ExpAverage(_rsiTrigger, 8) * 1.7) - (ExpAverage(RSI(), 34) * 1.7)) ;
def OB = triggerOB;
def OS = triggerOS;
def Bullet = ExpAverage(_rsiEmaTrigger - _rsiEmaTrigger[1], Sensitivity) * 10;
def SpeedEma = ExpAverage(Bullet, 8);
def SpeedEma2 = ExpAverage(Bullet, 55) * 3;

input t_ReversalAmount = 1.0;
def triggerZZ = reference ZigZagHighLow(Bullet, Bullet, 0, t_ReversalAmount, 1, 0);
def zzSave = if !IsNaN(triggerZZ) then Bullet else GetValue(zzSave, 1);
def chgTrigger = Bullet - GetValue(zzSave, 1);
def isUpTrigger = chgTrigger >= 0;
def isConfTrigger = AbsValue(chgTrigger) >= t_ReversalAmount
    or (IsNaN(GetValue(triggerZZ, 1)) and GetValue(isConfTrigger, 1));

# Debugging Tools for Trigger
#
#plot trigZZ = triggerZZ;
#trigZZ.EnableApproximation();
#def x = trigZZ;
#AddLabel(yes, AsText(x, NumberFormat.TWO_DECIMAL_PLACES), Color.WHITE);

AddLabel(
    1,
    if _rsiEmaDirection >= 0                                   # Direction is Trending Up
        then if directionZZSave >= _rsiEmaDirection              # Direction ZZ is BUY (B1-4)
            then
                if zzSave >= GetValue(zzSave, 1)                  # Trigger is Rising
                    then if zzSave >= 0 then "  B1  " else "  B4  "         # Trigger above 0 (B1) else below 0 (B4)
                else                                              # Trigger is Decreasing
                    if zzSave >= 0 then "  B2  " else "  B3  "              # Trigger above 0 (B2) else below 0 (B3)
        else                                                    # Direction ZZ is B-Ranging (B5-8)
            if zzSave >= GetValue(zzSave, 1)                      # Trigger is Rising
                    then if zzSave >= 0 then "  B5  " else "  B8  "         # Trigger above 0 (B5) else below 0 (B8)
                else                                              # Trigger is Decreasing
                    if zzSave >= 0 then "  B6  " else "  B7  "              # Trigger above 0 (B6) else below 0 (B7)
    else                                                      # Direction is Trending Down
        if directionZZSave < _rsiEmaDirection                   # Direction ZZ is SELL (S1-4)
               then
                if zzSave >= GetValue(zzSave, 1)                  # Trigger is Rising
                    then if zzSave >= 0 then "  S3  " else "  S2  "         # Trigger above 0 (S3) else below 0 (S2)
                else                                              # Trigger is Decreasing
                    if zzSave >= 0 then "  S4  " else "  S1  "              # Trigger above 0 (S4) else below 0 (S1)
        else                                                    # Direction ZZ is S-Ranging (S5-8)
            if zzSave >= GetValue(zzSave, 1)                      # Trigger is Rising
                    then if zzSave >= 0 then "  S7  " else "  S6  "         # Trigger above 0 (S7) else below 0 (S6)
                else                                              # Trigger is Decreasing
                    if zzSave >= 0 then "  S8  " else "  S5  "              # Trigger above 0 (S8) else below 0 (S5)
    ,
      if _rsiEmaDirection >= 0                            # Direction is Trending Up
      then if directionZZSave >= _rsiEmaDirection                            # Direction ZZ is BUY (B1-4)
          then
              if zzSave >= GetValue(zzSave, 1)
                  then if zzSave >= 0 then Color.UPTICK else CreateColor(252, 136, 3)
              else
                  if zzSave >= 0 then CreateColor(3, 152, 252) else Color.YELLOW        # Light Blue & Yellow
      else
          if zzSave >= GetValue(zzSave, 1)
                  then if zzSave >= 0 then Color.DARK_GREEN else Color.GRAY
              else
                  if zzSave >= 0 then Color.GRAY else CreateColor(140, 3, 252)          # Gray & Purple
  else
      if directionZZSave < _rsiEmaDirection
              then
              if zzSave >= GetValue(zzSave, 1)
                  then if zzSave >= 0 then Color.YELLOW else Color.RED   # Yellow
              else
                  if zzSave >= 0 then CreateColor(252, 136, 3) else CreateColor(248, 3, 252)      # Orange & Hot Pink
      else
          if zzSave >= GetValue(zzSave, 1)
                  then if zzSave >= 0 then CreateColor(140, 3, 252) else Color.DARK_GRAY
              else
                  if zzSave >= 0 then Color.GRAY else Color.GRAY          # Gray & Purple
);
def TT = (
 if _rsiEmaDirection >= 0                            # Direction is Trending Up
        then if directionZZSave >= _rsiEmaDirection                            # Direction ZZ is BUY (B1-4)
            then
                if zzSave >= GetValue(zzSave, 1)                  # Trigger is Rising
                    then if zzSave >= 0 then 10 else 4         # Trigger above 0 (B1) else below 0 (B4)
                else                                              # Trigger is Decreasing
                    if zzSave >= 0 then 2 else 3              # Trigger above 0 (B2) else below 0 (B3)
        else                                                    # Direction ZZ is SELL (S5-8)
            if zzSave >= GetValue(zzSave, 1)                      # Trigger is Rising
                    then if zzSave >= 0 then 5 else 8         # Trigger above 0 (B5) else below 0 (B8)
                else                                              # Trigger is Decreasing
                    if zzSave >= 0 then 6 else 7              # Trigger above 0 (B6) else below 0 (B7)
    else                                                      # Direction is Trending Down
        if  directionZZSave < _rsiEmaDirection                                 # Direction ZZ is SELL (S1-4)
               then
                if zzSave >= GetValue(zzSave, 1)                  # Trigger is Rising
                    then if zzSave >= 0 then -3 else -2         # Trigger above 0 (S3) else below 0 (S2)
                else                                              # Trigger is Decreasing
                    if zzSave >= 0 then -4 else -1              # Trigger above 0 (S4) else below 0 (S1)
        else                                                    # Direction ZZ is BUY (B5-8)
            if zzSave >= GetValue(zzSave, 1)                      # Trigger is Rising
                    then if zzSave >= 0 then -7 else -6         # Trigger above 0 (S7) else below 0 (S6)
                else                                              # Trigger is Decreasing
                    if zzSave >= 0 then -8 else -5              # Trigger above 0 (S8) else below 0 (S5)
);
def marketStrength = (
    if 0 < TT && TT <= 4 then 1
    else if 4 < TT && TT < 10 then -1
    else if 0 > TT && TT >= -4 then 1
    else if TT equals 10 then 1
    else -1
);


Addlabel(
    1,
    if 0 <= directionZZ and directionZZ < 3 then "  0  |  " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else if 3 <= directionZZ and directionZZ < 5 then "  3  |  " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else if 5 <= directionZZ and directionZZ < 8 then "  5  |  " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else if 8 <= directionZZ and directionZZ < 12 then "  8  | + " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else if 12 <= directionZZ then "  12  | ++ " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else if 0 > directionZZ and directionZZ > -3 then "  0  |  " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else if -3 >= directionZZ and directionZZ > -5 then "  -3  |  " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else if -5 >= directionZZ and directionZZ > -8 then "  -5  |  " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else if -8 >= directionZZ and directionZZ > -12 then "  -8  |  + " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    else "  -12  |  ++ " + if marketStrength == 1 then " STRONG  " else " WEAK  "
    ,
    if 0 <= directionZZ and directionZZ < 3 then CreateColor(140, 3, 252)  #Purple
    else if 3 <= directionZZ and directionZZ < 5 then CreateColor(252, 136, 3)  #Orange
    else if 5 <= directionZZ and directionZZ < 8 then Color.Dark_Green
    else if 8 <= directionZZ and directionZZ < 12 then Color.UPTICK
    else if 12 <= directionZZ then Color.Cyan
    else if 0 > directionZZ and directionZZ > -3 then CreateColor(140, 3, 252)  #Purple
    else if -3 >= directionZZ and directionZZ > -5 then CreateColor(252, 136, 3)  #Orange
    else if -5 >= directionZZ and directionZZ > -8 then Color.DARK_RED
    else if -8 >= directionZZ and directionZZ > -12 then Color.Magenta
    else CreateColor(248, 3, 252) # Hot Pink
);


#==================================================================
#====================== Buy & Sell Detection ======================
#==================================================================
def prey = (
    if emaFast > smaSlow then 1
    else if emaFast < smaSlow then -1
    else 0
);
input mood = { default Aggressive, Conservative };
def target = prey;
def outlook;
switch (mood) {
case Aggressive:
    outlook = 1;
case Conservative:
    outlook = -1;
}
#--------------------------- Consolidation ---------------------------
def bearishConsolidation = if outlook == -1 then
    (_rsiDirection < _rsiEmaDirection) && (_rsiEmaDirection > lowerBound)
    else 0;

def bullishConsolidation = if outlook == -1 then
    (_rsiDirection > _rsiEmaDirection) && (_rsiEmaDirection < upperBound)
    else 0;

def doNothing = bearishConsolidation or bullishConsolidation;
#--------------------------- ZONES ---------------------------
def BuyZone = (
  target == 1
  && doNothing == 0
  && _rsiDirection > _rsiEmaDirection
);
def SellZone = (
  target == -1
  && doNothing == 0
  && _rsiDirection < _rsiEmaDirection
);
#--------------------------- BUY ---------------------------
def buyTrig1 = Bullet crosses below triggerBuyLine;
def buyTrig3 = Bullet crosses above triggerOS;
def buyTrig4 = Bullet crosses above upperBound;
def buyTrig5 = Bullet crosses above upperBound2;
def buyTrig6 = (
    wilderRsi crosses below wilderRsi_fifty
    or wilderRsi[1] crosses above wilderRsi_fifty
) && rsiBuyHistory;


def Buy1 = (
  BuyZone
  && buyTrig1
  && close > emaFast
  && _rsiEmaDirection > rsiBear
);

def Buy3 = (
  #BuyZone &&
  buyTrig3
  && close > emaFast
  && _rsiEmaDirection > rsiBear
);

def Buy4 = (
  BuyZone
  && (buyTrig4 or buyTrig5)
  && close > emaFast
  && _rsiEmaDirection >= rsiBear
  && directionZZSave >= 0
);

def Buy5 = (
  buyTrig6
  && _rsiEmaDirection > rsiBear
  && directionZZSave >= 0
  && (Bullet <= upperBound or GetValue(Bullet, 1) <= upperBound)
);


Alert(
    Buy4 or Buy5,
    "Buy!",
    Alert.BAR,
    Sound.Chimes
);
#--------------------------- SELL ---------------------------

def sellTrig1 = Bullet crosses above triggerSellLine;
def sellTrig3 = Bullet crosses below triggerOB;
def sellTrig4 = Bullet crosses below lowerBound;
def sellTrig5 = Bullet crosses below lowerBound2;
def sellTrig6 = (
    wilderRsi crosses above wilderRsi_fifty
    or wilderRsi[1] crosses above wilderRsi_fifty
    ) && rsiSellHistory;

def Sell1 = (
  SellZone
  && sellTrig1
  && close < emaFast
  && _rsiEmaDirection < rsiBear
);

def Sell3 = (
  #SellZone &&
  sellTrig3
  && close < emaFast
  && _rsiEmaDirection < rsiBear
);


def Sell4 = (
  SellZone
  && (sellTrig4 or sellTrig5)
  && close < emaFast
  && _rsiEmaDirection <= rsiBear
  && directionZZSave <= 0
);

def Sell5 = (
  sellTrig6
  && _rsiEmaDirection < rsiBear
  && directionZZSave <= 0
  && (Bullet >= lowerBound or GetValue(Bullet, 1) >= lowerBound)
);


Alert(
    Sell4 or Sell5,
    "Sell!",
    Alert.BAR,
    Sound.Chimes
);

#---------------------------------------------------------------
#----- Candle Conditions: Candle closes across multiple EMAs
#---------------------------------------------------------------
plot candle_sell = (
    (open > emaFast && open > emaSlow)
    && (close < emaSlow && close < emaFast)
);
candle_sell.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
candle_sell.SetLineWeight(1);
candle_sell.SetDefaultColor(Color.ORANGE);

plot candle_buy = (
   (open < emaFast && open < emaSlow)
    && (close > emaSlow && close > emaFast)
);
candle_buy.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
candle_buy.SetLineWeight(1);
candle_buy.SetDefaultColor(Color.YELLOW);

Alert(
    if candle_buy
        then if TT is equal to (1 or 2 or 3 or 4) then yes else no
    else if candle_sell
        then if TT is equal to (-1 or -2 or -3 or -4) then yes else no
    else no,
    "Entry",
    Alert.TICK,
    Sound.Chimes
);

#==================================================================
#========================== Colors ================================
#==================================================================

plot StrongBUY_aim = Buy1;
plot StrongBUY_fire = Buy4 or Buy5;
plot StrongSELL_aim = Sell1;
plot StrongSELL_fire = Sell4 or Sell5;
plot StrongestSELL = Sell3;
plot StrongestBUY = Buy3;
plot newHigh =  close > emaFast and high > high[1];
plot newLow = close < emaFast and low < low[1];

newHigh.SetPaintingStrategy(PaintingStrategy.BOOLEAN_POINTS);
newHigh.SetDefaultColor(Color.YELLOW);

newLow.SetPaintingStrategy(PaintingStrategy.BOOLEAN_POINTS);
newLow.SetDefaultColor(Color.YELLOW);

StrongBUY_aim.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
StrongBUY_fire.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);

StrongSELL_aim.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
StrongSELL_fire.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);

StrongBUY_fire.SetDefaultColor(Color.CYAN);
StrongBUY_aim.SetDefaultColor(Color.CYAN);
StrongBUY_fire.SetLineWeight(3);

StrongSELL_fire.SetDefaultColor(Color.MAGENTA);
StrongSELL_aim.SetDefaultColor(Color.MAGENTA);
StrongSELL_fire.SetLineWeight(3);

StrongestSELL.SetLineWeight(5);
StrongestSELL.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
StrongestSELL.SetDefaultColor(Color.WHITE);

StrongestBUY.SetLineWeight(5);
StrongestBUY.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
StrongestBUY.SetDefaultColor(Color.WHITE);


AssignPriceColor(
    if doNothing then Color.DARK_GRAY
    else if (
      SellZone && _rsiDirection > _rsiDirection[1] && (target == -1) && open < emaFast
    ) then Color.DARK_RED
    else if (
      BuyZone && _rsiDirection < _rsiDirection[1] && (target == 1)
    ) then Color.DARK_GREEN
    else if candle_buy then Color.YELLOW
    else if candle_sell then Color.YELLOW
    else if (StrongBUY_aim && (target == 1)) && open > emaFast then Color.GREEN
    else if (StrongBUY_fire && (target == 1)) then Color.CYAN
    else if (StrongSELL_aim && (target == -1) && low < emaFast) then Color.RED
    else if (StrongSELL_fire && (target == -1)) then Color.MAGENTA
    else if BuyZone && (target == 1) && open > emaFast then Color.GREEN
    else if SellZone && (target == -1) && low < emaFast then Color.RED
    else Color.GRAY
  );