declare lower;

input BuyLine = -20;
input SellLine = 20;
def BuyZone = BuyLine;
def SellZone = SellLine;

#----RSI Math
def NetChgAvg = MovingAverage(AverageType.EXPONENTIAL, close - close[1], 14);
def TotChgAvg = MovingAverage(AverageType.EXPONENTIAL, AbsValue(close - close[1]), 14);
def ChgRatio =
    if TotChgAvg != 0
    then NetChgAvg / TotChgAvg
    else 0;

def RSI = 50 * (ChgRatio + 1) ;
def RSIValue = ((ExpAverage(RSI, 8) * 1.7) - (ExpAverage( RSI(), 34) * 1.7)) ;

def OB = 30;
def OS = -30;
def Zero = 0;
def Speed = ExpAverage(RSIValue - RSIValue[1], 10) * 10;
def SpeedEma = ExpAverage(Speed, 8);
def SpeedEma2 = ExpAverage(Speed, 55) * 3;

def sellDiff = _rsiYellow2 < _rsiYellow;
def length = 1000;

plot BuyTriggers = (
  fold i = 0 to length
  with aboveBuy = 0
    while trigger[i] > BuyLine
      do (aboveBuy + 1)
);
