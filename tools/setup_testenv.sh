#!/bin/bash

CTYPES_VERSION=1.0.2

curl -kL https://raw.github.com/saghul/pythonz/master/pythonz-install | bash
. $HOME/.pythonz/etc/bashrc

sudo apt-get install -y \
    build-essential \
    zlib1g-dev \
    libbz2-dev \
    libssl-dev \
    libreadline-dev \
    libncurses5-dev \
    libsqlite3-dev \
    libgdbm-dev \
    libdb-dev \
    libexpat-dev \
    libpcap-dev \
    liblzma-dev \
    libpcre3-dev

pythonz install 2.4.6 2.5.6 2.6.9 2.7.8 3.1.5 3.2.5 3.3.5 3.4.1

cd /tmp
wget https://dl.dropboxusercontent.com/u/20387324/ctypes-1.0.2.tar.gz

tar -xzf ctypes-${CTYPES_VERSION}.tar.gz
cd ctypes-${CTYPES_VERSION}
$HOME/.pythonz/pythons/CPython-2.4.6/bin/python setup.py install
cd -
rm -rf ctypes*
