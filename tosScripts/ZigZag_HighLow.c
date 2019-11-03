#
# TD Ameritrade IP Company, Inc. (c) 2013-2019
#

input priceH = high;
input priceL = low;
input percentageReversal = 5.0;
input absoluteReversal = 0.0;
input atrLength = 5;
input atrReversal = 1.5;
input tickReversal = 0;

// Assert(percentageReversal >= 0, "'percentage reversal' must not be negative: " + percentageReversal);
// Assert(absoluteReversal >= 0, "'absolute reversal' must not be negative: " + absoluteReversal);
// Assert(atrReversal >= 0, "'atr reversal' must not be negative: " + atrReversal);
// Assert(tickReversal >= 0, "'ticks' must not be negative: " + tickReversal);
// Assert(percentageReversal != 0 or absoluteReversal != 0 or atrReversal != 0 or tickReversal != 0, "Either 'percentage reversal' or 'absolute reversal' or 'atr reversal' or 'tick reversal' must not be zero");

def absReversal;
if (absoluteReversal != 0) {
    absReversal = absoluteReversal;
} else {
    absReversal =  tickReversal * TickSize();
}

def hlPivot;
if (atrReversal != 0) {
    hlPivot = percentageReversal / 100 + WildersAverage(TrueRange(high, close, low), atrLength) / close * atrReversal;
} else {
    hlPivot = percentageReversal / 100;
}
def state = {
  default init,
  undefined,
  uptrend,
  downtrend
};
def maxPriceH;
def minPriceL;
def newMax;
def newMin;
def prevMaxH = GetValue(maxPriceH, 1);
def prevMinL = GetValue(minPriceL, 1);

if GetValue(state, 1) == GetValue(state.init, 0) {
    maxPriceH = priceH;
    minPriceL = priceL;
    newMax = yes;
    newMin = yes;
    state = state.undefined;
} else if GetValue(state, 1) == GetValue(state.undefined, 0) {
    if priceH >= prevMaxH {
        state = state.uptrend;
        maxPriceH = priceH;
        minPriceL = prevMinL;
        newMax = yes;
        newMin = no;
    } else if priceL <= prevMinL {
        state = state.downtrend;
        maxPriceH = prevMaxH;
        minPriceL = priceL;
        newMax = no;
        newMin = yes;
    } else {
        state = state.undefined;
        maxPriceH = prevMaxH;
        minPriceL = prevMinL;
        newMax = no;
        newMin = no;
    }
} else if GetValue(state, 1) == GetValue(state.uptrend, 0) {
    if priceL <= prevMaxH - prevMaxH * hlPivot - absReversal {
        state = state.downtrend;
        maxPriceH = prevMaxH;
        minPriceL = priceL;
        newMax = no;
        newMin = yes;
    } else {
        state = state.uptrend;
        if (priceH >= prevMaxH) {
            maxPriceH = priceH;
            newMax = yes;
        } else {
            maxPriceH = prevMaxH;
            newMax = no;
        }
        minPriceL = prevMinL;
        newMin = no;
    }
} else {
    if priceH >= prevMinL + prevMinL * hlPivot + absReversal {
        state = state.uptrend;
        maxPriceH = priceH;
        minPriceL = prevMinL;
        newMax = yes;
        newMin = no;
    } else {
        state = state.downtrend;
        maxPriceH = prevMaxH;
        newMax = no;
        if (priceL <= prevMinL) {
            minPriceL = priceL;
            newMin = yes;
        } else {
            minPriceL = prevMinL;
            newMin = no;
        }
    }
}

def barNumber = BarNumber();
def barCount = HighestAll(If(IsNaN(priceH), 0, barNumber));
def newState = GetValue(state, 0) != GetValue(state, 1);
def offset = barCount - barNumber + 1;
def highPoint = state == state.uptrend and priceH == maxPriceH;
def lowPoint = state == state.downtrend and priceL == minPriceL;

def lastH;
if highPoint and offset > 1 {
    lastH = (
      fold iH = 1 to offset
      with tH = priceH
      while !IsNaN(tH) and !GetValue(newState, -iH)
      do (
        if (GetValue(newMax, -iH) or iH == offset - 1 and GetValue(priceH, -iH) == tH)
          then Double.NaN
        else tH;
      )
    )
} else {
  lastH = Double.NaN;
}

def lastL;
if lowPoint and offset > 1 {
    lastL = (
      fold iL = 1 to offset
      with tL = priceL
      while !IsNaN(tL) and !GetValue(newState, -iL)
      do (
        if GetValue(newMin, -iL) or iL == offset - 1 and GetValue(priceL, -iL) == tL
          then Double.NaN
        else tL;
      )
} else {
    lastL = Double.NaN;
}

plot ZZ;
if barNumber == 1 {
    ZZ = (
      fold iF = 1 to offset
      with tP = Double.NaN
      while IsNaN(tP)
      do (
        if GetValue(state, -iF) == GetValue(state.uptrend, 0)
          then priceL
        else if GetValue(state, -iF) == GetValue(state.downtrend, 0)
          then priceH
        else Double.NaN;
      )
} else if barNumber == barCount {
    ZZ = if highPoint or state == state.downtrend and priceL > minPriceL
      then priceH
    else if lowPoint or state == state.uptrend and priceH < maxPriceH
      then priceL
    else Double.NaN;
} else {
    ZZ = if !IsNaN(lastH)
      then lastH
    else if !IsNaN(lastL)
      then lastL
    else Double.NaN;
}
ZZ.SetDefaultColor(GetColor(1));
ZZ.EnableApproximation();
