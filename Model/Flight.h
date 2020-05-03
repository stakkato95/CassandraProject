//
// Created by Artsiom Kaliaha on 5/3/20.
//

#ifndef CASSANDRAPROJECT_FLIGHT_H
#define CASSANDRAPROJECT_FLIGHT_H

#include <cstdint>

struct Flight {
    int companyId;
    int droneId;
    int flightId;
    float latitude;
    float longitude;
    float elevation;
    int yearMonthDay;
    int64_t hourMinuteSecond;
};

#endif //CASSANDRAPROJECT_FLIGHT_H
