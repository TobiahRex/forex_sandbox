import React, { useCallback, useEffect } from 'react';
import { PropTypes } from 'prop-types';
import { Helmet } from 'react-helmet';
import { FormattedMessage } from 'react-intl';
import { Table, Tabs, Collapse } from 'antd';

import { useInjectReducer } from 'utils/injectReducer';
import { useInjectSaga } from 'utils/injectSaga';
import H1 from 'components/H1';
import reducer from '../modules/reducer';
import saga from '../modules/saga';
import messages from './messages';
import DivCenter from './DivCenter';
// import DivText from './DivText';
import Ptitle from './Ptitle';
import { useTable } from './hooks';

const { TabPane } = Tabs;
const { Panel } = Collapse;
const key = 'data';

export default function DataPage(props) {
  useInjectReducer({ key, reducer });
  useInjectSaga({ key, saga });

  const onTabChange = useCallback(tabIndex => {
    console.log('%ctabIndex', 'background:blue;color:white;', tabIndex);
  }, []);
  const onPanelChange = useCallback(panelIndex => {
    console.log('%ctabIndex', 'background:blue;color:white;', panelIndex);
  }, []);

  useEffect(() => {
    props.handlers.getDataSuite({
      log: false,
      write: false,
      return: true,
    });
  }, []);

  const tablePrices = useTable('priceData');
  // const tableTrigger = useTable('indicatorData');
  // const tableDirection = useTable('indicatorData');

  return (
    <div>
      <Helmet>
        <title>Data Page</title>
        <meta
          name="Renders API data"
          content="Data page of Cointosis application"
        />
      </Helmet>
      <H1>
        <FormattedMessage {...messages.header} />
      </H1>

      <Tabs defaultActiveKey="1" onChange={onTabChange}>
        <TabPane tab="Prices" key="1">
          <DivCenter>
            {/*
              TODO create a price data table

              data keys are:
              'dateString', 'time', 'open', 'high', 'low', 'close'
            */}
            <Table
              columns={tablePrices.columns}
              dataSource={tablePrices.dataSource}
            />
          </DivCenter>
        </TabPane>
        <TabPane tab="Trigger" key="2">
          <DivCenter>
            <Collapse defaultActiveKey={['1']} onChange={onPanelChange}>
              <Panel header="EMA Average" key="2">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigAverage} />
                </Ptitle>
              </Panel>
              <Panel header="EMA Average Fast" key="3">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigAverageSmall} />
                </Ptitle>
              </Panel>
              <Panel header="EMA Average Slow" key="4">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigAverageBig} />
                </Ptitle>
              </Panel>
              <Panel header="Zig Zag Raw" key="5">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigZigZag} />
                </Ptitle>
              </Panel>
              <Panel header="Zig Zag Duration" key="6">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigZZDurations} />
                </Ptitle>
              </Panel>
              <Panel header="Zig Zag Magnitude" key="7">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigZZMagnitudes} />
                </Ptitle>
              </Panel>
            </Collapse>
          </DivCenter>
        </TabPane>
        <TabPane tab="Direction" key="3">
          <DivCenter>
            <Collapse defaultActiveKey={['1']} onChange={onPanelChange}>
              <Panel header="EMA Average" key="2">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirAverage} />
                </Ptitle>
              </Panel>
              <Panel header="EMA Average Fast" key="3">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirAverageSmall} />
                </Ptitle>
              </Panel>
              <Panel header="EMA Average Slow" key="4">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirAverageBig} />
                </Ptitle>
              </Panel>
              <Panel header="Zig Zag Raw" key="5">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirZigZag} />
                </Ptitle>
              </Panel>
              <Panel header="Zig Zag Duration" key="6">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirZZDurations} />
                </Ptitle>
              </Panel>
              <Panel header="Zig Zag Magnitude" key="7">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirZZMagnitudes} />
                </Ptitle>
              </Panel>
            </Collapse>
          </DivCenter>
        </TabPane>
      </Tabs>
    </div>
  );
}
const { func, shape } = PropTypes;

DataPage.propTypes = {
  handlers: shape({
    getDataSuite: func.isRequired,
  }),
};
