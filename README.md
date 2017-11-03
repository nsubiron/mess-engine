MESS Engine
===========

Install dependencies

    $ sudo apt-get install build-essential libboost-all-dev libssl-dev libpython3.5-dev

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
