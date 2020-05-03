//
// Created by Artsiom Kaliaha on 5/3/20.
//

#ifndef CASSANDRAPROJECT_FLIGHTMAPPER_H
#define CASSANDRAPROJECT_FLIGHTMAPPER_H

#include "Mapper.h"
#include "databaseapp.grpc.pb.h"
#include "../Flight.h"

class FlightMapper : public Mapper<databaseapp::FlightResponse, Flight> {
public:
    Flight getModel(const databaseapp::FlightResponse &response) const override;
};


#endif //CASSANDRAPROJECT_FLIGHTMAPPER_H
