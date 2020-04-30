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
    return { move(company), move(drones) };
}