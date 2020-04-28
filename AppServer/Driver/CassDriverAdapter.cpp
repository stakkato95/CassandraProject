//
// Created by Artsiom Kaliaha on 4/28/20.
//

#include "CassDriverAdapter.h"

using namespace std;

CassDriverAdapter::CassDriverAdapter(CassDriverWrapper d) : driver{d} {
    driver.connect();
}

CassDriverAdapter::~CassDriverAdapter() {
    driver.disconnect();
}

//template<typename TModel>
//variant<DriverError, vector<TModel>> CassDriverAdapter::select(const ContentValues &where) {
//    const type_info &modelType = typeid(TModel);
//    const Adapter<TModel> adapter = adapters.at(modelType);
//    const string &table = tables.at(modelType);
//
//    if (auto result = driver.select(table, where, adapter.getAttrs()); holds_alternative<ResultCollection>(result)) {
//        ResultCollection collection = get<ResultCollection>(result);
//
//        vector<TModel> resultVector;
//        for (const ContentValues &values : collection) {
//            TModel model = adapter->getModel(values);
//            resultVector.push_back(model);
//        }
//
//        return resultVector;
//    } else {
//        return get<DriverError>(result);
//    }
//}

template<typename TModel>
optional<DriverError> CassDriverAdapter::insert(const TModel &model) {
    const type_info &modelType = typeid(TModel);
    Adapter<TModel> adapter = adapters[modelType];
    string &table = tables[modelType];
    ContentValues cv = adapter.getContentValues(model);
    return driver.insert(table, cv);
}

template<typename TModel>
optional<DriverError> CassDriverAdapter::update(const ContentValues &where, const ContentValues &values) {
    const type_info &modelType = typeid(TModel);
    string &table = tables[modelType];
    return driver.update(table, where, values);
}

template<typename TModel>
optional<DriverError> CassDriverAdapter::del(const ContentValues &where) {
    const type_info &modelType = typeid(TModel);
    string &table = tables[modelType];
    return driver.del(table, where);
}