const queryDataSchema = {
  logResults: false,
  returnResults: false,
  writeToFiles: false,
};

export function composeQueryParams(req) {
  const queryData = { ...queryDataSchema };

  if (req.query.log === 'true') queryData.logResults = true;
  if (req.query.return === 'true') queryData.returnResults = true;
  if (req.query.write === 'true') queryData.writeToFiles = true;

  return queryData;
}

export default {
  generate: composeQueryParams,
};
