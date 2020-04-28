//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef APPSERVER_SENSOR_H
#define APPSERVER_SENSOR_H


struct Sensor {
    int companyId;
    int droneId;

    float latitude;
    float longitude;
    float elevation;

    //time_t timestamp;
    //bool isAutonomousMode;
    //string ipAddress;
};


#endif //APPSERVER_SENSOR_H
