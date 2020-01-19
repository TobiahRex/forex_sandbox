const { PROD_URL, NODE_ENV } = process.env;

const config = {
  globals: {
    'process.env': {
      NODE_ENV,
      BASE_URL: NODE_ENV === 'production' ? PROD_URL : 'http://localhost:3000/api',
    },
  },
};

export default config;
