declare lower;


input length = 14;
input RSIfast = 8;
input RSIslow = 34;
input RSIAvg = 13;
input lowerB = -20;
input upperB = 20;
plot lwrBound = lowerB;
plot upprBound = upperB;
input price = close;
input averageType = AverageType.EXPONENTIAL;

#----RSI Math
def NetChgAvg = MovingAverage(averageType, price - price[1], length);
def TotChgAvg = MovingAverage(averageType, AbsValue(price - price[1]), length);
def ChgRatio =
    if TotChgAvg != 0
    then NetChgAvg / TotChgAvg
    else 0;
def RSI = 50 * (ChgRatio + 1) ;
#----RSI-H Lines

def RSIDiffAvg = (ExpAverage((((ExpAverage(RSI, RSIfast) * 1.7  - ExpAverage( RSI(), RSIslow) * 1.7)) ), RSIAvg) * 1.5) * .8;
plot OB = 30;
plot OS = -30;
plot RSIValue = ((ExpAverage(RSI, 8) * 1.7) - (ExpAverage( RSI(), 34) * 1.7)) ;
plot RSIema55 = (SimpleMovingAvg(RSI, 55) - 50) * 1.5  ;
plot Zero = 0;


#SIDiffAvg.SetDefaultColor(Color.Green);
#RSIDiffAvg.SetLineWeight(1);
OB.SetDefaultColor(Color.GRAY);
OS.SetDefaultColor(Color.GRAY);
RSIValue.SetDefaultColor(CreateColor(0, 255, 0));
RSIValue.SetLineWeight(1);
RSIema55.SetDefaultColor(Color.LIGHT_GRAY);
RSIema55.SetLineWeight(2);
Zero.SetDefaultColor(Color.LIGHT_GRAY);

RSIema55.AssignValueColor(
    if (RSIema55 > 0 and RSIema55 > RSIema55[1]) then (CreateColor(0, 155, 255)) else
    if (RSIema55 > 0 and RSIema55 < RSIema55[1]) then Color.BLUE else
    if (RSIema55 < 0 and RSIema55 < RSIema55[1]) then Color.PINK else
    if (RSIema55 < 0 and RSIema55 > RSIema55[1]) then Color.RED else
    Color.LIGHT_GRAY);

#plot Diff = (RSIValue - RSIDiffAvg) ;
#plot Diff = (RSIValue - RSIema55);
def Diff2 = (RSIValue - RSIValue[1]);
plot Speed = ExpAverage(Diff2, 10) * 10;
Speed.SetDefaultColor(Color.CYAN);
Speed.SetPaintingStrategy(PaintingStrategy.LINE_VS_POINTS);

plot Diff = Speed - 0;
Diff.SetDefaultColor(GetColor(5));
Diff.SetPaintingStrategy(PaintingStrategy.HISTOGRAM);
Diff.SetLineWeight(3);
Diff.DefineColor("Positive and Up", CreateColor(0, 155, 255));
Diff.DefineColor("Positive and Down", Color.BLUE);
Diff.DefineColor("Negative and Down", Color.PINK);
Diff.DefineColor("Negative and Up", Color.RED);
Diff.AssignValueColor(if Diff >= 0 then if Diff > Diff[1] then Diff.Color("Positive and Up") else Diff.Color("Positive and Down") else if Diff < Diff[1] then Diff.Color("Negative and Down") else Diff.Color("Negative and Up"));

AddCloud(RSIema55, 0, Color.BLUE, Color.RED);



#----EMAs
def EMA8 = ExpAverage(close, 8);
def EMA21 = ExpAverage(close, 21);

def RSI2 = RSI;
#---- RSI Expirement
def _RSIAvg_2 = (ExpAverage((((ExpAverage(RSI2, 8) * 1.7 - ExpAverage(RSI(), 34) * 1.7))), 13) * 1.5);
def _RSISlow_2 = SimpleMovingAvg((((ExpAverage(RSI2, 8) * 1.7 - ExpAverage( RSI(), 34) * 1.7))), 34);
def _RSIValue2_2 = ((ExpAverage(RSI2, 8) * 1.7 - ExpAverage( RSI(), 13) * 1.7));
def _RSIValue_2 = ((ExpAverage(RSI2, 8) * 1.7 - ExpAverage( RSI(), 34) * 1.7));
plot dydxRSI = ((_RSIValue_2 - _RSIValue2_2) / 21) * 50;
dydxRSI.SetDefaultColor(Color.GREEN);
