#
# TS_BetterVolume_Indicator
#
# A better volume indicator with color coded histogram plot that
# shows 4 different colors depending on price action and volume
#
# Yellow - Low volume for the size of the bar - Amateurs at work
# White - Climax down
# Blue -Climax up
# Red - Churn - Pros buying at lows or unloading at highs
# Magenta - Climax Churn - Pros taking profits at highs or lows
# Gray - Normal (default) Bar
#
declare lower;
#
input lookback = 8;
input Use2Bars = {"No", default "Yes"};
input LowVol = {"No",default "Yes"};
input ClimaxUp = {"No",default "Yes"};
input ClimaxDown = {"No",default "Yes"};
input Churn = {"No",default "Yes"};
input ClimaxChurn = {"No",default "Yes"};

def LowVolColor = 8;
def ClimaxUpColor = 9;
def ClimaxDownColor = 1;
def ChurnColor = 5;
def ClimaxChurnColor = 0;
def DefaultBarColor = 7;
def AvgColor = 0;

def Range = TrueRange(high,low,close);
def Value1 = if (close >= open, Volume * ((Range) / ((2+(Range*Range)/10) * Range + (open - close))),
Volume * (((Range + close - open)) / (2+(Range*Range)/10) * Range + (close - open)));
def Value2 = Volume - Value1;

def Value3 = Value1 + Value2;
def Value4 = Value1 * Range;
def Value5 = (Value1 - Value2) * Range;
def Value6 = Value2 * Range;
def Value7 = (Value2 - Value1) * Range;
def Value8 = if (Range <> 0, Value1 / Range, 1);
def Value9 = if (Range <> 0, (Value1 - Value2) / Range, 1);
def Value10 = if (Range <> 0, Value2 / Range, 1);
def Value11 = if (Range <> 0, (Value2 - Value1) / Range, 1);
def Value12 = if (Range <> 0, Value3 / Range, 1);
def Value13 = if (Use2Bars, Value3 + Value3[1],1);
def Value14 = if (Use2Bars, (Value1 + Value1[1])*(Highest(High,2)-Lowest(Low,2)),1);
def Value15 = if (Use2Bars, (Value1 + Value1[1]-Value2-Value2[1])*(Highest(High,2)-Lowest(Low,2)),1);
def Value16 = if (Use2Bars, (Value2 + Value2[1])*(Highest(High,2)-Lowest(Low,2)),1);
def Value17 = if (Use2Bars, (Value2 + Value2[1]-Value1-Value1[1])*(Highest(High,2)-Lowest(Low,2)),1);
def Value18 = if ((Use2Bars and (Highest(High,2)<>Lowest(Low,2))),(Value1+Value1[1])/(Highest(High,2)-Lowest(Low,2)),1);
def Value19 = if ((Use2Bars and (Highest(High,2)<>Lowest(Low,2))),(Value1+Value1[1]-Value2-Value2[1])/(Highest(High,2)-Lowest(Low,2)),1);
def Value20 = if ((Use2Bars and (Highest(High,2)<>Lowest(Low,2))),(Value2+Value2[1])/(Highest(High,2)-Lowest(Low,2)),1);
def Value21 = if ((Use2Bars and (Highest(High,2)<>Lowest(Low,2))),(Value2+Value2[1]-Value1-Value1[1])/(Highest(High,2)-Lowest(Low,2)),1);
def Value22 = if ((Use2Bars and (Highest(High,2)<>Lowest(Low,2))),Value13/(Highest(High,2)-Lowest(Low,2)),1);


def Condition1 = if(Value3 == Lowest(Value3, Lookback), 1, 0);
def Condition2 = if((Value4 == Highest(Value4, Lookback) and close > open), 1, 0);
def Condition3 = if((Value5 == Highest(Value5, Lookback) and close > open), 1, 0);
def Condition4 = if((Value6 == Highest(Value6, Lookback) and close < open), 1, 0);
def Condition5 = if((Value7 == Highest(Value7, Lookback) and close < open), 1, 0);
def Condition6 = if((Value8 == Lowest(Value8, Lookback) and close < open), 1, 0);
def Condition7 = if((Value9 == Lowest(Value9, Lookback) and close < open), 1, 0);
def Condition8 = if((Value10 == Lowest(Value10, Lookback) and close > open), 1, 0);
def Condition9 = if((Value11 == Lowest(Value11, Lookback) and close > open), 1, 0);
def Condition10 = if(Value12 == Highest(Value12, lookback), 1, 0);
def Condition11 = if (Use2Bars and (Value13==Lowest(Value13,Lookback) and close > open and Close[1] > Open[1]),1,0);
def Condition12 = if (Use2Bars and (Value14==Highest(Value14,Lookback) and close > open and Close[1] > Open[1]),1,0);
def Condition13 = if (Use2Bars and (Value15==Highest(Value15,Lookback) and close > open and Close[1] < Open[1]),1,0);
def Condition14 = if (Use2Bars and (Value16==Lowest(Value16,Lookback) and close < open and Close[1] < Open[1]),1,0);
def Condition15 = if (Use2Bars and (Value17==Lowest(Value17,Lookback) and close < open and Close[1] < Open[1]),1,0);
def Condition16 = if (Use2Bars and (Value18==Lowest(Value18,Lookback) and close < open and Close[1] < Open[1]),1,0);
def Condition17 = if (Use2Bars and (Value19==Lowest(Value19,Lookback) and close > open and Close[1] < Open[1]),1,0);
def Condition18 = if (Use2Bars and (Value20==Lowest(Value20,Lookback) and close > open and Close[1] > Open[1]),1,0);
def Condition19 = if (Use2Bars and (Value21==Lowest(Value21,Lookback) and close > open and Close[1] > Open[1]),1,0);
def Condition20 = if (Use2Bars and (Value22==Lowest(Value22,Lookback)), 1,0);

plot Vol = volume;
plot VolAvg = Average(volume, lookback);


Vol.SetPaintingStrategy(PaintingStrategy.HISTOGRAM);
Vol.SetLineWeight(3);
Vol.AssignValueColor (
if (ClimaxChurn and (Condition10 or Condition20)) and (Condition2 or Condition3 or Condition4 or Condition5 or Condition6 or Condition7 or Condition8 or Condition9) then GetColor(ClimaxChurnColor) else
if (LowVol and (Condition1 or Condition11)) then GetColor(LowVolColor) else
if (ClimaxUp and (Condition2 or Condition3 or Condition8 or Condition9 or Condition12 or Condition13 or Condition18 or Condition19)) then GetColor(ClimaxUpColor) else
if (ClimaxDown and (Condition4 or condition5 or Condition6 or Condition7 or Condition14 or Condition15 or Condition16 or Condition17)) then GetColor(ClimaxDownColor) else
if (Churn and Condition10 or Condition20) then GetColor(ChurnColor) else

GetColor(DefaultBarColor));

VolAvg.SetDefaultColor(GetColor(AvgColor));
