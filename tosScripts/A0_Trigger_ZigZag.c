
input BuyLineOS = -50;
input SellLineOB = 50;
input BuyLine = -30;
input SellLine = 30;
input Sensitivity = 5;
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

def Bullet = ExpAverage(RSIValue - RSIValue[1], Sensitivity) * 10;
def SpeedEma = ExpAverage(Bullet, 8);
def SpeedEma2 = ExpAverage(Bullet, 55) * 3;


OB.SetDefaultColor(Color.MAGENTA);
OB.SetStyle(Curve.MEDIUM_DASH);
OB.SetLineWeight(2);
SellZone.SetDefaultColor(Color.RED);
SellZone.SetStyle(Curve.SHORT_DASH);
OS.SetDefaultColor(Color.LIGHT_GREEN);
OS.SetStyle(Curve.MEDIUM_DASH);
BuyZone.SetDefaultColor(Color.GREEN);
BuyZone.SetStyle(Curve.SHORT_DASH);
Zero.SetDefaultColor(Color.LIGHT_GRAY);


# TD Ameritrade IP Company, Inc. (c) 2011-2019
#
declare lower;


def price = Bullet;
input reversalAmount = 1.0;
input showBubbles = no;
input showLabel = no;

plot "ZZ$" = reference ZigZagHighLow(price, price, 0, reversalAmount, 1, 0);

def zzSave = if !IsNaN("ZZ$") then price else GetValue(zzSave, 1);
def chg = price - GetValue(zzSave, 1);
def isUp = chg >= 0;
def isConf = AbsValue(chg) >= reversalAmount or (IsNaN(GetValue("ZZ$", 1)) and GetValue(isConf, 1));

"ZZ$".EnableApproximation();
"ZZ$".DefineColor("Up Trend", Color.Cyan);
"ZZ$".DefineColor("Down Trend", Color.Magenta);
"ZZ$".DefineColor("Undefined", Color.Gray);
"ZZ$".AssignValueColor(if !isConf then "ZZ$".Color("Undefined") else if isUp then "ZZ$".Color("Up Trend") else "ZZ$".Color("Down Trend"));

DefineGlobalColor("Unconfirmed", Color.DARK_ORANGE);
DefineGlobalColor("Up", Color.UPTICK);
DefineGlobalColor("Down", Color.DOWNTICK);


AddCloud(SellLine, SellLineOB, Color.RED);
AddCloud(BuyLine, BuyLineOS, Color.GREEN);
AddCloud(SpeedEma, SpeedEma2, Color.Green, Color.Red);
AddCloud(SpeedEma, SpeedEma2, Color.Green, Color.Red);

# ------ Trigger Alert ------
def trigZZValue = "ZZ$";
input T_Alert_Op = { default "Disabled", "Greater Than", "Less Than", "Crosses Above", "Crosses Below" };
input alert_Trigger = 50;

def t_expression;
switch (T_Alert_Op) {
case "Disabled":
    t_expression = no;
case "Greater Than":
    t_expression = trigZZValue > alert_trigger;
case "Less Than":
    t_expression = trigZZValue < alert_trigger;
case "Crosses Above":
    t_expression = trigZZValue crosses above alert_trigger;
case "Crosses Below":
    t_expression = trigZZValue crosses below alert_trigger;
}
Alert(
    t_expression,
    "Trigger Alert",
    Alert.TICK,
    Sound.Bell
);
#AddLabel(yes, AsText(x, NumberFormat.TWO_DECIMAL_PLACES), Color.WHITE);
#-----------------------------