from comms.packet import *
from comms.serialize import *


class Test_Outbound:
    def __init__(self, field_1, field_2):
        self.field_1 = field_1
        self.field_2 = field_2

    def pack(self):
        return Packet(377, serialize(
            (Float, Float),
            [self.field_1, self.field_2]
        ))

    def __repr__(self):
        return f'Test_Outbound<{self.field_1}, {self.field_2}>'


class Test_Inbound:

    def __init__(self, p):
        [
            self.field_1, self.field_2, self.field_3
        ], _ = deserialize(
            (Float, Float, Float),
            p.data()
        )

    @staticmethod
    def id():
        return 377

    def __repr__(self):
        return f'Test_Inbound<{self.field_1}, {self.field_2}, {self.field_3}>'


class SwitchToConsole:
    def __init__(self):
        self.id_ = 0

    def pack(self):
        return Packet(self.id_, b' ')


class SimpleMove:
    def __init__(self, msg):
        self.id_ = 66

        self.packet_spec = (
            Float,  # distance
            Float,  # curvature
            Float,  # velocity
        )

        if type(msg) is tuple:
            self.fields = [
                msg[0],
                msg[1],
                msg[2]
            ]
        else:
            self.fields = [
                msg.distance,
                msg.curvature,
                msg.velocity,
            ]

    def pack(self):
        return Packet(self.id_, serialize(
            self.packet_spec,
            self.fields
        ))


class MoveFeedback:
    def __init__(self, p):
        self.id_ = MoveFeedback.id()

        self.packet_spec = (
            Float,  # distance
            Float,  # curvature
            Float,  # velocity
        )

        [
            self.distance, self.curvature, self.velocity
        ], _ = deserialize(
            self.packet_spec,
            p.data()
        )

    @classmethod
    def id(cls):
        return 42

    def __repr__(self):
        return f'MoveFeedback<{self.distance}, {self.curvature}, {self.velocity}>'


class Stop:
    def __init__(self):
        self.id_ = 666

    def pack(self):
        return Packet(self.id_, b' ')


class Twist:
    def __init__(self, msg):
        self.id_ = 88
        self.packet_spec = (Float, Float)

        if type(msg) is Packet:
            self.from_pack(msg)
            return None
        if type(msg) is tuple:
            [self.linear, self.angular] = [msg[0], msg[1]]
        else:
            [self.linear, self.angular] = [msg.linear, msg.angular]
        self.fields = [self.linear, self.angular]

    def pack(self):
        return Packet(self.id_, serialize(self.packet_spec, self.fields))

    @staticmethod
    def id():
        return 88

    def from_pack(self, p):
        [
            self.linear, self.angular
        ], _ = deserialize(
            (Float, Float),
            p.data()
        )
        self.fields = [self.linear, self.angular]

    def __repr__(self):
        return f'Twist<{self.linear}, {self.angular}>'


# class Move:
#     def __init__(self, msg):
#         self.id_ = 74
#         self.packet_spec = (Float,)

#         if type(msg) is Packet:
#             self.from_pack(msg)
#             return None
#         if type(msg) is tuple:
#             [self.power] = [msg[0]]
#         else:
#             [self.power] = [msg.power]
#         self.fields = [self.power]

#     def pack(self):
#         return Packet(self.id_, serialize(self.packet_spec, self.fields))

class Move:
    def __init__(self, msg):
        self.id_ = 80
        self.packet_spec = (Float, Float)

        if type(msg) is Packet:
            self.from_pack(msg)
            return None
        if type(msg) is tuple:
            [self.left, self.right] = [msg[0], msg[1]]
        else:
            [self.left, self.right] = [msg.left, msg.right]
        self.fields = [self.left, self.right]

    def pack(self):
        return Packet(self.id_, serialize(self.packet_spec, self.fields))

    @staticmethod
    def id():
        return 80

    def from_pack(self, p):
        [
            self.left, self.right
        ], _ = deserialize(
            (Float, Float),
            p.data()
        )
        self.fields = [self.left, self.right]

    def __repr__(self):
        return f'directions <{self.left}, {self.right}>'


class State:
    def __init__(self, msg):
        self.id_ = 23
        self.packet_spec = (Float,)

        if type(msg) is Packet:
            self.from_pack(msg)
            return None
        if type(msg) is tuple:
            [self.wireless] = [msg[0],]
        else:
            [self.wireless] = [msg.wireless,]
        self.fields = [self.wireless,]

    def pack(self):
        return Packet(self.id_, serialize(self.packet_spec, self.fields))

    @staticmethod
    def id():
        return 23

    def from_pack(self, p):
        [
            self.autonomous
        ], _ = deserialize(
            (Float),
            p.data()
        )
        self.fields = [self.wireless]

    def __repr__(self):
        return f'directions <{self.wireless}>'


class Sensor_Data:
    def __init__(self, msg):
        self.id_ = 501
        self.packet_spec = (Int32, Int32, Int32, Float, Int32, Int32, Float)

        if type(msg) is Packet:
            self.from_pack(msg)
            return None
        if type(msg) is tuple:
            [
                self.time, self.potentiometer, self.dist, self.angVelZ,
                self.leftCount, self.rightCount, self.theta
            ] = [
                msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6]
            ]
        else:
            [
                self.time, self.potentiometer, self.dist, self.angVelZ,
                self.leftCount, self.rightCount, self.theta
            ] = [
                msg.time, msg.potentiometer, msg.dist, msg.angVelZ,
                msg.leftCount, msg.rightCount, msg.theta
            ]
        self.fields = [
            self.time, self.potentiometer, self.dist, self.angVelZ,
            self.leftCount, self.rightCount, self.theta
        ]

    def pack(self):
        return Packet(self.id_, serialize(self.packet_spec, self.fields))

    @staticmethod
    def id():
        return 501

    def from_pack(self, p):
        [
            self.time, self.potentiometer, self.dist, self.angVelZ,
            self.leftCount, self.rightCount, self.theta
        ], _ = deserialize(
            (Int32, Int32, Int32, Float, Int32, Int32, Float),
            p.data()
        )
        self.fields = [
            self.time, self.potentiometer, self.dist, self.angVelZ,
            self.leftCount, self.rightCount, self.theta
        ]

    def __repr__(self):
        return f'Sensor_Data<{self.time}, {self.potentiometer}, {self.dist}, {self.angVelZ}, {self.leftCount}, {self.rightCount}, {self.theta}>'
