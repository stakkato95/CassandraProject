//
// Created by Artsiom Kaliaha on 4/28/20.
//

#include "SensorAdapter.h"

using namespace std;

Sensor SensorAdapter::getModel(const ContentValues &value) {
    return {
            get<int>(value.at("companyId")),
            get<int>(value.at("droneId")),
            get<float>(value.at("latitude")),
            get<float>(value.at("longitude")),
            get<float>(value.at("elevation"))
    };
}

ContentValues SensorAdapter::getContentValues(const Sensor &model) {
    return {
            {"companyId", model.companyId},
            {"droneId",   model.droneId},
            {"latitude",  model.latitude},
            {"longitude", model.longitude},
            {"elevation", model.elevation},
    };
}

ContentMappings SensorAdapter::getAttrs() {
    return {
            {"companyId", type_index(typeid(string))},
            {"droneId",   type_index(typeid(int))},
            {"latitude",  type_index(typeid(float))},
            {"longitude", type_index(typeid(float))},
            {"elevation", type_index(typeid(float))}
    };
}