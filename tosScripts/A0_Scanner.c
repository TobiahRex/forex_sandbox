declare lower;
#==================================================================
#==========================Moving Averages=========================
#==================================================================
input fast = 8;
input medium = 21;
input slow = 55;
input slowest = 89;
input BuyZoneEdge = 2;
input SellZoneEdge = -2;

def emaFast = ExpAverage(close, fast);
def emaSlow = ExpAverage(close, medium);
def smaFast = SimpleMovingAvg(close, slow);
def smaSlow = SimpleMovingAVg(close, slowest);

def buyAvgs = emaFast > smaFast;
def sellAvgs = emaFast < smaFast;

#==================================================================
#========================== Zone Calc =============================
#==================================================================
input timeFrame = 10;

def zoneRsiYellow = ExpAverage(RSI(), timeFrame * 8) - 50;
def zoneRSIDiff = (zoneRsiYellow - zoneRsiYellow[1]);
def zoneSpeed = ExpAverage(zoneRSIDiff, 8) * 25;
def zoneSpeed2 = ExpAverage(zoneRSIDiff, 21) * 25;
def zoneSpeedDiff = zoneSpeed - 0;

#==================================================================
#========================== Triggers Calc =========================
#==================================================================
input trig_SellLine = -19;
input trig_BuyLine = 19;
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

#==================================================================
#====================== Buy & Sell Detection ======================
#==================================================================
def trig_strongOS = trig_OS;
def trig_medOB = trig_SellLine;
def trig_strongOB = trig_OB;
def trig_medOS = trig_BuyLine;

def BuyZone = zoneRsiYellow > BuyZoneEdge;
def SellZone = zoneRsiYellow < SellZoneEdge;

def buyTrig1 = trig_Speed crosses below trig_SellLine;
def buyTrig2 = trig_Speed crosses above trig_SellLine;
def buyTrig3 = trig_Speed <= trig_OS;

def BuyTrigger_1 = (
    (zoneSpeed <= zoneSpeed2)
    && (buyTrig1)
);

def BuyTrigger_2 = (
    (zoneSpeed <= zoneSpeed2)
    && (buyTrig2)
);

def sellTrig1 = trig_Speed crosses above trig_BuyLine;
def sellTrig2 = trig_Speed crosses below trig_BuyLine;
def sellTrig3 = trig_Speed >= trig_OB;
def SellTrigger_1 = (
    (zoneSpeed >= zoneSpeed2)
    && (sellTrig1)
);
def SellTrigger_2 = (
    (zoneSpeed >= zoneSpeed2)
    && (sellTrig2)
);


#==================================================================
#========================== Colors ================================
#==================================================================
input d_rsiOB = 8;
input d_rsiOS = -8;
def zoneRSI = trigRSI - 50;

def StrongBUY_dn = (BuyZone && BuyTrigger_1);
def StrongBUY_up = (BuyZone && BuyTrigger_2);
def StrongSELL_up = (SellZone && SellTrigger_1);
def StrongSELL_dn = (SellZone && SellTrigger_2);

plot Signal = (
  StrongBUY_dn or
  StrongBUY_up or
  StrongSELL_up or
  StrongSELL_dn
);
