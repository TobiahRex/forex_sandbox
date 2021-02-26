## zigZag

* Zig Zag is a technical indicator that detects changes in value given a threshold of percentage. If a change in some time series values exceeds the given threshold, then a line is drawn from the moment that change exceeded the threshold to the next moment the change occurs again, but in the opposite direction. The effect, is that you have a set of time series data, that has a Zig Zag like line connecting swing highs to swing lows.

### generateZigZag
1. receives the raw time series data as __data__ && __times__. The __reversalAmount__ param is is the threshold.
2. Calls "seedTrendHelpers" for every data point.
  - For every new data point, the function returns a set of utility functions that are contextually updated to
  include the current data point's value, and the previous data point values.
3. 