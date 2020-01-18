import express from 'express';

import TechAnalysis from './techAnalysis';

const router = new express.Router();

router.use('/data', TechAnalysis);

export default router;
