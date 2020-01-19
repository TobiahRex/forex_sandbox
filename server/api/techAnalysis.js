import express from 'express';

import * as TechAnalysis from '../models/techAnalysis';

const router = new express.Router();

router.get('/generate/all', (req, res) => {
  const queryData = {
    logResults: false,
    returnResults: false,
    writeToFiles: false,
  };
  if (req.query.log === 'true') queryData.logResults = true;
  if (req.query.return === 'true') queryData.logResults = true;
  if (req.query.write === 'true') queryData.writeToFiles = true;

  TechAnalysis.buildSignals({ query: queryData }, (err, data) => {
    if (queryData.logResults) console.log('\n', data.trigZigZag.metrics[400].meta);
    if (queryData.returnResults) {
      res.handle(err, data);
    } else {
      res.handle(err);
    }
  });
});

export default router;
