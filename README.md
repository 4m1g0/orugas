orugas
======

Node installation
-----------------
In another location

    git clone https://github.com/joyent/node.git
    cd node
    git checkout v0.8.9
    ./configure
    make
    sudo make install

Module installation (32 bits)
-------------------

    cd orugas
    npm install --arch=ia32


Usage
-----

    node SerialToJSON.js [port]


