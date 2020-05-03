//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef APPSERVER_APPLICATIONSERVERSERVICEIMPL_H
#define APPSERVER_APPLICATIONSERVERSERVICEIMPL_H

#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <algorithm>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "databaseapp.grpc.pb.h"

#include "Driver/CassDriverAdapter.h"

#include "Adapter/UserAdapter.h"
#include "Adapter/CompanyAdapter.h"
#include "Adapter/DroneAdapter.h"
#include "Adapter/FlightAdapter.h"

#include "../Model/Mapping/Mapper.h"
#include "../Model/Mapping/CompanyMapper.h"
#include "../Model/Mapping/DroneMapper.h"

#include "../Model/Company.h"
#include "../Model/Drone.h"
#include "../Model/Flight.h"

class ApplicationServerServiceImpl final : public databaseapp::ApplicationServer::Service {
public:
    explicit ApplicationServerServiceImpl(CassDriverAdapter &d);

    template<typename TModel, typename TMapper>
    void registerMapper();

    grpc::Status getAllCompanies(grpc::ServerContext *context,
                                 const databaseapp::EmptyRequest *request,
                                 grpc::ServerWriter<databaseapp::CompanyMessage> *writer) override;

    grpc::Status getCompany(grpc::ServerContext *context,
                            const databaseapp::CompanyRequest *request,
                            databaseapp::CompanyMessage *writer) override;

    grpc::Status getDrones(grpc::ServerContext *context,
                           const databaseapp::CompanyRequest *request,
                           grpc::ServerWriter<databaseapp::DroneResponse> *writer) override;

    grpc::Status saveCompany(grpc::ServerContext *context,
                             const databaseapp::CompanyMessage *request,
                             databaseapp::SaveCompanyResponse *response) override;

    grpc::Status getFlights(grpc::ServerContext *context,
                            const databaseapp::FlightRequest *request,
                            grpc::ServerWriter<databaseapp::FlightResponse> *writer) override;

    grpc::Status getDrone(grpc::ServerContext *context,
                          const databaseapp::DroneRequest *request,
                          databaseapp::DroneResponse *response) override;

private:
    CassDriverAdapter &driver;
    std::unordered_map<std::type_index, IMapper *> mappers;
};


#endif //APPSERVER_APPLICATIONSERVERSERVICEIMPL_H
