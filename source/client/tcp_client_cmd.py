#!/usr/bin/env python2

import argparse
import logging

from lib.console import Console
from lib.tcp_client import TCPClient
from lib.util import blue
from lib.util import green
from lib.util import make_client
from lib.util import red


class TcpClientConsole(Console):
    def __init__(self, host, port):
        Console.__init__(self)
        self.host = host
        self.port = port
        self.tcpclient = None
        self.update_prompt()

    def _is_connected(self):
        return self.tcpclient is not None and self.tcpclient._socket is not None

    def _get_display_status(self):
        if self._is_connected():
            return green('(%s:%d)' % (self.tcpclient._host, self.tcpclient._port))
        else:
            return red('(not connected)')

    def update_prompt(self):
        self.prompt = '%s %s$ ' % (blue('client'), self._get_display_status())

    def postcmd(self, stop, line):
        self.update_prompt()
        return stop

    def default(self, line):
        if line.startswith('c ') or line == 'c':
            self.do_connect(line[2:])
        elif line.startswith('d ') or line == 'd':
            self.do_disconnect(line[2:])
        elif line.startswith('s ') or line == 's':
            self.do_send(line[2:])
        elif line.startswith('r ') or line == 'r':
            self.do_read(line[2:])
        else:
            logging.error('unknown command \'%s\'!', line)

    def do_connect(self, line):
        '''connect'''
        try:
            self.tcpclient = TCPClient(self.host, self.port, 2000)
            self.tcpclient.connect()
        except Exception as exception:
            logging.error('exception: %s', exception)
            self.do_disconnect()

    def do_disconnect(self, line=None):
        '''disconnect the client'''
        try:
            if self.tcpclient is not None:
                self.tcpclient.disconnect()
                self.tcpclient = None
        except Exception as exception:
            logging.error('exception: %s', exception)

    def do_send(self, line):
        '''send "message"'''
        try:
            if not self._is_connected():
                raise RuntimeError('server not connected')
            self.tcpclient.write(line)
            self.do_read()
        except Exception as exception:
            logging.error('exception: %s', exception)
            self.do_disconnect()

    def do_read(self, line=None):
        '''read from the socket'''
        try:
            if not self._is_connected():
                raise RuntimeError('server not connected')
            message = self.tcpclient.read()
            print(message)
        except Exception as exception:
            logging.error('exception: %s', exception)
            self.do_disconnect()


def main():
    argparser = argparse.ArgumentParser(description=__doc__)
    argparser.add_argument(
        '-d', '--debug',
        action='store_true',
        help='print debug information')
    argparser.add_argument(
        '--host',
        metavar='H',
        default='127.0.0.1',
        help='IP of the host server (default: 127.0.0.1)')
    argparser.add_argument(
        '-p', '--port',
        metavar='P',
        default=2017,
        type=int,
        help='TCP port to listen to (default: 2017)')

    args = argparser.parse_args()

    logging_config = {
        'format': '%(levelname)s: %(message)s',
        'level': logging.DEBUG if args.debug else logging.INFO
    }
    logging.basicConfig(**logging_config)

    cmd = TcpClientConsole(args.host, args.port)
    cmd.cmdloop()


if __name__ == '__main__':

    main()
