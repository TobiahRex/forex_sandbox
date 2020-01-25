import { put } from 'redux-saga/effects';

export default function* handleSagaError(error, handler) {
  const { message = 'Unknown Error', response = { ok: false } } = error;
  yield put(handler({ error: !response.ok, errorMsg: message }));
}
