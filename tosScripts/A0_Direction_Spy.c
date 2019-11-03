/*
def <result> = fold <index> = <start> to <end> [ with <variable> [ = <init> ] ] [ while <condition> ] do <expression>;

*/
def length = 100;

const currentDiff = rsiYellow2 < rsiYellow;
def sellBasketSize = (
  fold i = 0 to length
  with counter = 0
    while currentDiff[i] < 0
      do (counter + 1)
);

def buyBasket = (
  fold j = 0 to length
  with currentBuy = Double.POSITIVE_INFINITY
    while currentDiff[j] > 0
      do Min(currentBuy, currentDiff[j]
  )
);

def sellDiff = _rsiYellow2 < _rsiYellow;
plot sellBasketSize = (
  fold i = 0 to length
  with counter = 0
    while sellDiff[i] < 0
      do (counter + 1)
);
