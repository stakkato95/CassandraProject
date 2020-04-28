//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef APPSERVER_SENSORADAPTER_H
#define APPSERVER_SENSORADAPTER_H

#include "Adapter.h"
#include "../../Model/Sensor.h"

class SensorAdapter : public Adapter<Sensor> {
public:
    Sensor getModel(const ContentValues &value) override;

    ContentValues getContentValues(const Sensor &model) override;

    ContentMappings getAttrs() override;
};


#endif //APPSERVER_SENSORADAPTER_H
