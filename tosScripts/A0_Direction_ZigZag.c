declare lower;

input timeFrame = 10;
input rsiEmaLength = 13;
input range_top = 3;
input range_bottom = -3;
plot rTop = range_top;
plot rBottom = range_bottom;
plot zero = 0;
plot std_pos2 = 12.39;
plot std_pos1 = 7.97;
plot std_neg1 = -7.97;
plot std_neg2 = -12.39;

def timeFrameLength = timeFrame * 13;
# ------ RSI Calculation ------
def averageClose = (close + close[timeFrame]) / 2;
def avgNetChange = MovingAverage(AverageType.Exponential, averageClose - averageClose[1], timeFrameLength);
def avgTotalChange = MovingAverage(AverageType.Exponential, AbsValue(averageClose - averageClose[1]), timeFrameLength);
def changeRatio = if avgTotalChange != 0 then (avgNetChange / avgTotalChange) else 0;
plot _rsi = 50 * (1 + changeRatio) - 50;
# ------ RSI Ema ------
plot _rsiEma = ExpAverage(_rsi, rsiEmaLength);
_rsi.AssignValueColor(
    if (_rsi > _rsiEma and _rsi > _rsi[1]) then Color.Green
    else if (_rsi > _rsiEma and _rsi < _rsi[1]) then Color.Dark_Green
    else if (_rsi < _rsiEma and _rsi < _rsi[1]) then Color.Red
    else Color.Dark_Red
);
plot rsiBear = ExpAverage(_rsi, timeFrame * 5);
# ------ RSI Colors & Clouds ------
rTop.SetDefaultColor(Color.Gray);
rBottom.SetDefaultColor(Color.Gray);
AddCloud(rTop, 0, Color.Dark_Gray, Color.Dark_Gray);
AddCloud(rBottom, 0, Color.Dark_Gray, Color.Dark_Gray);
AddCloud(rBottom, 0, Color.Dark_Gray, Color.Dark_Gray);
AddCloud(_rsiEma, rsiBear, Color.Green, Color.Red);
_rsi.SetDefaultColor(Color.Cyan);
_rsi.SetpaintingStrategy(PaintingStrategy.LINE_VS_POINTS);
_rsiEma.SetDefaultColor(Color.Blue);
_rsiEma.SetLineWeight(3);
_rsiEma.AssignValueColor(
    if _rsiEma > _rsiEma[1] then CreateColor(68, 154, 255)
    else  CreateColor(232, 111, 0)
);
zero.setdefaultcolor(Color.Gray);
Zero.SetpaintingStrategy(PaintingStrategy.LINE);
rsiBear.SetLineWeight(1);
rsiBear.SetDefaultColor(Color.Magenta);
# ------ RSI Zig Zags ------
def price = _rsi;
input reversalAmount = 1.0;
plot ZZ = reference ZigZagHighLow(_rsi, _rsi, 0, reversalAmount, 1, 0);
def zzSave = if !IsNaN(ZZ) then _rsi else GetValue(zzSave, 1);
def chg = _rsi - GetValue(zzSave, 1);
def isUp = chg >= 0;
def confirmedZZ = AbsValue(chg) >= reversalAmount or (IsNaN(GetValue(ZZ, 1)) and GetValue(confirmedZZ, 1));
ZZ.EnableApproximation();
ZZ.DefineColor("Up Trend", Color.Cyan);
ZZ.DefineColor("Down Trend", Color.Magenta);
ZZ.DefineColor("Undefined", Color.Gray);
ZZ.AssignValueColor(if !confirmedZZ then ZZ.Color("Undefined") else if isUp then ZZ.Color("Up Trend") else ZZ.Color("Down Trend"));
ZZ.SetLineWeight(3);
ZZ.SetPaintingStrategy(PaintingStrategy.LINE_VS_POINTS);

# ------ ZZ Slow Source Choice ------
input ZZ_slow_source = { default "rsi EMA", "rsi Slow"};
input alert_Trigger = 50;

def slow_source;
switch (ZZ_slow_source) {
case "rsi EMA":
    slow_source = _rsiEma;
case "rsi Slow":
    slow_source = rsiBear;
}
#-----------------------------

def slowZZ = slow_source;
input reversalAmount_slow = 1.0;
plot ZZ_slow = reference ZigZagHighLow(slowZZ, slowZZ, 0, reversalAmount_slow, 1, 0);

def zzSave_slow = if !IsNaN(ZZ_slow) then _rsi else GetValue(zzSave_slow, 1);
def chg_slow = _rsi - GetValue(zzSave_slow, 1);
def isUp_slow = chg_slow >= 0;
def confirmedZZSlow = AbsValue(chg_slow) >= reversalAmount_slow or (IsNaN(GetValue(ZZ_slow, 1)) and GetValue(confirmedZZSlow, 1));
ZZ_slow.EnableApproximation();
ZZ_slow.DefineColor("Up Trend", Color.Cyan);
ZZ_slow.DefineColor("Down Trend", Color.Magenta);
ZZ_slow.DefineColor("Undefined", Color.Gray);
ZZ_slow.AssignValueColor(if !confirmedZZSlow then ZZ_slow.Color("Undefined") else if isUp_slow then ZZ_slow.Color("Up Trend") else ZZ_slow.Color("Down Trend"));
ZZ_slow.SetLineWeight(4);
#-----------------------------------------------
# ------------- ZZ Slow | Counts  --------------
#-----------------------------------------------
def zzSlowCurrPolarity = if GetValue(zzSave_slow, 0) > GetValue(zzSave_slow, 1) then 1 else -1;
def zzSlowCurrDistance = (
  fold i = 0 to 100 with currentDist = 0
  while (
    if (zzSlowCurrPolarity < 0) then (GetValue(zzSave_slow, i) <= GetValue(zzSave_slow, i + 1))
    else (GetValue(zzSave_slow, i) >= GetValue(zzSave_slow, i + 1))
  ) do (
    currentDist + 1
  )
);
def zzSlowPrevPolarity = if zzSlowCurrPolarity > 0 then -1 else 1;
def zzSlowPrevDist = (
  fold j = zzSlowCurrDistance to (zzSlowCurrDistance + 100) with prevDist = 0
  while (
    if (zzSlowCurrPolarity > 0) then (GetValue(zzSave_slow, j) <= GetValue(zzSave_slow, j + 1))
    else (GetValue(zzSave_slow, j) >= GetValue(zzSave_slow, j + 1))
  ) do (
    prevDist + 1
  )
);

#-----------------------------------------------
# ------------- ZZ Slow | Theta  ---------------
#-----------------------------------------------
# Shows Current Angle of Direction ZZ
def zzSlowCurrHeight = (
  if zzSlowCurrPolarity > 0 then AbsValue(GetValue(ZZ_Slow, 0) - GetValue(ZZ_Slow, zzSlowCurrDistance))
  else AbsValue(GetValue(ZZ_Slow, zzSlowCurrDistance) - GetValue(ZZ_Slow, 0))
);

def currAngle = ASin(zzSlowCurrHeight / zzSlowCurrDistance) * 180 / Double.Pi;
def currTheta = (
  if zzSlowCurrPolarity > 0 then currAngle
  else RoundDown(180 - (currAngle + 90) - 90, 2)
);

# Shows Previous Angle of Direction ZZ
def zzSlowPrevHeight = (
  if zzSlowPrevPolarity > 0 then AbsValue(GetValue(ZZ_Slow, zzSlowCurrDistance) - GetValue(ZZ_Slow, zzSlowCurrDistance + zzSlowPrevDist))
  else AbsValue(GetValue(ZZ_Slow, zzSlowCurrDistance + zzSlowPrevDist) - GetValue(ZZ_Slow, zzSlowCurrDistance))
);

def prevAngle = ASin(zzSlowPrevHeight / zzSlowPrevDist) * 180 / Double.Pi;
def prevTheta = (
  if zzSlowPrevPolarity > 0 then prevAngle
  else RoundDown(180 - (prevAngle + 90) - 90, 2)
);
AddLabel(1, "Curr Dist: " + zzSlowCurrDistance, if (zzSlowCurrPolarity > 0) then Color.Uptick else Color.Red);
AddLabel(
  1,
  "Curr θ: " + (if zzSlowCurrPolarity > 0 then "+" else "") + RoundDown(currTheta, 2),
  if (zzSlowCurrPolarity > 0) then Color.Uptick else Color.Red
);
AddLabel(1, "Prev Dist: " + zzSlowPrevDist, if (zzSlowCurrPolarity > 0) then Color.Red else Color.Uptick);
AddLabel(
  1,
  "Prev θ: " + (if zzSlowPrevPolarity > 0 then "+" else "") + RoundDown(prevTheta, 2),
  if (zzSlowPrevPolarity > 0) then Color.Uptick else Color.Red
);

#-----------------------------------------------
# ------------- ZZ Slow | Force  ---------------
#-----------------------------------------------
# Force = Mass * Acceleration
# Formula for Acceleration === v2^2-v1^2 / (2*distance)
# We calculate Mass by looping through all the data points in the current count and counting their absolute values.


# The value at the beginning of the current slope.
#AddLabel(1, GetValue(ZZ_slow, zzSlowCurrDistance), Color.WHITE);
def slope = (
  zzSlowCurrHeight / zzSlowCurrDistance * (if zzSlowCurrPolarity > 0 then 1 else -1)
);
def mass = (
  fold k = 0 to zzSlowCurrDistance with massCount = 0
  while 1 do (
    # tangent value = zzSave_slow - slope
    # distance to tangent = GetValue(_rsi, k) - tangent value
    # we must multiply the slope by the current iteration to get the historical value
    massCount + (
      AbsValue(
        GetValue(_rsi, k) - (
          zzSave_slow - (slope * k)
        )
      )
    )
  )
);
def acceleration = (
  Power(
    AbsValue(
      GetValue(ZZ_Slow, zzSlowCurrDistance) - _rsi
    ),
    2
  ) - Power(
    AbsValue(
      GetValue(ZZ_Slow, zzSlowCurrDistance) - GetValue(_rsi, zzSlowCurrDistance)
    ),
    2
  ) / (2 * Sqrt(Power(zzSlowCurrDistance, 2) + Power(zzSlowCurrHeight, 2)))
);
#Addlabel(1, "Distance: " + Sqrt(Power(zzSlowCurrDistance, 2) + Power(zzSlowCurrHeight, 2)), Color.White);
#AddLabel(1, "Mass: " + mass, Color.White);
#AddLabel(1, "Acceleration: " + acceleration, Color.White);
AddLabel(1, "Force: " + RoundDown(mass * acceleration, 0) * zzSlowCurrPolarity, Color.Cyan);
#-----------------------------------------------
# ------------- ZZ Slow | HISTOGRAM  -----------
#-----------------------------------------------
plot Diff = _rsiEma - rsiBear;
Diff.SetDefaultColor(GetColor(5));
Diff.SetPaintingStrategy(PaintingStrategy.HISTOGRAM);
Diff.SetLineWeight(3);
Diff.DefineColor("Positive and Up", Color.GREEN);
Diff.DefineColor("Positive and Down", Color.DARK_GREEN);
Diff.DefineColor("Negative and Down", Color.RED);
Diff.DefineColor("Negative and Up", Color.DARK_RED);
Diff.AssignValueColor(if Diff >= 0 then if Diff > Diff[1] then Diff.Color("Positive and Up") else Diff.Color("Positive and Down") else if Diff < Diff[1] then Diff.Color("Negative and Down") else Diff.Color("Negative and Up"));

