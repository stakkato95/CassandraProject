//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef APPSERVER_CASSDRIVERADAPTER_H
#define APPSERVER_CASSDRIVERADAPTER_H

#include "CassDriverWrapper.h"
#include "../Adapter/Adapter.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <variant>

class CassDriverAdapter {
public:
    explicit CassDriverAdapter(CassDriverWrapper d);

    ~CassDriverAdapter();

    template<typename TModel, typename TAdapter>
    void registerAdapter(const std::string &table) {
        adapters[typeid(TModel)] = new TAdapter;
        tables[typeid(TModel)] = table;
    }

    template<typename TModel, typename TAdapter>
    std::variant<DriverError, std::vector<TModel>> select(const ContentValues &where);

    template<typename TModel>
    std::optional<DriverError> insert(const TModel &model);

    template<typename TModel>
    std::optional<DriverError> update(const ContentValues &where, const ContentValues &values);

    template<typename TModel>
    std::optional<DriverError> del(const ContentValues &where);

private:
    CassDriverWrapper driver;
    std::unordered_map<std::type_index, IAdapter*> adapters;
    std::unordered_map<std::type_index, std::string> tables;
};


#endif //APPSERVER_CASSDRIVERADAPTER_H
