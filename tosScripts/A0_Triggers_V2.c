declare lower;

input BuyLine = -20;
input SellLine = 20;
plot BuyZone = BuyLine;
plot SellZone = SellLine;

#----RSI Math
def NetChgAvg = MovingAverage(AverageType.EXPONENTIAL, close - close[1], 14);
def TotChgAvg = MovingAverage(AverageType.EXPONENTIAL, AbsValue(close - close[1]), 14);
def ChgRatio =
    if TotChgAvg != 0
    then NetChgAvg / TotChgAvg
    else 0;

def RSI = 50 * (ChgRatio + 1) ;
def RSIValue = ((ExpAverage(RSI, 8) * 1.7) - (ExpAverage( RSI(), 34) * 1.7)) ;

plot OB = 30;
plot OS = -30;
plot Zero = 0;
plot Speed = ExpAverage(RSIValue - RSIValue[1], 10) * 10;
plot SpeedEma = ExpAverage(Speed, 8);
plot SpeedEma2 = ExpAverage(Speed, 55) * 3;
SpeedEma2.SetLineweight(3);
SpeedEma2.SetDefaultColor(Color.Magenta);


OB.SetDefaultColor(Color.Magenta);
OB.SetStyle(Curve.MEDIUM_DASH);
OB.SetLineWeight(2);
SellZone.SetDefaultColor(Color.Red);
SellZone.SetStyle(Curve.Short_DASH);
OS.SetDefaultColor(Color.Light_Green);
OS.SetStyle(Curve.MEDIUM_DASH);
BuyZone.SetDefaultColor(Color.Green);
BuyZone.SetStyle(Curve.Short_DASH);
Zero.SetDefaultColor(Color.LIGHT_GRAY);
Speed.SetDefaultColor(Color.CYAN);
Speed.SetPaintingStrategy(PaintingStrategy.LINE_VS_POINTS);
SpeedEma.SetDefaultColor(Color.Yellow);
SpeedEma.SetLineWeight(2);

AddCloud(0, BuyZone, Color.Green);
AddCloud(SellZone, 0, Color.Red);
