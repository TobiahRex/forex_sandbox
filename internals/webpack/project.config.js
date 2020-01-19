const { PROD_URL, NODE_ENV } = process.env;

const config = {
  globals: {
    'process.env': {
      NODE_ENV: JSON.stringify(NODE_ENV),
      BASE_URL:
        NODE_ENV === 'production'
          ? JSON.stringify(PROD_URL)
          : JSON.stringify('http://localhost:3000/api'),
    },
  },
};

export default config;
