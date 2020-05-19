declare upper;
#==================================================================
#==========================Moving Averages=========================
#==================================================================
input fast = 8;
input medium = 21;
input slow = 55;
input slowest = 89;

def emaFast = ExpAverage(close, fast);
def emaSlow = ExpAverage(close, medium);
def smaFast = SimpleMovingAvg(close, slow);
def smaSlow = SimpleMovingAVg(close, slowest);

def buyAvgs = emaFast > smaFast;
def sellAvgs = emaFast < smaFast;

#==================================================================
#===================== Direction Calc =============================
#==================================================================

input direction_Time = 10;
input direction_rsiEma = 13;
def timeFrame = direction_Time;
def rsiEma = 13;
def timeFrameLength = timeFrame * 13;
def averageClose = (close + close[timeFrame]) / 2;
def avgNetChange = MovingAverage(AverageType.Exponential, averageClose - averageClose[1], timeFrameLength);
def avgTotalChange = MovingAverage(AverageType.Exponential, AbsValue(averageClose - averageClose[1]), timeFrameLength);
def dirChangeRatio = if avgTotalChange != 0 then (avgNetChange / avgTotalChange) else 0;
def _rsiDirection = 50 * (1 + dirChangeRatio) - 50;
def _rsiEmaDirection = ExpAverage(_rsiDirection, rsiEma);
def rsiValueEma = (
  MovingAverage(AverageType.EXPONENTIAL, _rsiDirection, 5 * timeFrame)
  - MovingAverage(AverageType.EXPONENTIAL, _rsiDirection, 55 * timeFrame)
);
def rsiValueEmaFast = MovingAverage(AverageType.EXPONENTIAL, rsiValueEma, 13 * timeFrame);
def rsiValueEmaSlow = MovingAverage(AverageType.EXPONENTIAL, rsiValueEma, 55 * timeFrame);
def rsiBear = ExpAverage(_rsiDirection, timeFrame * 5);
def zero = 0;
input d_ReversalAmount = 1.0;
def directionZZ = reference ZigZagHighLow(_rsiDirection, _rsiDirection, 0, d_ReversalAmount, 1, 0);
def directionZZSave = if !IsNaN(directionZZ) then _rsiDirection else GetValue(directionZZSave, 1);
def chg = _rsiDirection - GetValue(directionZZSave, 1);
def isUpDirection = chg >= 0;
def isConfDirection = AbsValue(chg) >= d_ReversalAmount or (IsNaN(GetValue(directionZZ, 1)) and GetValue(isConfDirection, 1));

#==================================================================
#===================== Triggers Calc ==============================
#==================================================================
  input triggerOS = -50;
  input triggerOB = 50;
  input triggerBuyLine = -30;
  input triggerSellLine = 30;
  input Sensitivity = 5;
  def netChange = MovingAverage(AverageType.EXPONENTIAL, close - close[1], 14);
  def totalChange = MovingAverage(AverageType.EXPONENTIAL, AbsValue(close - close[1]), 14);
  def changeRatio =
      if totalChange != 0
      then netChange / totalChange
      else 0;
  def _rsiTrigger = 50 * (changeRatio + 1) ;
  def _rsiEmaTrigger = ((ExpAverage(_rsiTrigger, 8) * 1.7) - (ExpAverage(RSI(), 34) * 1.7)) ;
  def OB = triggerOB;
  def OS = triggerOS;
  def Bullet = ExpAverage(_rsiEmaTrigger - _rsiEmaTrigger[1], Sensitivity) * 10;
  def SpeedEma = ExpAverage(Bullet, 8);
  def SpeedEma2 = ExpAverage(Bullet, 55) * 3;

  input t_ReversalAmount = 1.0;
  def triggerZZ = reference ZigZagHighLow(Bullet, Bullet, 0, t_ReversalAmount, 1, 0);
  def zzSave = if !IsNaN(triggerZZ) then Bullet else getValue(zzSave, 1);
  def chgTrigger = Bullet - getValue(zzSave, 1);
  def isUpTrigger = chgTrigger >= 0;
  def isConfTrigger = AbsValue(chgTrigger) >= t_ReversalAmount
      or (IsNaN(getValue(triggerZZ, 1)) and getValue(isConfTrigger, 1));

#==================================================================
#====================== Conditions Matrix Eval ====================
#==================================================================
def X = if _rsiEmaDirection >= rsiBear                            # Direction is Trending Up
            then if directionZZSave >= 0                            # Direction ZZ is BUY (B1-4)
                then
                    if zzSave >= GetValue(zzSave, 1)                  # Trigger is Rising
                        then if zzSave >= 0 then
                            1 else 0                    # Trigger above 0 (B1) else below 0 (B4)
                    else                                              # Trigger is Decreasing
                        if zzSave >= 0 then 0 else 0            # Trigger above 0 (B2) else below 0 (B3)
            else                                                    # Direction ZZ is SELL (S5-8)
                if zzSave >= GetValue(zzSave, 1)                      # Trigger is Rising
                        then if zzSave >= 0 then 0 else 0       # Trigger above 0 (S7) else below 0 (S6)
                    else                                              # Trigger is Decreasing
                        if zzSave >= 0 then 0 else 0            # Trigger above 0 (S8) else below 0 (S5)
        else                                                      # Direction is Trending Down
            if directionZZSave < 0                                  # Direction ZZ is SELL (S1-4)
                then
                    if zzSave >= GetValue(zzSave, 1)                  # Trigger is Rising
                        then if zzSave >= 0 then 0 else 0       # Trigger above 0 (S3) else below 0 (S2)
                    else                                              # Trigger is Decreasing
                        if zzSave >= 0 then 0 else 1            # Trigger above 0 (S4) else below 0 (S1)
            else                                                    # Direction ZZ is BUY (B5-8)
                if zzSave >= GetValue(zzSave, 1)                      # Trigger is Rising
                        then if zzSave >= 0 then 0 else 0       # Trigger above 0 (B5) else below 0 (B8)
                    else                                              # Trigger is Decreasing
                        if zzSave >= 0 then 0 else 0;            # Trigger above 0 (B6) else below 0 (B7)
Alert(
    X > 0,
    "B1 | S1",
    Alert.TICK,
    Sound.Ding
);