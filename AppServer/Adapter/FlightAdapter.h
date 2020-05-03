//
// Created by Artsiom Kaliaha on 5/3/20.
//

#ifndef APPSERVER_FLIGHTADAPTER_H
#define APPSERVER_FLIGHTADAPTER_H

#include <cstdint>

#include "Adapter.h"
#include "../../Model/Flight.h"

class FlightAdapter : public Adapter<Flight> {
public:
    Flight getModel(const ContentValues &value) override;

    ContentValues getContentValues(const Flight &model) override;

    ContentMappings getAttrs() override;
};


#endif //APPSERVER_FLIGHTADAPTER_H
