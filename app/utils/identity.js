import { Map as IMap, List } from 'immutable';

const identity = {
  isFunction: data => typeof data === 'function',
  isZero: data => typeof data === 'number' && data === 0,
  isTruthyNumber: data => typeof data === 'number' && data > 0,
  isTruthyString: data => typeof data === 'string' && data.length > 0,
  isEmptyString: data => typeof data === 'string' && data.length === 0,
  isNotNull: data => {
    if (IMap.isMap(data)) return true;
    return typeof data === 'object' && data !== null;
  },
  isObjWithChildren: function(data) {
    if (IMap.isMap(data)) return data.size > 0;
    return this.isNotNull(data) && Object.keys(data).length > 0;
  },
  isNotArray: data => !List.isList(data) && !Array.isArray(data),
  isFullArray: function(data) {
    if (this.isNotArray(data)) return false;
    if (List.isList(data) && data.size > 0) return true;
    return Array.isArray(data) && data.length > 0;
  },
  isEmptyArray: function(data) {
    if (this.isNotArray(data)) return true;
    if (List.isList(data) && data.size === 0) return true;
    return Array.isArray(data) && data.length === 0;
  },
  isChild: data => {
    const child = data;
    return {
      ofArray: arr => {
        if (List.isList(arr)) return arr.includes(child);
        return Array.isArray(arr) && arr.includes(child);
      },
      ofObject: obj => {
        if (IMap.isMap(obj)) return obj.has(child);
        return Object.hasOwnProperty.call(obj, child);
      },
    };
  },
};

export default identity;
