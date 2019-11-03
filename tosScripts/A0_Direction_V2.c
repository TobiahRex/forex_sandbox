declare lower;

input TimeFrame = 10;

#---Variables for MED TF RSI
def TF_med_length = TimeFrame * 13;
def TF_med = (close[1] + close[TimeFrame]) / 2;
def Med_TF_NetChangeAvg = MovingAverage(AverageType.Exponential, TF_med - TF_med[1], TF_med_length);
def Med_TF_TotChangeAvg = MovingAverage(AverageType.Exponential, AbsValue(TF_med - TF_med[1]), TF_med_length);

def Med_TF_ChgRatio = if Med_TF_TotChangeAvg != 0 then (Med_TF_NetChangeAvg / Med_TF_TotChangeAvg) else 0;

plot RSI = 50 * (1 + Med_TF_ChgRatio) - 50;
plot RSI_Avg = ExpAverage(RSI, 13);
RSI_Avg.SetDefaultColor(Color.White);

RSI_Avg.AssignValueColor(
    if RSI_Avg > RSI_Avg[1] then CreateColor(68, 154, 255)
    else  CreateColor(232, 111, 0)
);

RSI.AssignValueColor(
    if (RSI > RSI_Avg) then Color.Green
    else Color.Red
);


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


plot RSI_BULL = ExpAverage(RSI(), TimeFrame * RSIrawAverage) - 50;
RSI_BULL.SetLineWeight(2);
RSI_BULL.SetDefaultColor(Color.Yellow);

plot RSI_BEAR = ExpAverage(RSI, TimeFrame * RSIEMAfast);
RSI_BEAR.SetLineWeight(2);
RSI_BEAR.SetDefaultColor(Color.Magenta);

def RSIslowEMA = ExpAverage(RSI, TimeFrame * RSIEMAslow);
def RSISlowMA = ExpAverage(RSI, TimeFrame * RSISMA);

def RSIgreen = RSI > 9 or RSI_BULL > 5;
def RSIred = RSI < -9 or RSI_BULL < -5;
RSI.SetDefaultColor(Color.Cyan);
RSI.SetpaintingStrategy(PaintingStrategy.LINE_VS_POINTS);
plot OB = 10;
plot _ob = 5;
plot OS = -10;
plot _os = -5;
plot zero = 0;

OB.Setdefaultcolor(Color.Magenta);
OB.SetStyle(Curve.MEDIUM_DASH);
OB.SetLineWeight(2);

OS.SetDefaultColor(Color.Light_Green);
OS.SetStyle(Curve.MEDIUM_DASH);
OS.SetLineWeight(2);

_ob.SetdefaultColor(Color.Red);
_ob.SetStyle(Curve.Short_DASH);

_os.setdefaultColor(Color.Green);
_os.SetStyle(Curve.Short_DASH);

zero.setdefaultcolor(Color.Dark_Gray);
Zero.SetpaintingStrategy(PaintingStrategy.LINE);


def RSI_DIFF =  (RSI_BULL - RSI_BULL[1]);
