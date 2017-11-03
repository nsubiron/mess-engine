import datetime

from contextlib import contextmanager

def bold(text):
    return '\033[1m%s\033[0m' % text

def green(text):
    return '\x1b[1;32m%s\x1b[0m' % text

def yellow(text):
    return '\x1b[1;33m%s\x1b[0m' % text

def red(text):
    return '\x1b[1;31m%s\x1b[0m' % text

def blue(text):
    return '\x1b[1;34m%s\x1b[0m' % text


def to_hex_str(obj):
    if isinstance(obj, str):
        return ':'.join('{:02x}'.format(ord(c)) for c in obj)
    else:
        return '{:016x}'.format(obj)


@contextmanager
def make_client(client_type, *args, **kwargs):
    """Context manager to create a client."""
    client = None
    try:
        client = client_type(*args, **kwargs)
        client.connect()
        yield client
    finally:
        if client is not None:
            client.disconnect()


class StopWatch(object):
    def __init__(self):
        self.start = datetime.datetime.now()
        self.end = None

    def stop(self):
        self.end = datetime.datetime.now()

    def milliseconds(self):
        return 1000.0 * (self.end - self.start).total_seconds()
