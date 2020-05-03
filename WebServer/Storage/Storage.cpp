//
// Created by Artsiom Kaliaha on 4/30/20.
//

#include "Storage.h"

using namespace std;

Storage::Storage(const GrpcDriver &d) : driver{d} {}

vector<Company> Storage::getAllCompanies() const {
    return driver.getAllCompanies();
}

CompanyResponse Storage::getCompanyInfo(int companyId) const {
    Company company = driver.getCompanyById(companyId);
    vector<Drone> drones = driver.getDrones(companyId);
    return {move(company), move(drones)};
}

bool Storage::saveCompany(const Company &company) const {
    return driver.saveCompany(company);
}

vector<Flight> Storage::getFlights(int companyId, int droneId) const {
    return driver.getFlights(companyId, droneId);
}

DroneResponse Storage::getDroneInfo(int companyId, int droneId) const {
    Drone drone = driver.getDrone(companyId, droneId);
    vector<Flight> flights = driver.getFlights(companyId, droneId);
    return {move(drone), move(flights)};
}
