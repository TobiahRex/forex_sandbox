export function composeQueryParams(req) {
  const queryData = {
    logResults: false,
    returnResults: false,
    writeToFiles: false,
  };
  if (req.query.log === 'true') queryData.logResults = true;
  if (req.query.return === 'true') queryData.logResults = true;
  if (req.query.write === 'true') queryData.writeToFiles = true;

  return queryData;
}

export default {
  generate: composeQueryParams,
};
