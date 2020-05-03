//
// Created by Artsiom Kaliaha on 4/30/20.
//

#ifndef WEBSERVER_STORAGE_H
#define WEBSERVER_STORAGE_H

#include <vector>

#include "GrpcDriver.h"

#include "../../Model/Company.h"
#include "../../Model/Drone.h"
#include "../../Model/Flight.h"

struct CompanyResponse {
    Company company;
    std::vector<Drone> drones;
};

class Storage {
public:
    explicit Storage(const GrpcDriver &d);

    std::vector<Company> getAllCompanies() const;

    CompanyResponse getCompanyInfo(int companyId) const;

    bool saveCompany(const Company& company) const;

    std::vector<Flight> getFlights(int companyId, int droneId) const;

private:
    const GrpcDriver& driver;
};


#endif //WEBSERVER_STORAGE_H
