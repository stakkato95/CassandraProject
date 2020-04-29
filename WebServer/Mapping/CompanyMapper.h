//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef WEBSERVER_COMPANYMAPPER_H
#define WEBSERVER_COMPANYMAPPER_H

#include "Mapper.h"
#include "databaseapp.grpc.pb.h"
#include "../../Model/Company.h"

class CompanyMapper : public Mapper<databaseapp::CompanyResponse, Company> {
public:
    Company getModel(const databaseapp::CompanyResponse &response) override;
};


#endif //WEBSERVER_COMPANYMAPPER_H
