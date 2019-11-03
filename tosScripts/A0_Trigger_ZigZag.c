
input BuyLineOS = -50;
input SellLineOB = 50;
input BuyLine = -30;
input SellLine = 30;
input Sensitivity = 5;
plot BuyZone = BuyLine;
plot SellZone = SellLine;

def netChange = MovingAverage(AverageType.EXPONENTIAL, close - close[1], 14);
def totalChange = MovingAverage(AverageType.EXPONENTIAL, AbsValue(close - close[1]), 14);
def changeRatio =
    if totalChange != 0
    then netChange / totalChange
    else 0;

def _rsi = 50 * (changeRatio + 1) ;
def rsiAverage = ((ExpAverage(_rsi, 8) * 1.7) - (ExpAverage(RSI(), 34) * 1.7)) ;

plot Zero = 0;
plot OB = SellLineOB;
plot OS = BuyLineOS;
plot Bullet = ExpAverage(rsiAverage - rsiAverage[1], Sensitivity) * 10;
plot SpeedEma = ExpAverage(Bullet, 8);
plot SpeedEma2 = ExpAverage(Bullet, 55) * 3;


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
SpeedEma.SetDefaultColor(Color.Orange);
SpeedEma.SetLineWeight(2);
SpeedEma2.SetDefaultColor(Color.Magenta);
SpeedEma2.SetLineWeight(2);

Bullet.AssignValueColor(
    if (Bullet < SpeedEma2 && Bullet < Bullet[1]) then Color.Dark_Green
    else if (Bullet < SpeedEma2 && Bullet > Bullet[1]) then Color.Green
    else if (Bullet > SpeedEma2 && Bullet < Bullet[1]) then Color.Red
    else Color.Dark_Red
);



# TD Ameritrade IP Company, Inc. (c) 2011-2019
#
declare lower;


def price = Bullet;
input reversalAmount = 1.0;
input showBubbles = no;
input showLabel = no;

plot "ZZ$" = reference ZigZagHighLow(price, price, 0, reversalAmount, 1, 0);

def zzSave = if !IsNaN("ZZ$") then price else getValue(zzSave, 1);
def chg = price - getValue(zzSave, 1);
def isUp = chg >= 0;
def isConf = AbsValue(chg) >= reversalAmount or (IsNaN(getValue("ZZ$", 1)) and getValue(isConf, 1));

"ZZ$".EnableApproximation();
"ZZ$".DefineColor("Up Trend", Color.UPTICK);
"ZZ$".DefineColor("Down Trend", Color.DOWNTICK);
"ZZ$".DefineColor("Undefined", Color.DARK_ORANGE);
"ZZ$".AssignValueColor(if !isConf then "ZZ$".color("Undefined") else if isUp then "ZZ$".color("Up Trend") else "ZZ$".color("Down Trend"));

DefineGlobalColor("Unconfirmed", Color.DARK_ORANGE);
DefineGlobalColor("Up", Color.UPTICK);
DefineGlobalColor("Down", Color.DOWNTICK);

def barNumber = barNumber();

AddChartBubble(showBubbles and !IsNaN("ZZ$") and barNumber != 1, price, chg, if !isConf then globalColor("Unconfirmed") else if isUp then globalColor("Up") else globalColor("Down"), isUp);
AddLabel(showLabel and barNumber != 1, (if isConf then "Confirmed " else "Unconfirmed ") + "ZigZag: " + chg, if !isConf then globalColor("Unconfirmed") else if isUp then globalColor("Up") else globalColor("Down"));
