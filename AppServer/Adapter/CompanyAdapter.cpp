//
// Created by Artsiom Kaliaha on 4/28/20.
//

#include "CompanyAdapter.h"

using namespace std;

Company CompanyAdapter::getModel(const ContentValues &value) {
    return {
            get<int>(value.at("id")),
            get<string>(value.at("name")),
            get<string>(value.at("address"))
    };
}

ContentValues CompanyAdapter::getContentValues(const Company &model) {
    return {
            {"id",      model.id},
            {"name",    model.name},
            {"address", model.address}
    };
}

ContentMappings CompanyAdapter::getAttrs() {
    return {
            {"id",      type_index(typeid(int))},
            {"name",    type_index(typeid(string))},
            {"address", type_index(typeid(string))}
    };
}