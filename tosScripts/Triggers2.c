#==================================================================
#==========================A0 Direction===========================
#==================================================================
input TimeFrame = 10;

def TF_med_length = TimeFrame * 14;
def TF_med = (close[1] + close[TimeFrame]) / 2;
def Med_TF_NetChangeAvg = MovingAverage(AverageType.Exponential, TF_med - TF_med[1], TF_med_length);
def Med_TF_TotChangeAvg = MovingAverage(AverageType.Exponential, AbsValue(TF_med - TF_med[1]), TF_med_length);
def Med_TF_ChgRatio = if Med_TF_TotChangeAvg != 0 then (Med_TF_NetChangeAvg / Med_TF_TotChangeAvg) else 0;

def RSI = 50 * (1 + Med_TF_ChgRatio) - 50;
def RSI_Avg = SimpleMovingAvg(RSI, 8);

#---Local Inputs for MED TF MOMENTUM "MOMO" V2

def Med_MOMOValue1 = 5;
def Med_MOMOValue2 = 55;
def Med_MOMOAvg = 13;
def Med_MOMO_Type = AverageType.EXPONENTIAL;
def Med_MOMOslow = 55;
def Med_MOMO_SlowType = AverageType.EXPONENTIAL;


#---RSI Averages

plot RSIrawEMA = ExpAverage(RSI(), TimeFrame * 8) - 50;
def RSIgreen = RSI > 9 or RSIrawEMA > 5;
def RSIred = RSI < -9 or RSIrawEMA < -5;

plot RSIfastEMA = ExpAverage(RSI, TimeFrame * 5);
def RSIslowEMA = ExpAverage(RSI, TimeFrame * 13);
def RSISlowMA = ExpAverage(RSI, TimeFrame * 34);


RSIrawEMA.AssignValueColor(if RSIgreen
then Color.GREEN else if RSIred
then Color.RED else Color.GRAY);

AddCloud(RSIrawEMA, RSIfastEMA, Color.Green, Color.Red);


plot Diff = (RSIrawEMA - RSIfastEMA) * 2;

Diff.SetDefaultColor(GetColor(5));
Diff.SetPaintingStrategy(PaintingStrategy.HISTOGRAM);
Diff.SetLineWeight(3);
Diff.DefineColor("Positive and Up", CreateColor(0, 155, 255));
Diff.DefineColor("Positive and Down", Color.BLUE);
Diff.DefineColor("Negative and Down", Color.PINK);
Diff.DefineColor("Negative and Up", Color.RED);
Diff.AssignValueColor(if Diff >= 0 then if Diff > Diff[1] then Diff.Color("Positive and Up") else Diff.Color("Positive and Down") else if Diff < Diff[1] then Diff.Color("Negative and Down") else Diff.Color("Negative and Up"));



#==================================================================
#==========================A0 Triggers 2===========================
#==================================================================

#--------------------------------
#------------RSI Math------------
#--------------------------------
input lowerB = -20;
input upperB = 20;
def lwrBound = lowerB;
def upprBound = upperB;

def NetChgAvg = MovingAverage(AverageType.EXPONENTIAL, close - close[1], 14);
def TotChgAvg = MovingAverage(AverageType.EXPONENTIAL, AbsValue(close - close[1]), 14);
def ChgRatio =
    if (TotChgAvg != 0) then NetChgAvg / TotChgAvg
    else 0;
def _RSI = 50 * (ChgRatio + 1) ;

#--------------------------------
#------------RSI EMAs------------
#--------------------------------
def rsiFastExp = ExpAverage(_RSI, 8);
def rsiSlowExp = ExpAverage(_RSI, 34);
def RSIDiffAvg = (
  ExpAverage(
    (rsiFastExp  - rsiSlowExp),
    13
  ) * 1.5
) * .8;
def RSIValue = (
  ExpAverage(_RSI, 8) - ExpAverage(RSI(), 34)
) ;

#---------------------------------------------------------------
#----- rsiValueDiff: The change in RSI Value relative to its last value
#----- Speed: The average of Diff2 over 13 periods.
#---------------------------------------------------------------
def rsiValueDiff = (RSIValue - RSIValue[1]);
def Speed = ExpAverage(rsiValueDiff, 13) * 10;
def RSIema55 = (SimpleMovingAvg(_RSI, 55) - 50) * 1.5;


#------------------------
#--- dydx: The difference of 2 EXP Averages over a short distance (A)
#--- and the difference of 2 EXP Averages over a longer distance (B)
#--- and the difference between A & B, divided by a Medium distance.
#--- WHY ---:  Its meant to emulate how derivatives are calculated to provide instantaneous,
#--- change over time...ideally more responsive than "average".
#------------------------
def _RSIValueFaster = ((ExpAverage(_RSI, 8) * 1.7 - ExpAverage( RSI(), 13) * 1.7));
def _RSIValueSlower = ((ExpAverage(_RSI, 8) * 1.7 - ExpAverage( RSI(), 34) * 1.7));
def dydxRSI = ((_RSIValueSlower - _RSIValueFaster) / 21) * 50;
