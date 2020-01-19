import React from 'react';
import { Spin, Icon } from 'antd';

export default function Loading() {
  const SpinIcon = <Icon style={{ fontSize: 24 }} type="loading" spin />;
  return <Spin tip="Loading..." indicator={SpinIcon} />;
}
