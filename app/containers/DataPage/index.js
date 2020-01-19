/* eslint-disable */
/*
 * FeaturePage
 *
 * List all the features
 */
import React from 'react';
import { Helmet } from 'react-helmet';
import { FormattedMessage } from 'react-intl';
import { Table, Tabs, Collapse } from 'antd';

import H1 from 'components/H1';
import messages from './messages';
import DivCenter from './DivCenter';
import DivText from './DivText';
import Ptitle from './Ptitle';
import useTable from './hooks';

const { TabPane } = Tabs;
const { Panel } = Collapse;

export default function FeaturePage() {
  const onTabChange = React.useCallback(tabIndex => {
    console.log('%ctabIndex', 'background:blue;color:white;', tabIndex);
  }, []);
  const onPanelChange = React.useCallback(panelIndex => {
    console.log('%ctabIndex', 'background:blue;color:white;', panelIndex);
  }, []);

  const tablePrices = useTable('priceData');

  return (
    <div>
      <Helmet>
        <title>Data Page</title>
        <meta
          name="Renders API data"
          content="Data page of Market Catch application"
        />
      </Helmet>
      <H1>
        <FormattedMessage {...messages.header} />
      </H1>

      <Tabs defaultActiveKey="1" onChange={onTabChange}>
        <TabPane tab="Prices" key="1">
          <DivCenter>
            <DivText>
              <FormattedMessage {...messages.headers.priceData} />
            </DivText>
            <Table />
            {/*
              TODO create a price data table

              data keys are:
              'dateString', 'time', 'open', 'high', 'low', 'close'
            */}
            <Table columns={tablePrices.columns} dataSource={tablePrices.dataSource} />
          </DivCenter>
        </TabPane>

        <TabPane tab="Trigger" key="2">
          <DivCenter>
            <DivText>
              <FormattedMessage {...messages.headers.trigger} />
            </DivText>
            <Collapse defaultActiveKey={['1']} onChange={onPanelChange}>
              <Panel header="EMA Average" key="2">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigAverage} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="EMA Average Fast" key="3">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigAverageSmall} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="EMA Average Slow" key="4">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigAverageBig} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="Zig Zag Raw" key="5">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigZigZag} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="Zig Zag Duration" key="6">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigZZDurations} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="Zig Zag Magnitude" key="7">
                <Ptitle>
                  <FormattedMessage {...messages.headers.trigZZMagnitudes} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
            </Collapse>
          </DivCenter>
        </TabPane>

        <TabPane tab="Direction" key="2">
          <DivCenter>
            <DivText>
              <FormattedMessage {...messages.headers.direction} />
            </DivText>
            <Collapse defaultActiveKey={['1']} onChange={onPanelChange}>
              <Panel header="EMA Average" key="2">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirAverage} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="EMA Average Fast" key="3">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirAverageSmall} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="EMA Average Slow" key="4">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirAverageBig} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="Zig Zag Raw" key="5">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirZigZag} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="Zig Zag Duration" key="6">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirZZDurations} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
              <Panel header="Zig Zag Magnitude" key="7">
                <Ptitle>
                  <FormattedMessage {...messages.headers.dirZZMagnitudes} />
                </Ptitle>
                {/* TODO create a price data table */}
              </Panel>
            </Collapse>
          </DivCenter>
        </TabPane>
      </Tabs>
    </div>
  );
}
