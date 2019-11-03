declare upper;
#--------------------
#----Price Moving Averages
#--------------------
#----- EMAs
input fast = 8;
input medium = 21;
input slow = 55;
input slowest = 89;
input snail = 144;
input sloth = 233;

def EMA_fast = ExpAverage(close, fast);
def EMA_slow = ExpAverage(close, medium);
def SMA_fast = SimpleMovingAvg(close, slow);
def SMA_slow = SimpleMovingAVg(close, slowest);
def SMA_snail = SimpleMovingAVg(close, snail);
def SMA_sloth = SimpleMovingAVg(close, sloth);

AddCloud(EMA_fast, SMA_slow, Color.Dark_Green, Color.Dark_Red);
AddCloud(EMA_fast, SMA_fast, Color.Dark_Green, Color.Dark_Red);
AddCloud(EMA_fast, EMA_slow, Color.Cyan, Color.Magenta);
AddCloud(SMA_fast, SMA_snail, Color.Dark_Green, Color.Dark_Red);
AddCloud(EMA_fast, SMA_sloth, Color.Dark_Green, Color.Dark_Red);
