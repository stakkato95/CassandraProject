//
// Created by Artsiom Kaliaha on 4/30/20.
//

#ifndef WEBSERVER_DRONEMAPPER_H
#define WEBSERVER_DRONEMAPPER_H

#include "Mapper.h"
#include "databaseapp.grpc.pb.h"
#include "../Drone.h"

class DroneMapper : public Mapper<databaseapp::DroneResponse, Drone> {
public:
    Drone getModel(const databaseapp::DroneResponse &response) const override;
};


#endif //WEBSERVER_DRONEMAPPER_H
