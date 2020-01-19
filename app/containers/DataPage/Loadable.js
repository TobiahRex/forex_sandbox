/**
 * Asynchronously loads the component for FeaturePage
 */

import React from 'react';
import loadable from 'utils/loadable';
// import LoadingIndicator from 'components/LoadingIndicator';
import Loading from 'components/Loading';

export default loadable(() => import('./index'), {
  fallback: <Loading />,
});
