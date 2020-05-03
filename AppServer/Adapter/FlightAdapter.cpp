//
// Created by Artsiom Kaliaha on 5/3/20.
//

#include "FlightAdapter.h"

using namespace std;

Flight FlightAdapter::getModel(const ContentValues &value) {
    return {
            get<int>(value.at("companyid")),
            get<int>(value.at("droneid")),
            get<int>(value.at("flightid")),
            get<float>(value.at("latitude")),
            get<float>(value.at("longitude")),
            get<float>(value.at("elevation")),
            get<uint32_t>(value.at("yearmonthday")),
            get<int64_t>(value.at("hourminutesecond"))
    };
}

ContentValues FlightAdapter::getContentValues(const Flight &model) {
    return {
            {"companyid",        model.companyId},
            {"droneid",          model.droneId},
            {"flightid",         model.droneId},
            {"latitude",         model.latitude},
            {"longitude",        model.longitude},
            {"elevation",        model.elevation},
            {"yearmonthday",     model.yearMonthDay},
            {"hourminutesecond", model.hourMinuteSecond}
    };
}

ContentMappings FlightAdapter::getAttrs() {
    return {
            {"companyid",        type_index(typeid(int))},
            {"droneid",          type_index(typeid(int))},
            {"flightid",         type_index(typeid(int))},
            {"latitude",         type_index(typeid(float))},
            {"longitude",        type_index(typeid(float))},
            {"elevation",        type_index(typeid(float))},
            {"yearmonthday",     type_index(typeid(uint32_t))},
            {"hourminutesecond", type_index(typeid(int64_t))}
    };
}
