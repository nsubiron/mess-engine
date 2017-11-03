import logging
import socket
import struct

from contextlib import contextmanager

from util import to_hex_str


class TCPClient(object):
    def __init__(self, host, port, timeout):
        self._host = host
        self._port = port
        self._timeout = timeout
        self._socket = None

    def connect(self):
        self._log('connecting...')
        self._socket = socket.create_connection(address=(self._host, self._port), timeout=self._timeout)
        self._socket.settimeout(self._timeout)
        self._log('connected')

    def disconnect(self):
        if self._socket is not None:
            self._socket.close()

    def write(self, message):
        header = struct.pack('<L', len(message))
        self._socket.sendall(header + message)

    def read(self):
        header = self._read_n(4)
        if header == '':
            return ''
        length = struct.unpack('<L', header)[0]
        data = self._read_n(length)
        return data

    def _read_n(self, length):
        buf = ''
        while length > 0:
            data = self._socket.recv(length)
            if data == '':
                raise RuntimeError('connection closed')
            buf += data
            length -= len(data)
        return buf

    def _log(self, message, *args):
        logging.debug('tcpclient %s:%s - ' + message, self._host, self._port, *args)
