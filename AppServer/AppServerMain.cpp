#include <iostream>
#include <memory>
#include <string>
#include <typeindex>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "databaseapp.grpc.pb.h"

#include "Driver/CassDriverWrapper.h"
#include "Driver/CassDriverAdapter.h"

#include "Adapter/SensorAdapter.h"
#include "Adapter/UserAdapter.h"
#include "Adapter/CompanyAdapter.h"
#include "Adapter/DroneAdapter.h"

#include "../Model/Sensor.h"
#include "../Model/User.h"
#include "../Model/Company.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
using databaseapp::EmptyRequest;
using databaseapp::CompanyResponse;
using databaseapp::ApplicationServer;

class ApplicationServerServiceImpl final : public ApplicationServer::Service {
public:
    explicit ApplicationServerServiceImpl(CassDriverAdapter &d) : driver{d} {}

    Status getAllCompanies(ServerContext *context,
                           const EmptyRequest *request,
                           ServerWriter<CompanyResponse> *writer) override {
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
            DriverError& error = get<DriverError>(result);
            cout << error.error << " " << error.message << endl;
        }

        return Status::CANCELLED;
    }

private:
    CassDriverAdapter &driver;
};

void RunServer() {
    CassDriverAdapter driver(CassDriverWrapper("127.0.0.1", "autonomousflight"));
    //driver.registerAdapter<Sensor, SensorAdapter>("sensor");
    //driver.registerAdapter<User, UserAdapter>("users");
    driver.registerAdapter<Company, CompanyAdapter>("company");

    variant<DriverError, vector<Company>> result = driver.select<Company, CompanyAdapter>({});

    string server_address("localhost:50051");
    ApplicationServerServiceImpl service(driver);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on " << server_address << endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

void testDriverWrapper() {
    CassDriverWrapper wrapper("127.0.0.1", "demo");
    wrapper.connect();

    //test insert
//    ContentValues attrs{
//            {"lastname",  "Jones"},
//            {"age",       35},
//            {"city",      "Austin"},
//            {"email",     "bob@example.com"},
//            {"firstname", "Bob"}
//    };
//    wrapper.insert("users", attrs);


    //test update
//    ContentValues attrs{{"age", 111}};
//    ContentValues whereUpdate{{"lastname", "Aaliaha"}};
//    wrapper.update("users", whereUpdate, attrs);

    //test delete
//    ContentValues where{{"lastname", "Jones"}};
//    wrapper.del("users", where);

    //test select
    //ContentValues where{{"lastname", "Jones"}};
    ContentMappings attrsRow{
            {"lastname",  type_index(typeid(string))},
            {"age",       type_index(typeid(int))},
            {"city",      type_index(typeid(string))},
            {"email",     type_index(typeid(string))},
            {"firstname", type_index(typeid(string))}
    };
    if (auto result = wrapper.select("users", {}, attrsRow); holds_alternative<ResultCollection>(result)) {
        ResultCollection collection = get<ResultCollection>(result);

        for (const ContentValues &content : collection) {
            for (const pair<string, object> &pair : content) {
                if (holds_alternative<string>(pair.second)) {
                    cout << get<string>(pair.second);
                } else if (holds_alternative<int>(pair.second)) {
                    cout << get<int>(pair.second);
                } else if (holds_alternative<float>(pair.second)) {
                    cout << get<float>(pair.second);
                }
                cout << " ";
            }
            cout << endl;
        }

    }

    wrapper.disconnect();
}

void testDriverAdapter() {
    CassDriverAdapter driver(CassDriverWrapper("127.0.0.1", "demo"));
    //driver.registerAdapter<Sensor, SensorAdapter>("sensor");
    driver.registerAdapter<User, UserAdapter>("users");

    if (variant<DriverError, vector<User>> result = driver.select<User, UserAdapter>({});
            holds_alternative<vector<User>>(result)) {
        vector<User> users = get<vector<User>>(result);
        for (const User &user : users) {
            cout << user.firstname << " " << user.lastname << endl;
        }
    }
}

int main(int argc, char **argv) {
    //testDriverAdapter();

    CassDriverAdapter driver(CassDriverWrapper("127.0.0.1", "autonomousflight"));
    driver.registerAdapter<Drone, DroneAdapter>("drone");
    if (variant<DriverError, vector<Drone>> result = driver.select<Drone, DroneAdapter>({});
            holds_alternative<vector<Drone>>(result)) {
        vector<Drone> users = get<vector<Drone>>(result);
        for (const Drone &user : users) {
            cout << user.droneId << " " << user.model << endl;
        }
    } else {
        DriverError er = get<DriverError>(result);
        cout << er.error << " " << er.message << endl;
    }

    RunServer();

    return 0;
}
