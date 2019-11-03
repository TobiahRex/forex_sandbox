declare lower;

input TimeFrame = 10;

#---Variables for MED TF RSI
def TF_med_length = TimeFrame * 14;
def TF_med = (close[1] + close[TimeFrame]) / 2;
def Med_TF_NetChangeAvg = MovingAverage(AverageType.Exponential, TF_med - TF_med[1], TF_med_length);
def Med_TF_TotChangeAvg = MovingAverage(AverageType.Exponential, AbsValue(TF_med - TF_med[1]), TF_med_length);
def Med_TF_ChgRatio = if Med_TF_TotChangeAvg != 0 then (Med_TF_NetChangeAvg / Med_TF_TotChangeAvg) else 0;
plot RSI = 50 * (1 + Med_TF_ChgRatio) - 50;
plot RSI_Avg = ExpAverage(RSI, 8);


#---Local Inputs for MED TF MOMENTUM "MOMO" V2

def Med_MOMOValue1 = 5;
def Med_MOMOValue2 = 55;
def Med_MOMOAvg = 13;
def Med_MOMO_Type = AverageType.EXPONENTIAL;
def Med_MOMOslow = 55;
def Med_MOMO_SlowType = AverageType.EXPONENTIAL;

#---Local Variables for MED TF MOMENTUM "MOMO" V2

def Med_TF_MOMOvalue1 = Med_MOMOValue1 * TimeFrame;
def Med_TF_MOMOvalue2 = Med_MOMOValue2 * TimeFrame;
def Med_TF_MOMOAvg1 = Med_MOMOAvg * TimeFrame;
def Med_TF_MOMOSlow1 = Med_MOMOslow * TimeFrame;
def Med_TF_MOMOValue = MovingAverage(Med_MOMO_Type, RSI, Med_TF_MOMOvalue1) - MovingAverage(Med_MOMO_Type, RSI, Med_TF_MOMOvalue2);
def Med_TF_MOMOAvg = MovingAverage(Med_MOMO_Type, Med_TF_MOMOValue, Med_TF_MOMOAvg1);
def Med_TF_MOMOSlow = MovingAverage(Med_MOMO_SlowType, Med_TF_MOMOValue, Med_TF_MOMOSlow1);

#---RSI Averages

input RSIrawAverage = 8;
input RSIEMAfast = 5;
input RSIEMAslow = 13;
input RSISMA = 34;


plot RSI_GREEN = ExpAverage(RSI(), TimeFrame * RSIrawAverage) - 50;
RSI_GREEN.SetLineWeight(2);
RSI_GREEN.SetDefaultColor(Color.Green);

plot RSI_RED = ExpAverage(RSI, TimeFrame * RSIEMAfast);
RSI_RED.SetLineWeight(2);
RSI_RED.SetDefaultColor(Color.Red);

def RSIslowEMA = ExpAverage(RSI, TimeFrame * RSIEMAslow);
def RSISlowMA = ExpAverage(RSI, TimeFrame * RSISMA);

def RSIgreen = RSI > 9 or RSI_GREEN > 5;
def RSIred = RSI < -9 or RSI_GREEN < -5;
RSI.AssignValueColor(if RSIgreen
  then Color.GREEN else if RSIred
  then Color.RED else Color.GRAY
);

plot OB = 10;
plot _ob = 5;
plot OS = -10;
plot _os = -5;
plot zero = 0;

OB.setdefaultcolor(Color.Magenta);
OS.setdefaultcolor(Color.Magenta);
_ob.setdefaultColor(Color.Pink);
_os.setdefaultColor(Color.Pink);
zero.setdefaultcolor(Color.Yellow);
Zero.SetpaintingStrategy(PaintingStrategy.LINE);

plot Diff = (RSI_GREEN - RSI_RED ) * 10;
Diff.SetDefaultColor(GetColor(5));
Diff.SetPaintingStrategy(PaintingStrategy.HISTOGRAM);
Diff.SetLineWeight(3);
Diff.DefineColor("Positive and Up", CreateColor(0, 155, 255));
Diff.DefineColor("Positive and Down", Color.BLUE);
Diff.DefineColor("Negative and Down", Color.PINK);
Diff.DefineColor("Negative and Up", Color.RED);
Diff.AssignValueColor(if Diff >= 0 then if Diff > Diff[1] then Diff.Color("Positive and Up") else Diff.Color("Positive and Down") else if Diff < Diff[1] then Diff.Color("Negative and Down") else Diff.Color("Negative and Up"));

def RSI_DIFF =  (RSI_GREEN - RSI_GREEN[1]);
plot RSI_SPEED = ExpAverage(RSI_DIFF, 8) * 25;
RSI_SPEED.SetDefaultColor(Color.Cyan);
