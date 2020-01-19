import { useMemo } from 'react';

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

export default function useTable(data, tableType = 'priceData') {
  const columns = useMemo(() => allColumns[tableType], [tableType]);

  return { columns, dataSource: data };
}
