# Cointosis

## What

This project is meant as a way to analyze market conditions using probability.
I'm currently an active forex day-trader as well as a full-time software engineer
at a Fintech startup. This project uses some of the Data Science principles i've
learned at this job, and marries them with my own personal learned technical
analysis toolsets, to remove (as much as possible) the subjective parts of
trading the market, and replace them with objective data points. This is not
a project to build a black-box, or a fully-automated trading system. Instead,
it's a project, to create a hybridized trading system, that assists a manual
trader, to make objective choices on when to enter/exit a trade.

## Why

This project exists as an attempt to remove subjective processes from entering a trade
in the market, and replace it with an objective process based on probability.

- Trading without emotion is extremely hard.
- Often a trader is tormented with hindsight: asking themself "should I have made that trade?" if you loose money, one will always be tempted to find reasons afterward on why they shouldn't have made
  the trade.
- However, what if you **did make the right decision** on opening the trade?
  but now you're questioning a good analysis as a potential bad analysis? If you lost money despite
  a good analysis, its hard to win the argument that you shouldn't have done something different.
- So, how does one objectively, solve these dilemmas, and stop yourself from self sabotaging a good
  analysis in the future and preserve your confidence in your system/approach?
- **The answer:** _objectifying the reasons you enter a trade. Remove all subjective logic from the
  process, by relying only on probability of your entry yielding a profitable historical result._

## How

We will objectify Entries to probability questions, using Beysian probability
principles and data science techniques in the realm of classification algorithms.

- To accomplish this we will first rely on 2 custom indicators.
- They are derivatives of RSI, EMA's & ZigZag patterns.
- We will turn a continuous data stream of RSI and EMA signals, into a discrete
  data set, that can be quantified using simple geometric principles like,
  - _slope_
  - _magnitude_
  - _hypotenuse length_
  - _rise_
  - _run_

as metrics to count hueristic instances of certain conditions. Given those counts,
we can easily determine the probability of them occuring in the future using
data science classification ideas in tandem with Beysian probability theory.
