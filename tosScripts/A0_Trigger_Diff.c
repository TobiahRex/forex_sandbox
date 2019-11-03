declare lower;

input BuyLineOS = -50;
input SellLineOB = 50;
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

plot OB = SellLineOB;
plot OS = BuyLineOS;
plot Zero = 0;
plot Speed = ExpAverage(RSIValue - RSIValue[1], 10) * 10;
plot Bullet = ExpAverage(RSIValue - RSIValue[1], 3) * 10;
plot SpeedEma = ExpAverage(Speed, 8);
plot SpeedEma2 = ExpAverage(Speed, 55) * 3;


Bullet.SetDefaultColor(Color.White);
Bullet.SetPaintingStrategy(PaintingStrategy.LINE_VS_SQUARES);

SpeedEma2.SetLineweight(3);
SpeedEma2.SetDefaultColor(Color.Dark_Orange);

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

Bullet.AssignValueColor(
    if (Bullet > SpeedEma2 && Bullet > Bullet[1]) then Color.Green
    else if (Bullet > SpeedEma2 && Bullet < Bullet[1]) then Color.Dark_Green
    else if (Bullet < SpeedEma2 && Bullet > Bullet[1]) then Color.Dark_Red
    else Color.Red
);

plot Diff = (Speed - SpeedEma2);
Diff.SetDefaultColor(GetColor(5));
Diff.SetPaintingStrategy(PaintingStrategy.HISTOGRAM);
Diff.SetLineWeight(3);
Diff.DefineColor("Positive and Up", CreateColor(0, 155, 255));
Diff.DefineColor("Positive and Down", Color.BLUE);
Diff.DefineColor("Negative and Down", Color.PINK);
Diff.DefineColor("Negative and Up", Color.RED);
Diff.AssignValueColor(if Diff >= 0 then if Diff > Diff[1] then Diff.Color("Positive and Up") else Diff.Color("Positive and Down") else if Diff < Diff[1] then Diff.Color("Negative and Down") else Diff.Color("Negative and Up"));
