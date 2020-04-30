//
// Created by Artsiom Kaliaha on 4/29/20.
//

#include "ApplicationServerServiceImpl.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;

using databaseapp::EmptyRequest;
using databaseapp::CompanyMessage;
using databaseapp::ApplicationServer;
using databaseapp::CompanyRequest;
using databaseapp::DroneResponse;
using databaseapp::SaveCompanyRequest;
using databaseapp::SaveCompanyResponse;

ApplicationServerServiceImpl::ApplicationServerServiceImpl(CassDriverAdapter &d) : driver{d} {}

template<typename TModel, typename TMapper>
void ApplicationServerServiceImpl::registerMapper() {
    mappers[typeid(TModel)] = new TMapper;
}

Status ApplicationServerServiceImpl::getAllCompanies(ServerContext *context,
                                                     const EmptyRequest *request,
                                                     ServerWriter<CompanyMessage> *writer) {
    if (auto result = driver.select<Company, CompanyAdapter>({}); holds_alternative<vector<Company>>(result)) {
        vector<Company> companies = get<vector<Company>>(result);
        CompanyMessage response;
        for (const Company &comp : companies) {
            response.set_id(comp.id);
            response.set_name(comp.name);
            response.set_address(comp.address);
            if (!writer->Write(response)) {
                break;
            }
        }

        return Status::OK;
    } else {
        DriverError &error = get<DriverError>(result);
        cout << error.error << " " << error.message << endl;
    }

    return Status::CANCELLED;
}

Status ApplicationServerServiceImpl::getDrones(ServerContext *context,
                                               const CompanyRequest *request,
                                               ServerWriter<DroneResponse> *writer) {
    if (auto result = driver.select<Drone, DroneAdapter>({{"companyid", request->id()}});
            holds_alternative<vector<Drone>>(result)) {
        vector<Drone> companies = get<vector<Drone>>(result);
        DroneResponse response;
        for (const Drone &drone : companies) {
            response.set_companyid(drone.companyId);
            response.set_companyname(drone.companyName);
            response.set_droneid(drone.droneId);
            response.set_model(drone.model);
            response.set_firmwareversion(drone.firmwareVersion);
            if (!writer->Write(response)) {
                break;
            }
        }

        return Status::OK;
    } else {
        DriverError &error = get<DriverError>(result);
        cout << error.error << " " << error.message << endl;
    }

    return Status::CANCELLED;
}

Status ApplicationServerServiceImpl::getCompany(ServerContext *context,
                                                const CompanyRequest *request,
                                                CompanyMessage *response) {
    if (auto result = driver.select<Company, CompanyAdapter>({{"id", request->id()}});
            holds_alternative<vector<Company>>(result)) {
        vector<Company> companies = get<vector<Company>>(result);
        if (companies.empty()) {
            cout << "company not found in getCompany()" << endl;
            return Status::CANCELLED;
        }

        response->set_id(companies[0].id);
        response->set_name(companies[0].name);
        response->set_address(companies[0].address);

        return Status::OK;
    } else {
        DriverError &error = get<DriverError>(result);
        cout << error.error << " " << error.message << endl;
    }

    return Status::CANCELLED;
}

Status ApplicationServerServiceImpl::saveCompany(ServerContext *context,
                                                 const CompanyMessage *request,
                                                 SaveCompanyResponse *response) {
    CompanyMapper *mapper = static_cast<CompanyMapper *>(mappers[typeid(Company)]);
    Company company = mapper->getModel(*request);

    response->set_issaved(true);

    return Status::OK;
}

////////////////////////
//templates implementation given we know all the classes that will use our class
////////////////////////

template void ApplicationServerServiceImpl::registerMapper<Company, CompanyMapper>();
template void ApplicationServerServiceImpl::registerMapper<Drone, DroneMapper>();