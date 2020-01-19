import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';

import * as actions from '../modules/actions';

export default connect(
  ({ technicalData }) => ({ technicalData }),
  dispatch => ({
    handlers: bindActionCreators(actions, dispatch),
  }),
);
