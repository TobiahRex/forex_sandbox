/**
 * Asynchronously loads the component for FeaturePage
 */

import React from 'react';

import loadable from 'utils/loadable';
import Loading from 'components/Loading';
import Container from './Container';

const View = loadable(() => import('./View'), {
  fallback: <Loading />,
});

export default Container(View);
