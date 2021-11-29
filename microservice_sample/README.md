# OpenWifi hello world (ow_helloworld)

## What is this?
This is a skeleton Micro Service that shows off all the basic facilities of the OW architecture.

## Building
In order to build the uCentralGW, you will need to install its dependencies, which includes the following:
- cmake
- boost
- POCO 1.11.1 or later (special build)
- a C++17 compiler
- libyaml
- openssl
- libpq-dev (PortgreSQL development libraries)
- mysql-client (MySQL client)
- librdkafka
- cppkafka

The build is done in 2 parts. The first part is to build a local copy of the framework tailored to your environment. This
framework is called [Poco](https://github.com/pocoproject/poco). The version used in this project has a couple of fixes
from the master copy needed for cmake. Please use the version of this [Poco fix](https://github.com/stephb9959/poco). Building
Poco may take several minutes depending on the platform you are building on.

### Ubuntu/Debian
These instructions have proven to work on Ubuntu 20.4.
```
sudo apt install git cmake g++ libssl-dev libmariadb-dev 
sudo apt install libpq-dev libaprutil1-dev apache2-dev libboost-all-dev
sudo apt install librdkafka-dev libmysqlclient-dev default-libmysqlclient-dev

git clone https://github.com/stephb9959/poco
cd poco
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

git clone https://github.com/stephb9959/cppkafka
cd cppkafka
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

cd ~
git clone https://github.com/Telecominfraproject/wlan-cloud-tools
cd wlan-cloud-tools
cd microservice_sample
mkdir cmake-build
cd cmake-build
cmake ..
make -j
```

### After completing the build
After completing the build, you can remove the Poco source as it is no longer needed.

#### Expected directory layout
From the directory where your cloned source is, you will need to create the `certs`, `logs`, and `uploads` directories.
```shell
mkdir certs
mkdir logs
mkdir data
```

### Certificates
Love'em of hate'em, we gotta use'em. So we tried to make this as easy as possible for you.

#### The `certs` directory
For all deployments, you will need the following certs directory, populated with the proper files.

```asm
certs ---+
         +--- restapi-ca.pem
         +--- restapi-cert.pem
         +--- restapi-key.pem
```

#### Configuration
The configuration for this service is kept in a properties file. This file is called `owgw.properties` and you can
see the latest version [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/owgw.properties). The file will be loaded from
the directory set by the environment variable `UCENTRALGW_CONFIG`. To use environment variables in the configuration,
you must use `$<varname>`. Only `path names` support the use of environment variables. The sample configuration requires very
little changes if you keep the suggested directory structure. For the sample configuration to work, you need to define 2
environment variables.
```
export OWGW_ROOT=`pwd`
export UCENTRALGW_CONFIG=`pwd`
```
If you current working directory is the root of the project, this will set the variables properly. Otherwise, you can set the variables
to point to wherever is necessary.

##### Important config entries
###### This is the logging directory
```
logging.channels.c2.path = $OWGW_ROOT/logs/sample.log
```

###### This is the type of storage in use
```asm
storage.type = sqlite
```

###### Autoprovisioning settings
```asm
openwifi.autoprovisioning = true
openwifi.devicetypes.0 = AP:linksys_ea8300,edgecore_eap101,linksys_e8450-ubi
openwifi.devicetypes.1 = SWITCH:edgecore_ecs4100-12ph
openwifi.devicetypes.2 = IOT:esp32
```

###### This is the RESTAPI endpoint

```asm
openwifi.restapi.host.0.backlog = 100
openwifi.restapi.host.0.security = relaxed
openwifi.restapi.host.0.rootca = $OWHELLOW_ROOT/certs/restapi-ca.pem
openwifi.restapi.host.0.address = *
openwifi.restapi.host.0.port = 16002
openwifi.restapi.host.0.cert = $OWHELLOW_ROOT/certs/restapi-cert.pem
openwifi.restapi.host.0.key = $OWHELLOW_ROOT/certs/restapi-key.pem
openwifi.restapi.host.0.key.password = mypassword

openwifi.internal.restapi.host.0.backlog = 100
openwifi.internal.restapi.host.0.security = relaxed
openwifi.internal.restapi.host.0.rootca = $OWHELLOW_ROOT/certs/restapi-ca.pem
openwifi.internal.restapi.host.0.address = *
openwifi.internal.restapi.host.0.port = 17002
openwifi.internal.restapi.host.0.cert = $OWHELLOW_ROOT/certs/restapi-cert.pem
openwifi.internal.restapi.host.0.key = $OWHELLOW_ROOT/certs/restapi-key.pem
openwifi.internal.restapi.host.0.key.password = mypassword
```

##### This is the end point for the devices to connect with
This is the crucial section. I bet that 97.4% of all your problems will come from here, and it's boring. So put some good music on,
give the kids the iPad, get a cup of coffee, and pay attention. Every field will be explained.

###### openwifi.websocket.host.0.backlog
This is the number of concurrent devices you are expecting to call all at once. Not the current number of devices. This is how many will connect in the same exact second.
Take the total number of devices you have and divide by 100. That's a good rule of thumb. Never go above 500.

###### openwifi.websocket.host.0.rootca
This is the root file as supplied by Digicert. You can find it [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/root.pem)

###### openwifi.websocket.host.0.issuer
This is the issuer file as supplied by Digicert. You can find it [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/issuer.pem)

###### openwifi.websocket.host.0.cert
This is a `pem` file that you will receive from Digicert for the gateway itself. This is the certificate for the gateway.

###### openwifi.websocket.host.0.key
This is a `pem` file that you will receive from Digicert for the gateway itself. The is the private key for the gateway.

###### openwifi.websocket.host.0.clientcas
This is a `pem` file that contains both the issuer and the root CA certificates. You can find it You can find it [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/clientcas.pem)

###### openwifi.websocket.host.0.cas
This is a directory where you will copy your own `cert.pem`, the `root.pem`, and the `issuer.pem` files.

###### openwifi.websocket.host.0.address
Leve this a `*` in teh case you want to bind to all interfaces on your gateway host or select the address of a single interface.

###### openwifi.websocket.host.0.port
Leave to 15002 for now.

###### openwifi.websocket.host.0.security
Leave this as strict for now for devices.

###### openwifi.websocket.host.0.key.password
If you key file uses a password, please enter it here.

###### openwifi.websocket.maxreactors
A single reactor can handle between 1000-2000 devices. Never leave this smaller than 5 or larger than 50.

#### Conclusion
You will need to get the `cert.pem` and `key.pem` from Digicert. The rest is here.

###### host.0.address entries
If you want to limit traffic to a specific interface, you should specify the IP address of that interface instead of
the `*`. Using the `*` means all interfaces will be able to accept connections. You can add multiple interfaces
by changing the `0` to another index. You need to repeat the whole configuration block for each index. Indexes must be sequential
start at `0`.

```asm
openwifi.service.key = $OWGW_ROOT/certs/websocket-key.pem
```

#### Command line options
The current implementation supports the following. If you use the built-in configuration file, you do not need to use any command-line
options. However, you may decide to use the `--daemon` or `umask` options.

```bash
./ucentralgw --help
usage: ucentralgw OPTIONS
A uCentral gateway implementation for TIP.

--daemon        Run application as a daemon.
--umask=mask    Set the daemon's umask (octal, e.g. 027).
--pidfile=path  Write the process ID of the application to given file.
--help          display help information on command line arguments
--file=file     specify the configuration file
--debug         to run in debug, set to true
--logs=dir      specify the log directory and file (i.e. dir/file.log)
```

##### file
This allows you to point to another file without specifying the UCENTRALGW_CONFIG variable. The file name must end in `.properties`.
##### daemon
Run this as a UNIX service
##### pidfile
When running as a daemon, the pid of the running service will be set in the speficied file
##### debug
Run the service in debug mode.
##### logs
Speficy where logs should be kept. You must include an existing directory and a file name. For example `/var/ucentral/logs/log.0`.
##### umask
Seet the umask for the running service.

### ALB Support
Support for AWS ALB is provided through the following configuration elements
```asm
alb.enable = true
alb.port = 16102
```

## Kafka integration
So what about Kafka? Well, the gateway has basic integration with Kafka. It is turned off by default, to turn it on, in the configuration:

```asm
openwifi.kafka.group.id = hello_world
openwifi.kafka.client.id = hello_world1
openwifi.kafka.enable = false
openwifi.kafka.brokerlist = 127.0.0.1:9092
openwifi.kafka.commit = false
openwifi.kafka.queue.buffering.max.ms = 50
```
#### `openwifi.kafka.group.id`
This must be set to your service's name and should be unique

#### `openwifi.kafka.client.id`
This must be set to your service's name and should be unique

#### `openwifi.kafka.enable`
Kind of obvious but hey, set `true` or `false`. Default is `false`

#### `openwifi.kafka.brokerlist`
This is a comma separator list of the brokers in your `kafka` deployment.

#### Kafka topics
Toe read more about Kafka, follow the [document](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/KAFKA.md)

#### Securing `kafka`
This is beyond the scope of this document. As it stands today, the communication between the gateway and `kafka` is expected to be behind a firewall.

## Contributors
We love ya! We need more of ya! If you want to contribute, make sure you review
the [coding style](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/CODING_STYLE.md) document.
Feel free to ask questions and post issues. 
