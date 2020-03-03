import express from 'express';

import * as TechAnalysis from '../models/techAnalysis';
import { composeQueryParams } from './utils';

const router = new express.Router();

router.get('/generate/all', (req, res) => {
  const queryData = composeQueryParams.generate(req);

  TechAnalysis.buildSignals({ query: queryData }, (err, data) => {
    if (data && queryData.logResults) console.log('\n', Object.keys(data));
    if (data && queryData.returnResults) {
      res.handle(err, data);
    } else {
      res.handle(err);
    }
  });
});

router.get('/generate/prices', (req, res) => {
  const queryData = composeQueryParams.generate(req);

  queryData.type = 'prices';
  TechAnalysis.buildSignals({ query: queryData }, (err, data) => {
    if (data && queryData.logResults) console.log('\n', Object.keys(data));
    if (data && queryData.returnResults) {
      res.handle(null, data);
    } else {
      res.handle(err);
    }
  });
});

export default router;
