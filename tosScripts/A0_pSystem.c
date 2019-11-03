declare lower;
#==================================================================
#==========================Moving Averages=========================
#==================================================================
input fast = 8;
input medium = 21;
input slow = 55;
input slowest = 89;

def EMA_fast = ExpAverage(close, fast);
def EMA_slow = ExpAverage(close, medium);
def SMA_fast = SimpleMovingAvg(close, slow);
def SMA_slow = SimpleMovingAVg(close, slowest);

def buyAvgs = EMA_fast > SMA_fast;
def sellAvgs = EMA_fast < SMA_fast;

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
def RSIrawEMA = ExpAverage(RSI(), TimeFrame * 8) - 50;
def RSIfastEMA = ExpAverage(RSI, TimeFrame * 5);

#==================================================================
#==========================A0 Triggers 2===========================
#==================================================================


#--------------------------------
#------------RSI Math------------
#--------------------------------

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
plot Speed = ExpAverage(rsiValueDiff, 13) * 10;
def RSIema55 = (SimpleMovingAvg(_RSI, 55) - 50) * 1.5;


#------------------------
#--- dydx: The difference of 2 EXP Averages over a short distance (A)
#--- and the difference of 2 EXP Averages over a longer distance (B)
#--- and the difference between A & B, divided by a Medium distance.
#--- WHY ---:  Its meant to emulate how derivatives are calculated to provide instantaneous,
#--- change over time...ideally more responsive than "average".
#------------------------
def _RSIValueFaster = ((ExpAverage(_RSI, 8) * 1.7 - ExpAverage(RSI(), 13) * 1.7));
def _RSIValueSlower = ((ExpAverage(_RSI, 8) * 1.7 - ExpAverage(RSI(), 34) * 1.7));
def dydxRSI = ((_RSIValueSlower - _RSIValueFaster) / 21) * 50;



#==================================================================
#====================== Cycle Detection ===========================
#==================================================================
input triggerSell = 9;
plot tooFastSell = triggerSell;

input triggerBuy = -9;
plot tooFastBuy = triggerBuy;

def currentDiff = RSIrawEMA - RSIfastEMA;
def length = 100;
def zero = 0;
def diff = RSIrawEMA - RSIfastEMA;
def SellDiff = (
    diff[0] < diff[1]
    && diff[1] < diff[2]
    #&& diff[2] < diff[3]
);
def BuyDiff = (
    diff[0] > diff[1]
    && diff[1] > diff[2]
    #&& diff[2] > diff[3]
);
def sellBasket = fold i = 0 to length with currentSell = Double.NEGATIVE_INFINITY while currentDiff[i] < 0 do Max(currentSell, currentDiff[i]);
def buyBasket = fold j = 0 to length with currentBuy = Double.POSITIVE_INFINITY while currentDiff[j] > 0 do Min(currentBuy, currentDiff[j]);

plot BuyZone = (
 buyBasket != Double.POSITIVE_INFINITY
  && buyAvgs
  && (BuyDiff or (RSIrawEMA >= 0))
  #&& RSIfastEMA > 0
);
plot BuyTrigger = (
    Speed <= tooFastBuy
    && (
      Speed < Speed[1] or Speed[1] < Speed[2]
    )
);
plot SellZone = (
 sellBasket != Double.NEGATIVE_INFINITY
  && sellAvgs
  && (SellDiff or (RSIrawEMA <= 0))
  #&& RSIfastEMA < 0
);
plot SellTrigger = (
    Speed >= tooFastSell
    && (
      Speed > Speed[1] or Speed[1] > Speed[2]
    )
);

plot StrongBUY = BuyZone && BuyTrigger;
plot StrongSELL = SellZone && SellTrigger;

StrongBUY.SetDefaultColor(color.GREEN);
StrongSELL.SetDefaultColor(Color.RED);
