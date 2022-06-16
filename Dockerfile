FROM ubuntu:focal

WORKDIR /bang

RUN ["apt","update"]
RUN ["apt-get","-y","install","build-essential"]
RUN ["apt","-y","install","sox"]
RUN ["apt-get","-y","install","libsox-fmt-all"]

COPY . .