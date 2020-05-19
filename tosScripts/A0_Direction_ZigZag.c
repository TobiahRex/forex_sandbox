declare lower;

input timeFrame = 10;
input rsiEma = 13;

def timeFrameLength = timeFrame * 13;
def averageClose = (close + close[timeFrame]) / 2;
def avgNetChange = MovingAverage(AverageType.Exponential, averageClose - averageClose[1], timeFrameLength);
def avgTotalChange = MovingAverage(AverageType.Exponential, AbsValue(averageClose - averageClose[1]), timeFrameLength);

def changeRatio = if avgTotalChange != 0 then (avgNetChange / avgTotalChange) else 0;

plot _rsi = 50 * (1 + changeRatio) - 50;
plot _rsiEma = ExpAverage(_rsi, rsiEma);

_rsi.AssignValueColor(
    if (_rsi > _rsiEma and _rsi > _rsi[1]) then Color.Green
    else if (_rsi > _rsiEma and _rsi < _rsi[1]) then Color.Dark_Green
    else if (_rsi < _rsiEma and _rsi < _rsi[1]) then Color.Red
    else Color.Dark_Red
);

def rsiValueEma = (
  MovingAverage(AverageType.EXPONENTIAL, _rsi, 5 * timeFrame)
  - MovingAverage(AverageType.EXPONENTIAL, _rsi, 55 * timeFrame)
);
def rsiValueEmaFast = MovingAverage(AverageType.EXPONENTIAL, rsiValueEma, 13 * timeFrame);
def rsiValueEmaSlow = MovingAverage(AverageType.EXPONENTIAL, rsiValueEma, 55 * timeFrame);


plot rsiBull = ExpAverage(RSI(), TimeFrame * 8) - 50;
rsiBull.SetLineWeight(2);
rsiBull.SetDefaultColor(Color.Yellow);
plot rsiBear = ExpAverage(_rsi, timeFrame * 5);

plot zero = 0;
AddCloud(_rsiEma, rsiBear, Color.Green, Color.Red);
_rsi.SetDefaultColor(Color.Cyan);
_rsi.SetpaintingStrategy(PaintingStrategy.LINE_VS_POINTS);
_rsiEma.SetDefaultColor(Color.Blue);
_rsiEma.SetLineWeight(3);
_rsiEma.AssignValueColor(
    if _rsiEma > _rsiEma[1] then CreateColor(68, 154, 255)
    else  CreateColor(232, 111, 0)
);
zero.setdefaultcolor(Color.Dark_Gray);
Zero.SetpaintingStrategy(PaintingStrategy.LINE);
rsiBear.SetLineWeight(1);
rsiBear.SetDefaultColor(Color.Magenta);

def price = _rsi;
input reversalAmount = 1.0;

plot ZZ = reference ZigZagHighLow(price, price, 0, reversalAmount, 1, 0);

def zzSave = if !IsNaN(ZZ) then price else GetValue(zzSave, 1);
def chg = price - GetValue(zzSave, 1);
def isUp = chg >= 0;
def isConf = AbsValue(chg) >= reversalAmount or (IsNaN(GetValue(ZZ, 1)) and GetValue(isConf, 1));

ZZ.EnableApproximation();
ZZ.DefineColor("Up Trend", Color.Cyan);
ZZ.DefineColor("Down Trend", Color.Magenta);
ZZ.DefineColor("Undefined", Color.Gray);
ZZ.AssignValueColor(if !isConf then ZZ.Color("Undefined") else if isUp then ZZ.Color("Up Trend") else ZZ.Color("Down Trend"));
ZZ.SetLineWeight(3);
ZZ.SetPaintingStrategy(PaintingStrategy.LINE_VS_POINTS);

