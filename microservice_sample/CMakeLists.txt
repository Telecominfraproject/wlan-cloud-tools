cmake_minimum_required(VERSION 3.13)
project(ow_helloworld VERSION 2.7.0)

set(CMAKE_CXX_STANDARD 17)

if(UNIX AND APPLE)
    set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
    set(MYSQL_ROOT_DIR /usr/local/opt/mysql-client)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
endif()

if(UNIX AND NOT APPLE)
    set(PostgreSQL_TYPE_INCLUDE_DIR /usr/include/postgresql)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
endif()

if(SMALL_BUILD)
    add_definitions(-DSMALL_BUILD)
endif()

# Auto build increment. You must define BUILD_INCREMENT with cmake -DBUILD_INCREMENT=1
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build)
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/build BUILD_NUM)
    if(BUILD_INCREMENT)
        MATH(EXPR BUILD_NUM "${BUILD_NUM}+1")
        file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/build ${BUILD_NUM})
    endif()
else()
    set(BUILD_NUM 1)
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/build ${BUILD_NUM})
endif()

if(ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --tags
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_RESULT
            OUTPUT_VARIABLE GIT_HASH)
    if(NOT GIT_RESULT EQUAL "0")
        message(FATAL_ERROR "git describe --always --tags failed with ${GIT_RESULT}")
    endif()
    string(REGEX REPLACE "\n$" "" GIT_HASH "${GIT_HASH}")
endif()

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(fmt  REQUIRED)
find_package(nlohmann_json  REQUIRED)
find_package(nlohmann_json_schema_validator REQUIRED)

if(SMALL_BUILD)
    find_package(Poco REQUIRED COMPONENTS Crypto JWT Net Util NetSSL Data DataSQLite)
else()
    find_package(CppKafka REQUIRED)
    find_package(PostgreSQL REQUIRED)
    find_package(MySQL REQUIRED)
    find_package(Poco REQUIRED COMPONENTS JSON Crypto JWT Net Util NetSSL Data DataSQLite DataPostgreSQL DataMySQL)
endif()

include_directories(/usr/local/include  /usr/local/opt/openssl/include src include/kafka /usr/local/opt/mysql-client/include)
configure_file(src/ow_version.h.in ${PROJECT_SOURCE_DIR}/src/ow_version.h @ONLY)
add_compile_options(-Wall -Wextra)

add_executable( ow_helloworld
                build
                src/ow_version.h.in
                src/framework/CountryCodes.h
                src/framework/KafkaTopics.h
                src/framework/MicroService.h
                src/framework/OpenWifiTypes.h
                src/framework/orm.h
                src/framework/StorageClass.h
                src/framework/ConfigurationValidator.cpp
                src/framework/ConfigurationValidator.h
                src/framework/ow_constants.h
                src/RESTAPI/RESTAPI_routers.cpp
                src/RESTObjects/RESTAPI_SecurityObjects.cpp src/RESTObjects/RESTAPI_SecurityObjects.h
                src/RESTAPI/RESTAPI_helloWorld_handler.cpp src/RESTAPI/RESTAPI_helloWorld_handler.h
                src/StorageService.cpp src/StorageService.h
                src/Daemon.cpp src/Daemon.h
                src/framework/ConfigurationValidator.cpp src/framework/ConfigurationValidator.h
        )

if(NOT SMALL_BUILD)

endif()

INSTALL(TARGETS ow_helloworld
        RUNTIME DESTINATION /usr/bin
)

target_link_libraries(ow_helloworld PUBLIC
                ${Poco_LIBRARIES}
                ${Boost_LIBRARIES}
                ${ZLIB_LIBRARIES} )

if(NOT SMALL_BUILD)
    target_link_libraries(ow_helloworld PUBLIC
            ${MySQL_LIBRARIES} ${ZLIB_LIBRARIES}
                CppKafka::cppkafka
                nlohmann_json_schema_validator
                fmt::fmt
             )
    if(UNIX AND NOT APPLE)
        target_link_libraries(ow_helloworld PUBLIC PocoJSON)
    endif()
endif()