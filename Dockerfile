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
RUN apt-get install -y wget
WORKDIR /usr/share
RUN wget -O mysql_connector.tar.gz https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.0.17-linux-glibc2.12-x86-64bit.tar.gz && \
    tar zxvf mysql_connector.tar.gz && \
    rm mysql_connector.tar.gz && \
    mv mysql-connector-c++-8.0.17-linux-glibc2.12-x86-64bit mysql_connector && \
    cp -r mysql_connector/include/* /usr/include && \
    cp -r mysql_connector/lib64/* /usr/local/lib
ENV LD_LIBRARY_PATH="/usr/local/lib:${LD_LIBRARY_PATH}"

#create a mount point to the source dir
RUN mkdir -p /usr/src/poke_bat
WORKDIR /usr/src/poke_bat

