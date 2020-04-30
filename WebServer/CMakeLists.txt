cmake_minimum_required(VERSION 3.16)
project(WebServer)
set(CMAKE_CXX_STANDARD 20)

################################################################################################
#Protobuf commands
################################################################################################
if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else ()
    add_definitions(-D_WIN32_WINNT=0x600)
endif ()

find_package(Threads REQUIRED)

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if (CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else ()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif ()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if (CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else ()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif ()

# Proto file
get_filename_component(hw_proto "../Protobuffer/protos/databaseapp.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/databaseapp.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/databaseapp.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/databaseapp.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/databaseapp.grpc.pb.h")
add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
################################################################################################


################################################################################################
#POCO commands
################################################################################################

include(FindPackageHandleStandardArgs)

# C/C++ Apache Cassandra driver dependency
set(_LIBCASSANDRA_ROOT_PATHS "${PROJECT_SOURCE_DIR}/libs/cassandra/")
set(_LIBCASSANDRA_ROOT_HINTS ${LIBCASSANDRA_ROOT_DIR} $ENV{LIBCASSANDRA_ROOT_DIR})
if(NOT WIN32)
    set(_LIBCASSANDRA_ROOT_PATHS "${_LIBCASSANDRA_ROOT_PATHS}" "/usr/" "/usr/local/")
endif()
set(_LIBCASSANDRA_ROOT_HINTS_AND_PATHS HINTS
        HINTS ${_LIBCASSANDRA_ROOT_HINTS}
        PATHS ${_LIBCASSANDRA_ROOT_PATHS})

find_path(LIBCASSANDRA_INCLUDE_DIR
        NAMES cassandra.h
        HINTS ${_LIBCASSANDRA_INCLUDEDIR} ${_LIBCASSANDRA_ROOT_HINTS_AND_PATHS}
        PATH_SUFFIXES include)
find_library(LIBCASSANDRA_LIBRARY
        NAMES cassandra cassandra_static libcassandra libcassandra_static
        HINTS ${_LIBCASSANDRA_LIBDIR} ${_LIBCASSANDRA_ROOT_HINTS_AND_PATHS}
        PATH_SUFFIXES lib)

# Libuv dependency
set(_LIBUV_ROOT_PATHS "${PROJECT_SOURCE_DIR}/libs/libuv/")
set(_LIBUV_ROOT_HINTS ${LIBUV_ROOT_DIR} $ENV{LIBUV_ROOT_DIR})
if(NOT WIN32)
    set(_LIBUV_ROOT_PATHS "${_LIBUV_ROOT_PATHS}" "/usr/" "/usr/local/")
endif()
set(_LIBUV_ROOT_HINTS_AND_PATHS HINTS
        HINTS ${_LIBUV_ROOT_HINTS}
        PATHS ${_LIBUV_ROOT_PATHS})

find_path(LIBUV_INCLUDE_DIR
        NAMES uv.h
        HINTS ${_LIBUV_INCLUDEDIR} ${_LIBUV_ROOT_HINTS_AND_PATHS}
        PATH_SUFFIXES include)
find_library(LIBUV_LIBRARY
        NAMES uv uv_a libuv libuv_a
        HINTS ${_LIBUV_LIBDIR} ${_LIBUV_ROOT_HINTS_AND_PATHS}
        PATH_SUFFIXES lib)
find_package_handle_standard_args(Libuv "Could NOT find libuv, try to set the path to the libuv root folder in the system variable LIBUV_ROOT_DIR"
        LIBUV_LIBRARY
        LIBUV_INCLUDE_DIR)

find_package_handle_standard_args(CASSANDRA
        "Could NOT find the C/C++ Apache Cassandra driver, try to set the path to the C/C++ Apache Cassandra driver root folder in the system variable LIBCASSANDRA_ROOT_DIR"
        LIBCASSANDRA_LIBRARY
        LIBCASSANDRA_INCLUDE_DIR)

# Determine if CASS_STATIC needs to be defined
if(${LIBCASSANDRA_LIBRARY} MATCHES "cassandra_static(\\.a|\\.lib)$")
    add_definitions(-DCASS_STATIC)
endif()

LIST(APPEND INCLUDES ${LIBCASSANDRA_INCLUDE_DIR})
LIST(APPEND LIBRARIES ${LIBCASSANDRA_LIBRARY}
        ${LIBUV_LIBRARY})
if(WIN32)
    # Additional required libraries for libuv
    LIST(APPEND LIBRARIES iphlpapi psapi wsock32 crypt32 ws2_32 userenv)
endif()

# POCO
find_package(Poco REQUIRED Net)
find_package(Poco REQUIRED Util)
find_package(Poco REQUIRED JSON)

include_directories(${INCLUDES})
################################################################################################


################################################################################################
#Mustache commands
################################################################################################

find_package(mstch)

################################################################################################

# Targets
add_executable(WebServer WebServerMain.cpp
        ${hw_proto_srcs}
        ${hw_grpc_srcs}
        Storage/GrpcDriver.cpp Storage/GrpcDriver.h
        ../Model/Company.h ../Model/Sensor.h
        ../Model/Mapping/Mapper.h
        ../Model/Mapping/CompanyMapper.h ../Model/Mapping/CompanyMapper.cpp
        ../Model/Mapping/DroneMapper.h ../Model/Mapping/DroneMapper.cpp
        Webserver.cpp Webserver.h
        Helper.h
        Storage/Storage.cpp Storage/Storage.h
        RequestHandler/WebRequestHandlerFactory.cpp RequestHandler/WebRequestHandlerFactory.h
        RequestHandler/CompaniesRequestHandler.cpp RequestHandler/CompaniesRequestHandler.h
        RequestHandler/CompanyRequestHandler.cpp RequestHandler/CompanyRequestHandler.h
        RequestHandler/AddCompanyRequestHandler.cpp RequestHandler/AddCompanyRequestHandler.h)
target_link_libraries(WebServer
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
        ${LIBRARIES})

target_link_libraries(WebServer Poco::Net)
target_link_libraries(WebServer Poco::Util)
target_link_libraries(WebServer Poco::JSON)
target_link_libraries(WebServer mstch::mstch)