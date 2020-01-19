/* eslint consistent-return:0 import/order:0 */

import express from 'express';
import logger from './logger';
import bodyParser from 'body-parser';
import morgan from 'morgan';

import argv from './argv';
import port from './port';
import setup from './middlewares/frontendMiddleware';
import api from './api';

const isDev = process.env.NODE_ENV !== 'production';

const ngrok = (isDev && process.env.ENABLE_TUNNEL) || argv.tunnel ? require('ngrok') : false;
const { resolve } = require('path');
const app = express();

app.use(morgan('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static('public'));
app.use((req, res, next) => {
  res.handle = (err, data) => {
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

// In production we need to pass these values in instead of relying on webpack
setup(app, {
  outputPath: resolve(process.cwd(), 'build'),
  publicPath: '/',
});

// get the intended host and port number, use localhost and port 3000 if not provided
const customHost = argv.host || process.env.HOST;
const host = customHost || null; // Let http.Server use its default IPv6/4 host
const prettyHost = customHost || 'localhost';

// use the gzipped bundle
app.get('*.js', (req, res, next) => {
  req.url = req.url + '.gz'; // eslint-disable-line
  res.set('Content-Encoding', 'gzip');
  next();
});

// Start your app.
app.listen(port, host, async err => {
  if (err) {
    return logger.error(err.message);
  }

  // Connect to ngrok in dev mode
  if (ngrok) {
    let url;
    try {
      url = await ngrok.connect(port);
    } catch (e) {
      return logger.error(e);
    }
    logger.appStarted(port, prettyHost, url);
  } else {
    logger.appStarted(port, prettyHost);
  }
});
