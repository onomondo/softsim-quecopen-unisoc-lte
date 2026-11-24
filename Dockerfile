FROM --platform=linux/amd64 ubuntu:26.04@sha256:d66b366dac61b575bee2aef07e9c26e8070096fbe84a3d4d3eb0da069b5d76db

RUN apt update && \
  apt upgrade -y && \
  apt install -y python3 sudo libglib2.0-0 wget make git && \
  rm -rf /var/lib/apt/lists/* && \
  wget https://security.ubuntu.com/ubuntu/pool/main/i/icu/libicu55_55.1-7_amd64.deb && \
  dpkg -i libicu55_55.1-7_amd64.deb && \
  rm libicu55_55.1-7_amd64.deb
