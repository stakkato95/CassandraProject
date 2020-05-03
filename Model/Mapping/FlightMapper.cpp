//
// Created by Artsiom Kaliaha on 5/3/20.
//

#include "FlightMapper.h"

Flight FlightMapper::getModel(const databaseapp::FlightResponse &response) const {
    return {
            response.companyid(),
            response.droneid(),
            response.flightid(),
            response.latitude(),
            response.longitude(),
            response.elevation(),
            response.yearmonthday(),
            response.hourminutesecond()
    };
}