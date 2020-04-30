//
// Created by Artsiom Kaliaha on 4/30/20.
//

#include "DroneMapper.h"

Drone DroneMapper::getModel(const databaseapp::DroneResponse &response) const {
    return {
            response.companyid(),
            response.companyname(),
            response.droneid(),
            response.model(),
            response.firmwareversion()
    };
}