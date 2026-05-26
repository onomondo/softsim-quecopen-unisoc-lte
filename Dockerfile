FROM --platform=linux/amd64 ubuntu:24.04@sha256:c4a8d5503dfb2a3eb8ab5f807da5bc69a85730fb49b5cfca2330194ebcc41c7b

RUN apt update && \
  apt upgrade -y && \
  apt install -y python3 sudo libglib2.0-0 wget make git && \
  rm -rf /var/lib/apt/lists/* && \
  wget https://security.ubuntu.com/ubuntu/pool/main/i/icu/libicu55_55.1-7_amd64.deb && \
  dpkg -i libicu55_55.1-7_amd64.deb && \
  rm libicu55_55.1-7_amd64.deb
