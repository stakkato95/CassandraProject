//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "DroneAdapter.h"

using namespace std;

Drone DroneAdapter::getModel(const ContentValues &value) {
    return {
            get<int>(value.at("companyid")),
            get<string>(value.at("companyname")),
            get<int>(value.at("droneid")),
            get<string>(value.at("firmwareversion")),
            get<string>(value.at("model"))
    };
}

ContentValues DroneAdapter::getContentValues(const Drone &model) {
    return {
            {"companyid",       model.companyId},
            {"companyname",     model.companyName},
            {"droneid",         model.droneId},
            {"firmwareversion", model.firmwareVersion},
            {"model",           model.model}
    };
}

ContentMappings DroneAdapter::getAttrs() {
    return {
            {"companyid",       type_index(typeid(int))},
            {"companyname",     type_index(typeid(string))},
            {"droneid",         type_index(typeid(int))},
            {"firmwareversion", type_index(typeid(string))},
            {"model",           type_index(typeid(string))}
    };
}
