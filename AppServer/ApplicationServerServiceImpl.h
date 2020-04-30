//
// Created by Artsiom Kaliaha on 4/29/20.
//

#ifndef APPSERVER_APPLICATIONSERVERSERVICEIMPL_H
#define APPSERVER_APPLICATIONSERVERSERVICEIMPL_H

#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "databaseapp.grpc.pb.h"

#include "Driver/CassDriverAdapter.h"

#include "Adapter/UserAdapter.h"
#include "Adapter/CompanyAdapter.h"
#include "Adapter/DroneAdapter.h"

#include "../../Model/Mapping/Mapper.h"
#include "../../Model/Mapping/CompanyMapper.h"
#include "../../Model/Mapping/DroneMapper.h"

#include "../../Model/Company.h"
#include "../../Model/Drone.h"

class ApplicationServerServiceImpl final : public databaseapp::ApplicationServer::Service {
public:
    explicit ApplicationServerServiceImpl(CassDriverAdapter &d);

    template<typename TModel, typename TMapper>
    void registerMapper();

    grpc::Status getAllCompanies(grpc::ServerContext *context,
                                 const databaseapp::EmptyRequest *request,
                                 grpc::ServerWriter<databaseapp::CompanyResponse> *writer) override;

    grpc::Status getCompany(grpc::ServerContext *context,
                            const databaseapp::CompanyRequest *request,
                            databaseapp::CompanyResponse *writer) override;

    grpc::Status getDrones(grpc::ServerContext *context,
                           const databaseapp::CompanyRequest *request,
                           grpc::ServerWriter<databaseapp::DroneResponse> *writer) override;

private:
    CassDriverAdapter &driver;
    std::unordered_map<std::type_index, IMapper *> mappers;
};


#endif //APPSERVER_APPLICATIONSERVERSERVICEIMPL_H
