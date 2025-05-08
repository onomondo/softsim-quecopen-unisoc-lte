FROM --platform=linux/amd64 ubuntu:24.04

RUN apt update
RUN apt upgrade -y
RUN apt install -y python3 sudo libglib2.0-0 wget make git

RUN wget http://security.ubuntu.com/ubuntu/pool/main/i/icu/libicu55_55.1-7_amd64.deb && dpkg -i libicu55_55.1-7_amd64.deb && rm libicu55_55.1-7_amd64.deb
