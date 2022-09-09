# wlan-cloud-tools
OpenWiFi Community tools used with Cloud 2.x SDK  

# Overview
This repository shows how to build a microservice that can be part of the OpenWifi ecosystem, leveraging security, logging, Kafka integration, etc.

# Philosophy
The whole ecosystem must be API driven. We have chosen OpenAPI 3.0 as the standard to express inter-service exchanges. This guarantees language agnostic
architecture.

# Cmake OpenWifi `ow_helloworld`
In the directory `microservice_sample_cpp` you will find a fully working and integrated `hello world` micro service in C++.