/*
 * HomePage Messages
 *
 * This contains all the text for the HomePage component.
 */
import { defineMessages } from 'react-intl';

export const scope = 'boilerplate.components.Header';

export default defineMessages({
  home: {
    id: `${scope}.home`,
    defaultMessage: 'Home',
  },
  data: {
    id: `${scope}.data`,
    defaultMessage: 'Data',
  },
  charts: {
    id: `${scope}.charts`,
    defaultMessage: 'Charts',
  },
});
