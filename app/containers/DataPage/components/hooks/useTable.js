import { useMemo } from 'react';
import moment from 'moment';

// 'dateString', 'time', 'open', 'high', 'low', 'close'

const allColumns = {
  priceData: [
    {
      title: 'Date',
      dataIndex: 'dateString',
      key: 'dateString',
    },
    {
      title: 'Time',
      dataIndex: 'time',
      key: 'time',
    },
    {
      title: 'Open',
      dataIndex: 'open',
      key: 'open',
    },
    {
      title: 'High',
      dataIndex: 'high',
      key: 'high',
    },
    {
      title: 'Low',
      dataIndex: 'low',
      key: 'low',
    },
    {
      title: 'Close',
      dataIndex: 'close',
      key: 'close',
    },
  ],
  indicatorData: [
    {
      title: 'EMA',
      dataIndex: 'average',
      key: 'average',
    },
    {
      title: 'EMA Fast',
      dataIndex: 'averageSmall',
      key: 'averageSmall',
    },
    {
      title: 'EMA Slow',
      dataIndex: 'averageBig',
      key: 'averageBig',
    },
    {
      title: 'Zig Zag Raw',
      dataIndex: 'zigZag',
      key: 'zigZag',
    },
    {
      title: 'Zig Zag Duration',
      dataIndex: 'zzDurations',
      key: 'zzDurations',
    },
    {
      title: 'Zig Zag Magnitude',
      dataIndex: 'zzMagnitudes',
      key: 'zzMagnitudes',
    },
  ],
};

const seedData = [
  {
    open: 100,
    high: 100,
    low: 1,
    close: 50,
    dateString: new Date(),
    time: Date.now(),
  },
];

export default function useTable(tableType = 'priceData', rawData = seedData) {
  const columns = useMemo(() => allColumns[tableType], [tableType]);

  const dataSource = useMemo(() => {
    switch (tableType) {
      case 'priceData':
        return rawData.map(({ open, high, low, close, dateString, time }) => ({
          open,
          high,
          low,
          close,
          time: moment(time).format('hh:mm:ss'),
          dateString: `${moment(dateString).format('YYYY-MM-DD')}`,
        }));
      default:
        return null;
    }
  }, [rawData]);
  return { columns, dataSource };
}
