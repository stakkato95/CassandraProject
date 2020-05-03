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
using databaseapp::FlightRequest;
using databaseapp::FlightResponse;
using databaseapp::DroneRequest;

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

Status ApplicationServerServiceImpl::getFlights(ServerContext *context,
                                                const FlightRequest *request,
                                                ServerWriter<FlightResponse> *writer) {
    ContentValues where = {{"companyid", request->companyid()},
                           {"droneid",   request->droneid()}};
    if (auto result = driver.select<Flight, FlightAdapter>(where); holds_alternative<vector<Flight>>(result)) {
        //immediately invoked lambda
        const vector<Flight> f = [&result] {
            vector<Flight> flights = get<vector<Flight>>(result);
            auto last = unique(begin(flights), end(flights), [](const Flight &a, const Flight &b) {
                return a.companyId == b.companyId && a.droneId == b.droneId && a.flightId == b.flightId;
            });
            flights.erase(last, end(flights));

            return flights;
        }();

        FlightResponse response;
        for (const Flight &flight : f) {
            response.set_companyid(flight.companyId);
            response.set_droneid(flight.droneId);
            response.set_flightid(flight.flightId);
            response.set_latitude(flight.latitude);
            response.set_longitude(flight.longitude);
            response.set_elevation(flight.elevation);
            response.set_yearmonthday(flight.yearMonthDay);
            response.set_hourminutesecond(flight.hourMinuteSecond);

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

Status ApplicationServerServiceImpl::getDrone(ServerContext *context,
                                              const DroneRequest *request,
                                              DroneResponse *response) {
    ContentValues where = {
            {"companyid", request->companyid()},
            {"droneid",   request->droneid()}
    };
    if (auto result = driver.select<Drone, DroneAdapter>(where);
            holds_alternative<vector<Drone>>(result)) {
        vector<Drone> drones = get<vector<Drone>>(result);
        if (drones.empty()) {
            cout << "company not found in getCompany()" << endl;
            return Status::CANCELLED;
        }

        response->set_companyid(drones[0].companyId);
        response->set_companyname(drones[0].companyName);
        response->set_droneid(drones[0].droneId);
        response->set_model(drones[0].model);
        response->set_firmwareversion(drones[0].firmwareVersion);

        return Status::OK;
    } else {
        DriverError &error = get<DriverError>(result);
        cout << error.error << " " << error.message << endl;
    }

    return Status::CANCELLED;
}

////////////////////////
//templates implementation given we know all the classes that will use our class
////////////////////////

template void ApplicationServerServiceImpl::registerMapper<Company, CompanyMapper>();

template void ApplicationServerServiceImpl::registerMapper<Drone, DroneMapper>();