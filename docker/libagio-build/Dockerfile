# Set the base image using ubuntu
FROM ubuntu:latest

# Maintainer
MAINTAINER Tim Zheng (zhenghanecho@gmail.com)

# Expose Port
EXPOSE 8888/tcp

RUN apt update

# Environment
RUN apt install -y apt-utils curl wget htop 

# Build tools
RUN apt install -y git cmake g++ pkg-config 

# Dependency
RUN apt install -y libuv1-dev

# clone repo
# RUN git clone https://github.com/ultrasilicon/libagio.git /root/libagio

# Set working directory
WORKDIR /root/libagio

CMD git clone https://github.com/ultrasilicon/libagio.git /root/libagio && /bin/bash