
#==================================================================
#==========================Moving Averages=========================
#==================================================================
input fast = 8;
input medium = 21;
input slow = 55;
input slowest = 89;
input BuyZoneEdge = 1;
input SellZoneEdge = -1;

def emaFast = ExpAverage(close, fast);
def emaSlow = ExpAverage(close, medium);
def smaFast = SimpleMovingAvg(close, slow);
def smaSlow = SimpleMovingAVg(close, slowest);

def buyAvgs = emaFast > smaFast;
def sellAvgs = emaFast < smaFast;

#==================================================================
#===================== Direction Calc =============================
#==================================================================
input timeFrame = 10;

def zoneRsiYellow = ExpAverage(RSI(), timeFrame * 8) - 50;
def zoneRsiYellow2 = (ExpAverage(RSI(), (TimeFrame * .5) * 8) - 50);
def zoneRSIDiff = (zoneRsiYellow - zoneRsiYellow[1]);
def zoneSpeed = ExpAverage(zoneRSIDiff, 8) * 25;
def zoneSpeed2 = ExpAverage(zoneRSIDiff, 21) * 25;
def zoneSpeedDiff = zoneSpeed - 0;

#==================================================================
#========================== Triggers Calc =========================
#==================================================================
input trig_BuyLine = -30;
input trig_SellLine = 30;
def trig_OS = -30;
def trig_OB = 30;
def trig_NetChgAvg = MovingAverage(AverageType.EXPONENTIAL, close - close[1], 14);
def trig_TotChgAvg = MovingAverage(AverageType.EXPONENTIAL, AbsValue(close - close[1]), 14);
def trig_ChgRatio =
    if (trig_TotChgAvg != 0) then trig_NetChgAvg / trig_TotChgAvg
    else 0;
def trigRSI = 50 * (trig_ChgRatio + 1) ;

#---------------------------------------------------------------
#----- trig_Speed: The change in RSI Value relative to its last value
#----- trig_SpeedEma: The average of Diff2 over 8 periods.
#---------------------------------------------------------------

def trig_rsiDiffExp = (ExpAverage(trigRSI, 8) * 1.7 - ExpAverage(RSI(), 34) * 1.7);
def trig_Speed = ExpAverage((trig_rsiDiffExp - trig_rsiDiffExp[1]), 10) * 10;
def trig_SpeedEma = ExpAverage(trig_Speed, 8);
def trig_Accel = ExpAverage(trig_Speed - trig_Speed[1], 10) * 5;

#==================================================================
#====================== Buy & Sell Detection ======================
#==================================================================
def trig_strongOS = trig_OS;
def trig_medOB = trig_BuyLine;
def trig_strongOB = trig_OB;
def trig_medOS = trig_SellLine;

def BuyZone = zoneRsiYellow2 > zoneRsiYellow;
def SellZone = zoneRsiYellow2 < zoneRsiYellow;

def buyTrig1 = trig_Speed crosses below trig_BuyLine;
def buyTrig2 = trig_Speed < trig_BuyLine and (trig_Speed > trig_Speed[1] or trig_Speed crosses above trig_BuyLine);
def buyTrig3 = trig_Speed <= trig_OS;

def BuyTrigger_1 = (
    (zoneSpeed <= zoneSpeed2)
    && (buyTrig1)
);

def BuyTrigger_2 = (
    (zoneSpeed <= zoneSpeed2)
    && (buyTrig2)
);

def sellTrig1 = trig_Speed crosses above trig_SellLine;
def sellTrig2 = trig_Speed > trig_SellLine && (trig_Speed < trig_Speed[1] or trig_Speed crosses below trig_SellLine);
#def sellTrig3 = trig_Speed >= trig_OB;
def SellTrigger_1 = (
    (zoneSpeed >= zoneSpeed2)
    && (sellTrig1)
);
def SellTrigger_2 = (
    (zoneSpeed >= zoneSpeed2)
    && (sellTrig2)
);

plot buyExit = BuyZone && (trig_Accel crosses below 0);
plot sellExit = SellZone && (trig_Accel crosses above 0);
buyExit.SetPaintingStrategy(PaintingStrategy.BOOLEAN_WEDGE_DOWN);
sellExit.SetPaintingStrategy(PaintingStrategy.BOOLEAN_WEDGE_UP);
buyExit.SetDefaultColor(Color.Red);
sellExit.SetDefaultColor(Color.Green);
buyExit.SetLineWeight(5);
sellExit.SetLineWeight(5);

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
input d_rsiOB = 8;
input d_rsiOS = -8;
def zoneRSI = trigRSI - 50;
plot Overbought = zoneRSI > d_rsiOB;
plot Oversold = zoneRSI < d_rsiOS;

Overbought.SetPaintingStrategy(PaintingStrategy.BOOLEAN_POINTS);
Oversold.SetPaintingStrategy(PaintingStrategy.BOOLEAN_POINTS);
Overbought.SetDefaultColor(Color.Magenta);
Oversold.SetDefaultColor(Color.Cyan);

plot StrongBUY_dn = (BuyZone && BuyTrigger_1);
plot StrongBUY_up = (BuyZone && BuyTrigger_2);
plot StrongSELL_up = (SellZone && SellTrigger_1);
plot StrongSELL_dn = (SellZone && SellTrigger_2);

StrongBUY_up.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
StrongBUY_dn.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
StrongSELL_up.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
StrongSELL_dn.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
StrongBUY_up.SetDefaultColor(color.Cyan);
StrongBUY_dn.SetDefaultColor(color.Cyan);
StrongBUY_up.SetLineWeight(5);
StrongSELL_dn.SetDefaultColor(Color.Magenta);
StrongSELL_up.SetDefaultColor(Color.magenta);
StrongSELL_dn.SetLineWeight(5);


plot Exit = (
  (SellZone && BuyTrigger_1) && (SellZone && BuyTrigger_2)
) or (
  (BuyZone && SellTrigger_1) && (BuyZone && SellTrigger_2)
);
Exit.SetPaintingStrategy(PaintingStrategy.BOOLEAN_POINTS);
Exit.SetDefaultColor(Color.Yellow);
Exit.SetLineWeight(5);

AssignPriceColor(
    if c_buy then Color.Yellow
    else if c_sell then Color.Red
    else if StrongBUY_dn then Color.green
    else if StrongBUY_up then Color.cyan
    else if StrongSELL_up then Color.red
    else if StrongSELL_dn then Color.magenta
    else if BuyZone then Color.green
    else if SellZone then Color.red
    else Color.Gray
  );
