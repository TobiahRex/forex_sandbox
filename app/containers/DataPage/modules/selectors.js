import { List } from 'immutable';
import { createSelector } from 'reselect';

export const selectPriceData = () =>
  createSelector(
    state => state.getIn(['data', 'prices'], List()),
    prices => prices,
  );
