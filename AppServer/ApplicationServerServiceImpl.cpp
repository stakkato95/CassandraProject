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
using databaseapp::CompanyResponse;
using databaseapp::ApplicationServer;

ApplicationServerServiceImpl::ApplicationServerServiceImpl(CassDriverAdapter &d) : driver{d} {}

Status ApplicationServerServiceImpl::getAllCompanies(ServerContext *context,
                                                     const EmptyRequest *request,
                                                     ServerWriter<CompanyResponse> *writer) {
    if (auto result = driver.select<Company, CompanyAdapter>({}); holds_alternative<vector<Company>>(result)) {
        vector<Company> companies = get<vector<Company>>(result);
        CompanyResponse response;
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