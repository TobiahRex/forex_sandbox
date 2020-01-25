import { all, call, put, takeLatest } from 'redux-saga/effects';

import request from 'utils/request';
import handleSagaError from 'utils/handleSagaError';
import * as types from './types';
import {
  async,
  receivedPriceData,
  receivedDirectionData,
  receivedTriggerData,
} from './actions';
// import { reposLoaded, repoLoadingError } from 'containers/App/actions';

export default function* githubData() {
  yield takeLatest(types.DATA_GET_SUITE, getDataSuite);
}

export function* getDataSuite(action) {
  yield all([
    call(fetchPriceData, action),
    call(fetchDirectionData, action),
    call(fetchTriggerData, action),
  ]);
}

export function* fetchPriceData(action) {
  const { payload } = action;
  const requestURL = `/data/generate/prices?log=${payload.log}&return=${payload.return}&write=${payload.write}`; // eslint-disable-line

  try {
    const [data] = yield all([call(request, requestURL), put(async.request())]);
    yield put(receivedPriceData(data));
  } catch (err) {
    yield call(handleSagaError, err, async.error);
  }
}

function* fetchDirectionData(action) {
  const { payload } = action;
  const requestURL = `/data/generate/direction?log=${payload.log}&return=${payload.return}&write=${payload.write}`; // eslint-disable-line
  try {
    const [data] = yield all([call(request, requestURL), put(async.request())]);
    yield put(receivedDirectionData(data));
  } catch (err) {
    yield call(handleSagaError, err, async.error);
  }
}

function* fetchTriggerData(action) {
  const { payload } = action;
  const requestURL = `/data/generate/trigger?log=${payload.log}&return=${payload.return}&write=${payload.write}`; // eslint-disable-line
  try {
    const [data] = yield all([call(request, requestURL), put(async.request())]);
    yield put(receivedTriggerData(data));
  } catch (err) {
    yield call(handleSagaError, err, async.error);
  }
}
