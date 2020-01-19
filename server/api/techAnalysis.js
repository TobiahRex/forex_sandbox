import express from 'express';

import * as TechAnalysis from '../models/techAnalysis';

const router = new express.Router();

router.get('/generate/all', (req, res) => {
  TechAnalysis.buildSignals((err, data) => {
    
    res.handle(err);
  });
});

export default router;
