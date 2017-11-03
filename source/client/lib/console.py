import cmd
import logging


class Console(cmd.Cmd):
    def __init__(self):
        cmd.Cmd.__init__(self)

    def default(self, line):
        logging.error('unknown command \'%s\'!', line)

    def emptyline(self):
        pass

    def can_exit(self):
        return True

    def do_exit(self, line=None):
        """Exit the console."""
        return True

    def do_EOF(self, line=None):
        """Exit the console."""
        print('exit')
        return self.do_exit(line)

    def help_help(self):
        print('usage: help [topic]')
