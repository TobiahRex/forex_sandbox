declare upper;
#==================================================================
#==========================Moving Averages=========================
#==================================================================
input fast = 8;
input medium = 21;
input slow = 55;
input slowest = 89;

input prey = { default BUY, SELL, BUY_EXIT, SELL_EXIT };

def target;
switch (prey) {
case SELL:
    target = -1;
case BUY:
    target = 1;
case BUY_EXIT:
    target = 2;
case SELL_EXIT:
    target = -2;
}

def emaFast = ExpAverage(close, fast);
def emaSlow = ExpAverage(close, medium);
def smaFast = SimpleMovingAvg(close, slow);
def smaSlow = SimpleMovingAVg(close, slowest);

def buyAvgs = emaFast > smaFast;
def sellAvgs = emaFast < smaFast;

#==================================================================
#===================== Direction Calc =============================
#==================================================================

input direction_Time = 10;
input direction_rsiEma = 13;
def timeFrame = direction_Time;
def rsiEma = 13;
def timeFrameLength = timeFrame * 13;
def averageClose = (close + close[timeFrame]) / 2;
def avgNetChange = MovingAverage(AverageType.Exponential, averageClose - averageClose[1], timeFrameLength);
def avgTotalChange = MovingAverage(AverageType.Exponential, AbsValue(averageClose - averageClose[1]), timeFrameLength);
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
def zzSave = if !IsNaN(triggerZZ) then Bullet else getValue(zzSave, 1);
def chgTrigger = Bullet - getValue(zzSave, 1);
def isUpTrigger = chgTrigger >= 0;
def isConfTrigger = AbsValue(chgTrigger) >= t_ReversalAmount or (IsNaN(getValue(triggerZZ, 1)) and getValue(isConfTrigger, 1));
#==================================================================
#====================== Buy & Sell Detection ======================
#==================================================================

#--------------------------- Consolidation ---------------------------
def bearishConsolidation = _rsiDirection > _rsiEmaDirection && (
  _rsiDirection < Zero
  #&& _rsiEmaDirection < Zero
);

def bullishConsolidation = _rsiDirection < _rsiEmaDirection && (
  _rsiDirection > Zero
  #&& _rsiEmaDirection > Zero
);
def doNothing = bearishConsolidation or bullishConsolidation;


#--------------------------- ZONES ---------------------------

def BuyZone = (
  target == 1
  && doNothing == 0
  && _rsiDirection > _rsiEmaDirection
  #&& Bullet > zero
);
def SellZone = (
  target == -1
  && doNothing == 0
  && _rsiDirection < _rsiEmaDirection
  #&& Bullet < zero
);

#--------------------------- BUY ---------------------------

def buyTrig1 = Bullet crosses below triggerBuyLine;
def buyTrig2 = (Bullet < triggerBuyLine and Bullet > Bullet[1]) or Bullet crosses above triggerBuyLine;
def buyTrig3 = Bullet crosses above triggerOS;
def buyTrig4 = Bullet crosses above zero;


def Buy1 = (
  BuyZone
  && buyTrig1
);

def Buy2 = (
  BuyZone
  && buyTrig2
);

def Buy3 = (
  #BuyZone &&
  buyTrig3
);

def Buy4 = (
  BuyZone &&
  buyTrig4
);

#--------------------------- BUY ---------------------------

def sellTrig1 = Bullet crosses above triggerSellLine;
def sellTrig2 = (Bullet > triggerSellLine and Bullet < Bullet[1]) or Bullet crosses below triggerSellLine;
def sellTrig3 = Bullet crosses below triggerOB;
def sellTrig4 = Bullet crosses below zero;


def Sell1 = (
  SellZone
  && sellTrig1
);

def Sell2 = (
  SellZone
  && sellTrig2
);

def Sell3 = (
  #SellZone &&
  sellTrig3
);


def Sell4 = (
  SellZone &&
  sellTrig4
);

#---------------------------------------------------------------
#----- Candle Conditions: Candle closes across multiple EMAs
#---------------------------------------------------------------
plot c_sell = (
    (open > emaFast && open > emaSlow)
    && (close < emaSlow && close < emaFast)
);
c_sell.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
c_sell.SetLineWeight(1);
c_sell.SetDefaultColor(Color.Orange);

plot c_buy = (
    (open < emaFast && open < emaSlow)
    && (close > emaSlow && close > emaFast)
);
c_buy.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
c_buy.SetLineWeight(1);
c_buy.SetDefaultColor(Color.Yellow);


#==================================================================
#========================== Colors ================================
#==================================================================

plot StrongBUY_aim = Buy1;
plot StrongBUY_fire = Buy2 or Buy4;
plot StrongSELL_aim = Sell1;
plot StrongSELL_fire = Sell2 or Sell4;
plot StrongestSELL = Sell3;
plot StrongestBUY = Buy3;

StrongBUY_fire.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
StrongBUY_aim.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
StrongSELL_aim.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
StrongSELL_fire.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
StrongBUY_fire.SetDefaultColor(color.Cyan);
StrongBUY_aim.SetDefaultColor(color.Cyan);
StrongBUY_fire.SetLineWeight(3);
StrongSELL_fire.SetDefaultColor(Color.Magenta);
StrongSELL_aim.SetDefaultColor(Color.magenta);
StrongSELL_fire.SetLineWeight(3);

StrongestSELL.SetLineWeight(5);
StrongestSELL.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
StrongestSELL.SetDefaultColor(Color.White);

StrongestBUY.SetLineWeight(5);
StrongestBUY.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
StrongestBUY.SetDefaultColor(Color.White);


AssignPriceColor(
    if doNothing then Color.Dark_Gray
    else if (
      SellZone && _rsiDirection > _rsiDirection[1] && (target == -1)
    ) then Color.Dark_Red
    else if (
      BuyZone && _rsiDirection < _rsiDirection[1] && (target == 1)
    ) then Color.Dark_Green
    else if c_buy then Color.Yellow
    else if c_sell then Color.Yellow
    else if (StrongBUY_aim && (target == 1)) then Color.Cyan
    else if (StrongBUY_fire && (target == 1)) then Color.Cyan
    else if (StrongSELL_aim && (target == -1)) then Color.Magenta
    else if (StrongSELL_fire && (target == -1)) then Color.Magenta
    else if BuyZone && (target == 1) then Color.green
    else if SellZone && (target == -1) then Color.red
    else Color.Gray
  );
