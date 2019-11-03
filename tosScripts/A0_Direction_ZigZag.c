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
plot rsiBear = ExpAverage(_rsi, timeFrame * 5);

plot rsiBull = ExpAverage(RSI(), timeFrame * 8) - 50;


_rsi.AssignValueColor(
  if (_rsi > _rsiEma and _rsi > _rsi[1]) then Color.Green
  else if (_rsi > _rsiEma and _rsi < _rsi[1]) then Color.Dark_Green
  else if (_rsi < _rsiEma and _rsi < _rsi[1]) then Color.Red
  else Color.Dark_Red
);
rsiBull.SetLineWeight(2);
rsiBull.SetDefaultColor(Color.Yellow);
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
input showBubbles = no;
input showLabel = no;

plot ZZ = reference ZigZagHighLow(price, price, 0, reversalAmount, 1, 0);

def zzSave = if !IsNaN(ZZ) then price else GetValue(zzSave, 1);
def chg = price - GetValue(zzSave, 1);
def isUp = chg >= 0;
def isConf = AbsValue(chg) >= reversalAmount or (IsNaN(GetValue(ZZ, 1)) and GetValue(isConf, 1));

ZZ.EnableApproximation();
ZZ.DefineColor("Up Trend", Color.Green);
ZZ.DefineColor("Down Trend", Color.Red);
ZZ.DefineColor("Undefined", Color.Gray);
ZZ.AssignValueColor(if !isConf then ZZ.Color("Undefined") else if isUp then ZZ.Color("Up Trend") else ZZ.Color("Down Trend"));
ZZ.SetLineWeight(3);
ZZ.SetPaintingStrategy(PaintingStrategy.LINE_VS_POINTS);

DefineGlobalColor("Unconfirmed", Color.Gray);
DefineGlobalColor("Up", Color.Green);
DefineGlobalColor("Down", Color.Red);

def barNumber = BarNumber();

AddChartBubble(
  showBubbles and !IsNaN(ZZ) and barNumber != 1,
  price,
  chg,
  if !isConf then GlobalColor("Unconfirmed")
  else
    if isUp then GlobalColor("Up")
    else GlobalColor("Down"), isUp);

AddLabel(
  showLabel and barNumber != 1,
  (if isConf then "Confirmed " else "Unconfirmed ") + "ZigZag: " + chg,
  if !isConf then GlobalColor("Unconfirmed")
  else
    if isUp then GlobalColor("Up")
    else GlobalColor("Down"));
