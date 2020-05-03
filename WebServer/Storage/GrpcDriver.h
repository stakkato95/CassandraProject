//
// Created by Artsiom Kaliaha on 4/28/20.
//

#ifndef WEBSERVER_GRPCDRIVER_H
#define WEBSERVER_GRPCDRIVER_H

#include "../../Model/Mapping/Mapper.h"
#include "../../Model/Mapping/CompanyMapper.h"
#include "../../Model/Mapping/DroneMapper.h"
#include "../../Model/Mapping/FlightMapper.h"

#include "../../Model/Company.h"
#include "../../Model/Drone.h"
#include "../../Model/Flight.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

#include <grpcpp/grpcpp.h>
#include "databaseapp.grpc.pb.h"

class GrpcDriver {
public:
    explicit GrpcDriver(std::shared_ptr<grpc::Channel> channel);

    template<typename TModel, typename TMapper>
    void registerMapper();

    std::vector<Company> getAllCompanies() const;

    Company getCompanyById(int companyId) const;

    std::vector<Drone> getDrones(int companyId) const;

    bool saveCompany(const Company& company) const;

    std::vector<Flight> getFlights(int companyId, int droneId) const;
private:
    std::unordered_map<std::type_index, IMapper *> mappers;
    std::unique_ptr<databaseapp::ApplicationServer::Stub> stub;
};


#endif //WEBSERVER_GRPCDRIVER_H
