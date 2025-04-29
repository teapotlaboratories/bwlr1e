#!/usr/bin/env python3

from .Packet import Packet
from .Utilities import Crc16Ccitt
import struct

'''
// IMU Payload Definition
struct Frame {
    Header header;
    Vector magnetic;        // micro-Tesla (uT)
    Vector acceleration;    // meter per second (m/s^2
    Vector gyro;            // rad/s
    Trailer trailer;
} __attribute__((packed));
'''
class Imu(Packet):
    '''
    // Vector Payload Definition
    struct Vector {
        float x;
        float y;
        float z;
    } __attribute__((packed));
    '''
    class Vector:
        kSize = 4 + 4 + 4
        kStructFormat = "<fff"
        def __init__(self):
            self.x = 0 # float
            self.y = 0 # float
            self.z = 0 # float

        def GetAsBytes(self):
            return bytearray( struct.pack(Imu.Vector.kStructFormat,
                                          self.x,
                                          self.y,
                                          self.z) )

        def GetAsDict(self):
            ret = {}
            ret["x"] = self.x
            ret["y"] = self.y
            ret["z"] = self.z
            return ret
        
        @staticmethod
        def FromBytes( data: bytes ):
            parsed = struct.unpack(Imu.Vector.kStructFormat, data)
            vector = Vector()
            vector.x = parsed[0]
            vector.y = parsed[1]
            vector.z = parsed[2]
            return vector        
    
    kName       = "imu"
    kType       = 0x02
    kDataSize   = Vector.kSize + Vector.kSize + Vector.kSize
    kSize       = ( Packet.Header.kSize + 
                    kDataSize +
                    Packet.Trailer.kSize )
    def __init__(self, magnetic: Vector, acceleration: Vector, gyro: Vector):
        super().__init__()
        self.header.version = Packet.kVersion
        self.header.type    = Imu.kType
        self.name           = Imu.kName
        self.Set(magnetic, acceleration, gyro)

    def Set(self, magnetic: Vector, acceleration: Vector, gyro: Vector):
        self.magnetic       = magnetic 
        self.acceleration   = acceleration
        self.gyro           = gyro

        # calculate crc16-ccitt
        self.trailer.checksum = self.GetChecksum()

    def GetAsBytes(self):
        ret = self.__GetAsBytesWithoutTrailer()
        ret += bytearray( self.trailer.GetAsBytes() )
        return ret

    def GetChecksum(self):
        return Crc16Ccitt( bytes(self.__GetAsBytesWithoutTrailer()) ) 

    def __GetAsBytesWithoutTrailer(self):
        ret = bytearray( self.header.GetAsBytes() )
        ret += bytearray( self.magnetic.GetAsBytes() )
        ret += bytearray( self.acceleration.GetAsBytes() )
        ret += bytearray( self.gyro.GetAsBytes() )
        return ret

    def GetAsDict(self):
        ret = {}
        ret["magnetic"]     = self.magnetic.GetAsDict()
        ret["acceleration"] = self.acceleration.GetAsDict()
        ret["gyro"]         = self.gyro.GetAsDict()
        return ret


    @staticmethod
    def FromBytes( data: bytes ):
        # check if valid packet
        if not Packet.IsValid(data):
            return None
        if len(data) < Imu.kSize:
            return None
        
        # check crc16 checksum
        parsed_trailer = Packet.Trailer.FromBytes( data[Packet.Header.kSize + Imu.kDataSize : 
                                                        Packet.Header.kSize + Imu.kDataSize + Packet.Trailer.kSize] )
        calc_checksum  = Crc16Ccitt( data[:Packet.Header.kSize + Imu.kDataSize] )
        if calc_checksum != parsed_trailer.checksum:
            return None

        # create object
        # parse data
        magnetic = Imu.Vector()
        parsed_data = struct.unpack(Imu.Vector.kStructFormat, data[Packet.Header.kSize + (Imu.Vector.kSize * 0): 
                                                                   Packet.Header.kSize + Imu.Vector.kSize])
        magnetic.x = parsed_data[0]
        magnetic.y = parsed_data[1]
        magnetic.z = parsed_data[2]
        
        acceleration = Imu.Vector()
        parsed_data = struct.unpack(Imu.Vector.kStructFormat, data[Packet.Header.kSize + (Imu.Vector.kSize * 1) : 
                                                                   Packet.Header.kSize + (Imu.Vector.kSize*2)])
        acceleration.x = parsed_data[0]
        acceleration.y = parsed_data[1]
        acceleration.z = parsed_data[2]

        gyro = Imu.Vector()
        parsed_data = struct.unpack(Imu.Vector.kStructFormat, data[Packet.Header.kSize + (Imu.Vector.kSize * 2) :
                                                                   Packet.Header.kSize + (Imu.Vector.kSize*3)])
        gyro.x = parsed_data[0]
        gyro.y = parsed_data[1]
        gyro.z = parsed_data[2]

        parsed_packet = Imu(magnetic, acceleration, gyro)
        parsed_packet.trailer.checksum = parsed_trailer.checksum

        return parsed_packet        
