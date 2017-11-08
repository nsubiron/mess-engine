MESS Engine
===========

Install dependencies (requires a relatively new version of clang too)

    $ sudo apt-get install build-essential ninja-build libboost-all-dev libssl-dev libpython3.5-dev
    $ sudo pip install PyYaml

Setup

    $ ./Setup.sh

Build

    $ make

Run server

    $ ./build/mess/release/bin/mess_server --help

Run client

    $ ./source/client/tcp_client_cmd.py --help

Open Sublime Text project

    $ make sublime && subl ./projects/MESS\ Engine.sublime-project
