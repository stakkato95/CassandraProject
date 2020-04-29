//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef APPSERVER_DRONEADAPTER_H
#define APPSERVER_DRONEADAPTER_H

#include "Adapter.h"
#include "../../Model/Drone.h"

#include <string>

class DroneAdapter : public Adapter<Drone> {
public:
    Drone getModel(const ContentValues &value) override;

    ContentValues getContentValues(const Drone &model) override;

    ContentMappings getAttrs() override;
};


#endif //APPSERVER_DRONEADAPTER_H
