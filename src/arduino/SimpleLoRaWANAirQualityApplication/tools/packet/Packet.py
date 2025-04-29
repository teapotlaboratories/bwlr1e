#!/usr/bin/env python3

from abc import ABC, abstractmethod
from .Utilities import Crc16Ccitt
import struct

class Packet:
    '''
    // Header Payload Definition
    struct Header {
        uint8_t version;
        uint8_t type;
    } __attribute__((packed));
    '''
    class Header:
        kSize = 2
        def __init__(self):
            self.version    = 0     # uint8_t
            self.type       = 0     # uint8_t

        def GetAsBytes(self):
            return bytearray( struct.pack('<BB',
                                          self.version,
                                          self.type) )
    
        @staticmethod
        def FromBytes( data: bytes ):
            if len(data) < Packet.Header.kSize:
                return None
            parsed = struct.unpack('<BB', data[:Packet.Header.kSize])
            header = Packet.Header()
            header.version = parsed[0]
            header.type    = parsed[1]
            return header        

    '''
    // Trailer Payload Definition
    struct Trailer {
        uint16_t checksum; // CRC16
    } __attribute__((packed));
    '''
    class Trailer:
        kSize = 2
        def __init__(self):
            self.checksum    = 0    # uint16_t
        
        def GetAsBytes(self):
            return bytearray( struct.pack('<H',
                                          self.checksum) )
        
        @staticmethod
        def FromBytes( data: bytes ):
            if len(data) < Packet.Trailer.kSize:
                return None
            parsed = struct.unpack('<H', data[:Packet.Trailer.kSize])
            trailer = Packet.Trailer()
            trailer.checksum = parsed[0]
            return trailer        

    kVersion = 0x01    
    def __init__(self):
        self.header     = Packet.Header()
        self.trailer    = Packet.Trailer()
        self.name       = ""
 
    def GetType(self):
        return self.header.type

    def GetName(self):
        return self.name

    @abstractmethod
    def GetAsBytes(self):
        pass

    @staticmethod
    def IsValid( data: bytes ):
        # valid packet size is > 4 bytes
        if len(data) <= 4:
            return False

        header = Packet.Header.FromBytes(data)
        if header == None or header.version != Packet.kVersion:
            return False

        return True

        
        
