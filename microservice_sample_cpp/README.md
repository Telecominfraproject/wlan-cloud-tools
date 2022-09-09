# OpenWifi hello world (ow_helloworld)

## What is this?
This is a skeleton Micro Service that shows off all the basic facilities of the OW architecture.

## Building
In order to build the `hello_world` micro service, you will need to install its dependencies, which includes the following:
- cmake
- POCO 1.12 from Arilia
- C++17 compiler
- openssl
- libpq-dev (PortgreSQL development libraries)
- mysql-client (MySQL client)
- librdkafka
- cppkafka

The build is done in 2 parts. The first part is to build a local copy of the framework tailored to your environment. This
framework is called [Poco](https://github.com/pocoproject/poco). The version used in this project has a couple of fixes
from the master copy needed for cmake. Please use the version of this [Poco fix](https://github.com/AriliaWireless/poco). Building
Poco may take several minutes depending on the platform you are building on.

### Ubuntu/Debian
These instructions have proven to work on Ubuntu 20.4.
```
sudo apt install git cmake g++ libssl-dev libmariadb-dev 
sudo apt install libpq-dev libaprutil1-dev apache2-dev libboost-all-dev
sudo apt install librdkafka-dev // default-libmysqlclient-dev
sudo apt install nlohmann-json-dev

cd ~
git clone https://github.com/AriliaWireless/poco --branch poco-tip-v1
cd poco
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

cd ~
git clone https://github.com/AriliaWireless/cppkafka --branch tip-v1
cd cppkafka
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

cd ~
git clone https://github.com/pboettch/json-schema-validator.git --branch 2.1.0
cd json-schema-validator
mkdir cmake-build
cd cmake-build
cmake ..
make -j
sudo make install

git clone https://github.com/fmtlib/fmt --branch 9.0.0 /fmtlib
cd fmtlib
mkdir cmake-build
cd cmake-build
cmake ..
make
make install

cd ~
git clone https://github.com/Telecominfraproject/wlan-cloud-tools
cd wlan-cloud-tools
cd microservice_sample
mkdir cmake-build
cd cmake-build
cmake ..
make -j
```

#### Expected directory layout
From the directory where your cloned source is, you will need to create the `certs`, `data`, and `logs` directories.
```shell
mkdir certs
mkdir logs
mkdir data
```

### Certificates
Love'em of hate'em, we gotta use'em. So we tried to make this as easy as possible for you.

#### The `certs` directory
For all deployments, you will need the following `certs` directory, populated with the proper files.

```asm
certs ---+
         +--- restapi-ca.pem
         +--- restapi-cert.pem
         +--- restapi-key.pem
```

#### Configuration
The configuration for this service is kept in a properties file. This file is called `ow_helloworld.properties`. The file will be loaded 
from root the directory set by the environment variable `OWHELLOWORLD_CONFIG`. To use environment variables in the configuration,
you must use `$<varname>`. Only `path names` support the use of environment variables. The sample configuration requires very
little changes if you keep the suggested directory structure. For the sample configuration to work, you need to define 2
environment variables.
```
export $OWHELLOWORLD_ROOT=`pwd`
export $OWHELLOWORLD_CONFIG=`pwd`
```
If your current working directory is the root of the project, this will set the variables properly. Otherwise, you can set the variables
to point to wherever is necessary.

##### Important config entries

###### This is the type of storage in use
```asm
storage.type = sqlite
```

###### This is the RESTAPI endpoint
```asm
openwifi.restapi.host.0.backlog = 100
openwifi.restapi.host.0.security = relaxed
openwifi.restapi.host.0.rootca = $OWHELLOWORLD_ROOT/certs/restapi-ca.pem
openwifi.restapi.host.0.address = *
openwifi.restapi.host.0.port = 16051
openwifi.restapi.host.0.cert = $OWHELLOWORLD_ROOT/certs/restapi-cert.pem
openwifi.restapi.host.0.key = $OWHELLOWORLD_ROOT/certs/restapi-key.pem
openwifi.restapi.host.0.key.password = mypassword

openwifi.internal.restapi.host.0.backlog = 100
openwifi.internal.restapi.host.0.security = relaxed
openwifi.internal.restapi.host.0.rootca = $OWHELLOWORLD_ROOT/certs/restapi-ca.pem
openwifi.internal.restapi.host.0.address = *
openwifi.internal.restapi.host.0.port = 17051
openwifi.internal.restapi.host.0.cert = $OWHELLOWORLD_ROOT/certs/restapi-cert.pem
openwifi.internal.restapi.host.0.key = $OWHELLOWORLD_ROOT/certs/restapi-key.pem
openwifi.internal.restapi.host.0.key.password = mypassword
```

##### This is the end point for the devices to connect with
2 types of endpoints: `restapi` and `internal`. 

###### `restapi`
This is the public side of the API. This should always be secured and exposed through your container or a firewall. 
This should be protected. The IP address use by this interface should have an FQDN matching its certificate. That 
certificate and key are in `openwifi.restapi.host.0.cert` and `openwifi.restapi.host.0.key`. The port you specify
under `openwifi.restapi.host.0.port` is the one you will use in your URIs to get into the micro service. This 
should be open in your firewall or docker container.

###### `internal`
This is the private side of the API. This should never be exposed through your container or a firewall.
This should be protected. That certificate and key are in `openwifi.restapi.host.0.cert` and `openwifi.restapi.host.0.key`. 
The port you specify under `openwifi.restapi.host.0.port` is the one you will use in your URIs to get into the micro service
behind the firewall or using docker networking. This port is assumed secured. Never expose it.

#### Command line options
The current implementation supports the following. If you use the built-in configuration file, you do not need to use any command-line
options. However, you may decide to use the `--daemon` or `umask` options.

```bash
./ow_helloworld --help
usage: ow_helloworld OPTIONS
A ow_helloworld implementation for TIP.

--daemon        Run application as a daemon.
--umask=mask    Set the daemon's umask (octal, e.g. 027).
--pidfile=path  Write the process ID of the application to given file.
--help          display help information on command line arguments
--file=file     specify the configuration file
--debug         to run in debug, set to true
--logs=dir      specify the log directory and file (i.e. dir/file.log)
```

##### file
This allows you to point to another file without specifying the OWHELLOWORLD_CONFIG variable. The file name must end in `.properties`.
##### daemon
Run this as a UNIX service
##### pidfile
When running as a daemon, the pid of the running service will be set in the specified file
##### debug
Run the service in debug mode.
##### logs
Specify where logs should be kept. You must include an existing directory and a file name. For example `/var/ucentral/logs/log.0`.
##### umask
Set the umask for the running service.

### ALB Support
Support for AWS ALB is provided through the following configuration elements. This is the built-in AWS Application
Load Balancer
```asm
alb.enable = true
alb.port = 16102
```

## Kafka integration
So what about Kafka? Well, the micro service framework has basic integration with Kafka. It is turned off by default, 
to turn it on, in the configuration:
```asm
openwifi.kafka.group.id = hello_world
openwifi.kafka.client.id = hello_world1
openwifi.kafka.enable = false
openwifi.kafka.brokerlist = kafka:9092
openwifi.kafka.commit = false
openwifi.kafka.queue.buffering.max.ms = 50
openwifi.kafka.ssl.ca.location =
openwifi.kafka.ssl.certificate.location =
openwifi.kafka.ssl.key.location =
openwifi.kafka.ssl.key.password =

```
#### `openwifi.kafka.group.id`
This must be set to your service's name and should be unique

#### `openwifi.kafka.client.id`
This must be set to your service's name and should be unique

#### `openwifi.kafka.enable`
Kind of obvious but hey, set `true` or `false`. Default is `false`

#### `openwifi.kafka.brokerlist`
This is a comma separator list of the brokers in your `kafka` deployment. If you are using Docker, this will usually be something like
`kafka:9092`. If you are using an external Kafka cluster, `kafka-cluster.domain.com:9092`.

#### Kafka topics
To read more about Kafka, follow the [document](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/KAFKA.md)

#### Securing `kafka`
This is beyond the scope of this document. As it stands today, the communication between the gateway and `kafka` is expected to be behind a firewall.
However, the framework also allows secure Kafka access. To use secured access, you must fill the fields that
start with `openwifi.kafka.ssl`.

## Contributors
We love ya! We need more of ya! If you want to contribute, make sure you review
the [coding style](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/CODING_STYLE.md) document.
Feel free to ask questions and post issues. 
