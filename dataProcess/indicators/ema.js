class EMA {
  constructor({ values, period }) {
    this.values = values;
    this.period = period;
    this.smoother = 2 / (period + 1);
    this.previousEma = values.slice(0, period).reduce((a, b) => a + b) / period;

    return this.calculate();
  }

  /**
    Calculates the EMA for a given period.
    - First selects a length, but selecting all values except the first (period) values.
    - It creates it's first calculation by using the SMA(period) for it's previous EMA value.
    - The next subsequent value will be a non-SMA value.

    @param {None}
    @return {Array} result
  */
  calculate = () => {
    const results = new Array(this.period).fill(this.previousEma);
    this.values.slice(this.period).forEach(value => {
      const currentEma =
        (value - this.previousEma) * this.smoother + this.previousEma;
      results.push(currentEma);
      this.previousEma = currentEma;
    });

    return results;
  };
}

module.exports = EMA;
