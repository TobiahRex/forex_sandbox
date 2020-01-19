import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';

import actions from '../modules/actions';

export default connect(
  ({ technicalData }) => ({ technicalData }),
  dispatch => bindActionCreators(actions, dispatch),
);
