FROM --platform=linux/amd64 ubuntu:26.04@sha256:f144425ff09be612d6d9ad965196e9cdc23dae1f42110a8a11a3e9a8198759f7

RUN apt update && \
  apt upgrade -y && \
  apt install -y python3 sudo libglib2.0-0 wget make git && \
  rm -rf /var/lib/apt/lists/* && \
  wget https://security.ubuntu.com/ubuntu/pool/main/i/icu/libicu55_55.1-7_amd64.deb && \
  dpkg -i libicu55_55.1-7_amd64.deb && \
  rm libicu55_55.1-7_amd64.deb
