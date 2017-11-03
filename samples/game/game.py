import mess

from mess.decorators import *
from mess import access


class LoggingTest(mess.Actor):
    def __init__(self, *args, **kwargs):
        mess.Actor.__init__(self, *args, **kwargs)

        self.log.info("my name is %r", self.name)
        self.log.debug("my owner is [%s]", self.get_owner_id())
        self.log.notice("my visibility is %s", self.visibility)
        self.visibility = access.Faction
        self.log.notice("now my visibility is %s", self.visibility)
        self.log.warning("this is warning #%d", 1)
        self.log.warning("this is warning #%d", 2)
        self.log.info("now %d formatted float %.2f", 1, 3.141591)
        self.log.error("error!!")
        self.log.critical("CRITICAL ERROR!!")
        self.log.notice(access.Everyone, "Hey everyone!")
        self.log.notice(access.Faction, "Hey faction!")
        self.log.notice(access.Master, "A secret to master")


class MyActorComponent(mess.ActorComponent):
    def __init__(self, parent):
        mess.ActorComponent.__init__(self, parent)


class Building(mess.Actor):
    count = 0
    def __init__(self, *args, **kwargs):
        mess.Actor.__init__(self, *args, **kwargs)
        self.location = mess.Location(0, 0)
        self.component = MyActorComponent(self)
        self.visibility = access.Faction

    def begin_play(self):
        self.name += ' %d' % Building.count
        Building.count += 1

    def tick(self, delta_time):
        self.log.info("%r at %s ticking with delta time = %f", self.name, self.location, delta_time)

    @faction
    def do_something(self):
        """Do something"""
        print("doing something!")


class GameMaster(mess.Game):
    def __init__(self, *args, **kwargs):
        mess.Game.__init__(self, *args, **kwargs)
        self.players = []
        self.logging_test = None

    def initialize(self):
        self.add_player_resource(access.Player, "money", 10000)
        self.add_player_resource(access.Player, "food",      0)
        self.add_player_resource(access.Player, "coffee",    0)

    def register_player(self, new_player):
        new_player.main_building = Building(new_player, "Main Building", "building.main")
        self.players.append(new_player)

        if self.logging_test is None: # Just once...
            self.logging_test = LoggingTest(new_player, "LoggerTest", "logger")
