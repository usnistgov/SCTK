FROM alpine:3.8

LABEL Description="NIST Speech Recognition Scoring Toolkit (SCTK)"

# SCTK dependencies
RUN apk add --update \
    alpine-sdk \
    git \
    perl \
 && rm -rf /var/cache/apk/*

WORKDIR /opt

# Build and install all SCTK tools
RUN git clone https://github.com/usnistgov/SCTK \
 && cd SCTK \
 && make config \
 && make all \
 && make install \
 && make clean \
 && cp ./bin/* /usr/local/bin/

WORKDIR /var/sctk

CMD ["sclite"]
