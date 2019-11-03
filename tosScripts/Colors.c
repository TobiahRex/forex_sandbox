
declare upper;

def price = close;



#--------------------
#----Perspective
#--------------------
input Perspective_length = 21;
def Perspective_EMA = 144;
input Per_averageType = AverageType.WILDERS;

def NetChgAvg = MovingAverage(Per_averageType, price - price[1], Perspective_length);
def TotChgAvg = MovingAverage(Per_averageType, AbsValue(price - price[1]), Perspective_length);
def ChgRatio = if TotChgAvg != 0 then NetChgAvg / TotChgAvg else 0;
def Perspective_RSI = 50 * (ChgRatio + 1);
def RSIValue = ExpAverage(ExpAverage(RSI(), 8) - ExpAverage(RSI(), 21),34);
def RSI55 = SimpleMovingAvg(RSI(),55);
def RSI34 = ExpAverage(RSI(),34);
def RSI144 = SimpleMovingAvg(RSI(), Perspective_EMA);
def RSI13 = ExpAverage(RSI(),13);
def OS = 30;
def OB = 70;

def SmallDiff = RSI13 - RSI55;
def LargeDiff = RSI34 - 50;

def PR = Perspective_RSI;

#--------------------
#----Price Moving Averages
#--------------------
#----- EMA

plot ema13 = ExpAverage(close, 13);
ema13.SetDefaultColor(Color.Cyan);


plot sma55 = SimpleMovingAvg(close, 55);
sma55.SetDefaultColor(CreateColor(0,155,255));
sma55.SetLineWeight(1);

plot SMA89 = SimpleMovingAvg(close, 89);
SMA89.SetDefaultColor(Color.Blue);
SMA89.SetLineWeight(1);


plot SMA144 = SimpleMovingAvg(close, 144);
SMA144.SetDefaultColor(Color.Dark_Orange);

plot SMA200 = SimpleMovingAvg(close, 200);
SMA200.SetDefaultColor(Color.Orange);
SMA200.Setstyle(Curve.LONG_DASH);



#-----------
#---Price Conditions
#-----------
def P_OB = PR > OB;
def P_OS = PR < OS;

def BUYStrong =
    PR > RSI55 and
    SmallDiff > SmallDiff[1] and
    LargeDiff > LargeDiff[1];

def BUY =
    SmallDiff > SmallDiff[1] and
    LargeDiff > LargeDiff[1];

def SellStrong =
    PR < RSI55 and
    SmallDiff < SmallDiff[1] and
    largeDiff < LargeDiff[1];

def Sell =
    SmallDiff < SmallDiff[1] and
    LargeDiff < LargeDiff[1];

def buy =

#--------------------
#----Price Color
#--------------------

AssignPriceColor(
#OB

#Strong
    if BUYStrong then Color.Green else
    if Buy then Color.Orange else
    if SELLStrong then Color.Red else
    if Sell then Color.Magenta else


Color.Dark_GRAY);

#--------------------
#----Arrows
#--------------------


#BUYx.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_DOWN);
#BUYx.SetDefaultColor(Color.BLUE);
#SELLx.SetPaintingStrategy(PaintingStrategy.BOOLEAN_ARROW_UP);
#SELLx.SetDefaultColor(Color.YELLOW);


#--------------------
#----Dots
#--------------------
plot Overbought = P_OB;
plot Oversold = P_OS;

Overbought.SetPaintingStrategy(PaintingStrategy.BOOLEAN_POINTS);
Oversold.SetPaintingStrategy(PaintingStrategy.BOOLEAN_POINTS);
Overbought.SetDefaultColor(Color.Magenta);
Oversold.SetDefaultColor(Color.Cyan);
