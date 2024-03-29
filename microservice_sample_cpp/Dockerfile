ARG ALPINE_VERSION=3.16.2
ARG POCO_VERSION=poco-tip-v1
ARG FMTLIB_VERSION=9.0.0
ARG CPPKAFKA_VERSION=tip-v1
ARG JSON_VALIDATOR_VERSION=2.1.0

FROM alpine:$ALPINE_VERSION AS build-base

RUN apk add --update --no-cache \
    make cmake g++ git \
    postgresql-dev mariadb-dev \
    librdkafka-dev boost-dev openssl-dev \
    zlib-dev nlohmann-json

FROM build-base AS poco-build

ARG POCO_VERSION
ADD https://api.github.com/repos/AriliaWireless/poco/git/refs/tags/${POCO_VERSION} version.json
RUN git clone https://github.com/AriliaWireless/poco --branch ${POCO_VERSION} /poco

WORKDIR /poco
RUN mkdir cmake-build
WORKDIR cmake-build
RUN cmake ..
RUN cmake --build . --config Release -j8
RUN cmake --build . --target install

FROM build-base AS fmtlib-build

ARG FMTLIB_VERSION
ADD https://api.github.com/repos/fmtlib/fmt/git/refs/tags/${FMTLIB_VERSION} version.json
RUN git clone https://github.com/fmtlib/fmt --branch ${FMTLIB_VERSION} /fmtlib

WORKDIR /fmtlib
RUN mkdir cmake-build
WORKDIR cmake-build
RUN cmake ..
RUN make
RUN make install

FROM build-base AS cppkafka-build

ARG CPPKAFKA_VERSION
ADD https://api.github.com/repos/AriliaWireless/cppkafka/git/refs/tags/${CPPKAFKA_VERSION} version.json
RUN git clone https://github.com/AriliaWireless/cppkafka --branch ${CPPKAFKA_VERSION} /cppkafka

WORKDIR /cppkafka
RUN mkdir cmake-build
WORKDIR cmake-build
RUN cmake ..
RUN cmake --build . --config Release -j8
RUN cmake --build . --target install

FROM build-base AS json-schema-validator-build

ARG JSON_VALIDATOR_VERSION
ADD https://api.github.com/repos/pboettch/json-schema-validator/git/refs/tags/${JSON_VALIDATOR_VERSION} version.json
RUN git clone https://github.com/pboettch/json-schema-validator --branch ${JSON_VALIDATOR_VERSION} /json-schema-validator

WORKDIR /json-schema-validator
RUN mkdir cmake-build
WORKDIR cmake-build
RUN cmake ..
RUN make
RUN make install

FROM build-base AS ow_helloworld-build

ADD CMakeLists.txt build /ow_helloworld/
ADD cmake /ow_helloworld/cmake
ADD src /ow_helloworld/src
ADD .git /ow_helloworld/.git

COPY --from=poco-build /usr/local/include /usr/local/include
COPY --from=poco-build /usr/local/lib /usr/local/lib
COPY --from=cppkafka-build /usr/local/include /usr/local/include
COPY --from=cppkafka-build /usr/local/lib /usr/local/lib
COPY --from=json-schema-validator-build /usr/local/include /usr/local/include
COPY --from=json-schema-validator-build /usr/local/lib /usr/local/lib
COPY --from=fmtlib-build /usr/local/include /usr/local/include
COPY --from=fmtlib-build /usr/local/lib /usr/local/lib

WORKDIR /ow_helloworld
RUN mkdir cmake-build
WORKDIR /ow_helloworld/cmake-build
RUN cmake ..
RUN cmake --build . --config Release -j8

FROM alpine:$ALPINE_VERSION

ENV OWHELLOWORLD_USER=ow_helloworld \
    OWHELLOWORLD_ROOT=/ow_helloworld-data \
    OWHELLOWORLD_CONFIG=/ow_helloworld-data

RUN addgroup -S "OWHELLOWORLD_USER" && \
    adduser -S -G "OWHELLOWORLD_USER" "OWHELLOWORLD_USER"

RUN mkdir /openwifi
RUN mkdir -p "$OWHELLOWORLD_ROOT" "$OWHELLOWORLD_CONFIG" && \
    chown "$OWHELLOWORLD_USER": "$OWHELLOWORLD_ROOT" "$OWHELLOWORLD_CONFIG"

RUN apk add --update --no-cache librdkafka su-exec gettext ca-certificates bash jq curl \
    mariadb-connector-c libpq unixodbc postgresql-client

COPY readiness_check /readiness_check
COPY test_scripts/curl/cli /cli

COPY ow_helloworld.properties.tmpl /
COPY docker-entrypoint.sh /
COPY wait-for-postgres.sh /
COPY rtty_ui /dist/rtty_ui
RUN wget https://raw.githubusercontent.com/Telecominfraproject/wlan-cloud-ucentral-deploy/main/docker-compose/certs/restapi-ca.pem \
    -O /usr/local/share/ca-certificates/restapi-ca-selfsigned.pem

COPY --from=ow_helloworld-build /ow_helloworld/cmake-build/ow_helloworld /openwifi/ow_helloworld
COPY --from=cppkafka-build /cppkafka/cmake-build/src/lib /usr/local/lib/
COPY --from=poco-build /poco/cmake-build/lib /usr/local/lib

EXPOSE 16051 16151 17051

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/openwifi/ow_helloworld"]
