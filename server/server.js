/* eslint-disable import/first, no-console, operator-linebreak */

console.clear();
import express from 'express';
import http from 'http';
import path from 'path';
import morgan from 'morgan';
import bodyParser from 'body-parser';
import mongoose from 'mongoose';
import dotenv from 'dotenv';
import api from './api';

// ---------------------------- CONFIG -----------------------------------------
mongoose.Promise = Promise;
dotenv.config({ silent: true });
const PORT = process.env.PORT || 3000;
const MONGO = process.env.MONGODB_URI || 'mongodb://localhost/market-catch';
const BUILD = process.env.NODE_ENV || 'development';
process.stdout.write('NODE_ENV === ', BUILD);
const app = express();
const server = new http.Server(app);

// ---------------------- Express Middleware -----------------------------------
app.use(morgan('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static('public'));
app.use((req, res, next) => {
  const resRef = res;
  resRef.handle = (err, data) => {
    if (err) {
      process.stdout.write(`Response Error: 😕
${JSON.stringify(err)}
`);
    } else {
      //       process.stdout.write(`Response Data: 😎
      // ${JSON.stringify(data)}
      // `);
    }
    res.status(err ? 400 : 200).send(err || data);
  };
  next();
});

app.use('/api', api);
app.get('*', (req, res) => res.send('yo'));

// --------------------------- Listeners ---------------------------------------
server.listen(PORT, err =>
  process.stdout.write(
    err ||
      `
    ==> 📡  Server @ ${PORT}
    `,
  ),
);

mongoose.connect(MONGO, err =>
  process.stdout.write(
    err ||
      `
    ==> 📜  MONGO @ ${MONGO}
    `,
  ),
);
