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


def length = 1000;

plot SellTriggers = (
  fold i = 0 to length
  with aboveSell = 0
    while Speed[i] > SellLine
      do (
        aboveSell + (
            if (Speed[i] > GetValue(Speed, i + 1, 0)) then (Speed[i] - GetValue(Speed, i + 1, 0))
            else (-1 * (GetValue(Speed, i + 1, 0) - Speed[i]))
        )
    )
);
plot BuyTriggers = (
  fold j = 0 to length
  with aboveBuy = 0
    while Speed[j] < BuyLine
      do (
        aboveBuy + (
            if (Speed[j] > GetValue(Speed, j + 1, 0)) then (Speed[j] - GetValue(Speed, j + 1, 0))
            else (-1 * (GetValue(Speed, j + 1, 0) - Speed[j]))
        )
    )
);

SellTriggers.AssignValueColor(
    if SellTriggers > 0 then Color.MAGENTA
    else Color.Dark_Gray
);
BuyTriggers.AssignValueColor(
    if BuyTriggers < 0 then Color.Green
    else Color.Dark_Gray
);
