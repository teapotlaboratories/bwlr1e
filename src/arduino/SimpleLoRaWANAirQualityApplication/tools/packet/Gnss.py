#!/usr/bin/env python3

from .Packet import Packet
from .Utilities import Crc16Ccitt
import struct

'''
// GNSS Payload Definition
struct Frame {
    Header header;
    uint16_t satellite; 
    float hdop;
    double latitude;
    double longitude;
    double altitude;
    Trailer trailer;    
} __attribute__((packed));
'''
class Gnss(Packet):
    kName       = "gnss"
    kType       = 0x03
    kDataSize   = 2 + 4 + 8 + 8 + 8 
    kSize       = ( Packet.Header.kSize + 
                    kDataSize +
                    Packet.Trailer.kSize )
    kStructFormat = "<Hfddd"
    def __init__(self, satellite:int, hdop:float, latitude: float, longitude: float, altitude: float):
        super().__init__()
        self.header.version = Packet.kVersion
        self.header.type    = Gnss.kType
        self.name           = Gnss.kName
        self.Set(satellite, hdop, latitude, longitude, altitude)

    def Set(self, satellite:int, hdop:float, latitude: float, longitude: float, altitude: float):
        self.satellite  = satellite 
        self.hdop       = hdop
        self.latitude   = latitude
        self.longitude  = longitude
        self.altitude   = altitude

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
        ret += bytearray( struct.pack(Gnss.kStructFormat,
                                      self.satellite,
                                      self.hdop,
                                      self.latitude,
                                      self.longitude,
                                      self.altitude) )
        return ret
    
    def GetAsDict(self):
        ret = {}
        ret["satellite"] = self.satellite
        ret["hdop"]      = self.hdop
        ret["latitude"]  = self.latitude
        ret["longitude"] = self.longitude
        ret["altitude"]  = self.altitude
        return ret
    
    @staticmethod
    def FromBytes( data: bytes ):
        # check if valid packet
        if not Packet.IsValid(data):
            return None
        if len(data) < Gnss.kSize:
            return None
                
        # check crc16 checksum
        parsed_trailer = Packet.Trailer.FromBytes( data[Packet.Header.kSize + Gnss.kDataSize : 
                                                        Packet.Header.kSize + Gnss.kDataSize + Packet.Trailer.kSize] )
        calc_checksum  = Crc16Ccitt( data[:Packet.Header.kSize + Gnss.kDataSize] )
        if calc_checksum != parsed_trailer.checksum:
            return None

        # create object
        # parse data and trailer
        parsed_data = struct.unpack(Gnss.kStructFormat, data[Packet.Header.kSize : 
                                                             Packet.Header.kSize + Gnss.kDataSize])
        satellite       = parsed_data[0]
        hdop            = parsed_data[1]
        latitude        = parsed_data[2]
        longitude       = parsed_data[3]
        altitude        = parsed_data[4]
        parsed_packet   = Gnss(satellite, hdop, latitude, longitude, altitude)
        parsed_packet.trailer.checksum = parsed_trailer.checksum

        return parsed_packet        
