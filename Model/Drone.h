//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef CASSANDRAPROJECT_DRONE_H
#define CASSANDRAPROJECT_DRONE_H

#include <string>

struct Drone {
    int companyId;
    std::string companyName;
    int droneId;
    std::string model;
    std::string firmwareVersion;
};

#endif //CASSANDRAPROJECT_DRONE_H
