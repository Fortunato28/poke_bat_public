FROM ubuntu:18.04

#requirements installation
RUN apt-get update
RUN apt-get install -y automake bison flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make cmake pkg-config

#dependency installation for work with thrift
RUN git clone https://github.com/apache/thrift.git /usr/src/thrift
WORKDIR /usr/src/thrift
RUN git checkout 0.12.0 && \
    ./bootstrap.sh && \
    ./configure && \
    make && \
    make install

#dependency installation for config handlers
RUN apt-get install -y libconfig++-dev libgcrypt20-dev

#dependency installation for work with DB
RUN apt-get install -y wget mysql-client libmysqlclient-dev
WORKDIR /usr/share
RUN git clone https://github.com/mysql/mysql-connector-cpp.git && \
    cd mysql-connector-cpp && \
    git checkout 1.1 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install
ENV LD_LIBRARY_PATH="/usr/local/lib:${LD_LIBRARY_PATH}"

#create a mount point to the source dir
RUN mkdir -p /usr/src/poke_bat
WORKDIR /usr/src/poke_bat

