#!/usr/bin/env python3

from .Packet import Packet
from .Utilities import Crc16Ccitt
import struct

'''
// Environmental Payload Definition
struct Frame {
    Header header;
    float battery;          // Voltage
    float temperature;      // Celsius
    float pressure;         // Pascals
    float humidity;         // RH %
    float gas_resistance;   // milli-Ohms
    float lux;              // lux
    Trailer trailer;
} __attribute__((packed));
'''
class Environmental(Packet):
    kName       = "environmental"
    kType       = 0x01
    kDataSize   = 4 + 4 + 4 + 4 + 4 + 4 
    kSize       = ( Packet.Header.kSize + 
                    kDataSize +
                    Packet.Trailer.kSize )
    kStructFormat = "<ffffff"
    def __init__(self, battery:float, temperature:float, pressure: float, humidity: float, gas_resistance: float, lux: float):
        super().__init__()
        self.header.version = Packet.kVersion
        self.header.type    = Environmental.kType
        self.name           = Environmental.kName
        self.Set(battery, temperature, pressure, humidity, gas_resistance, lux)

    def Set(self, battery:float, temperature:float, pressure: float, humidity: float, gas_resistance: float, lux: float):
        self.battery        = battery 
        self.temperature    = temperature
        self.pressure       = pressure
        self.humidity       = humidity
        self.gas_resistance = gas_resistance
        self.lux            = lux

        # calculate crc16-ccitt
        self.trailer.checksum = self.GetChecksum()

    def GetAsBytes(self):
        ret = self.__GetAsBytesWithoutTrailer()
        ret += bytearray( self.trailer.GetAsBytes() )
        return ret

    def GetChecksum(self):
        return Crc16Ccitt( bytes(self.__GetAsBytesWithoutTrailer()) ) 

    def __GetAsBytesWithoutTrailer(self):
        ret = bytearray(self.header.GetAsBytes())
        ret += bytearray( struct.pack(Environmental.kStructFormat,
                                      self.battery,
                                      self.temperature,
                                      self.pressure,
                                      self.humidity,
                                      self.gas_resistance,
                                      self.lux) )
        return ret
    
    def GetAsDict(self):
        ret = {}
        ret["battery"]        = self.battery
        ret["temperature"]    = self.temperature
        ret["pressure"]       = self.pressure
        ret["humidity"]       = self.humidity
        ret["gas_resistance"] = self.gas_resistance
        ret["lux"]            = self.lux
        return ret

    @staticmethod
    def FromBytes( data: bytes ):
        # check if valid packet
        if not Packet.IsValid(data):
            return None
        if len(data) < Environmental.kSize:
            return None
        
        
        # check crc16 checksum
        parsed_trailer = Packet.Trailer.FromBytes( data[Packet.Header.kSize + Environmental.kDataSize : 
                                                        Packet.Header.kSize + Environmental.kDataSize + Packet.Trailer.kSize] )
        calc_checksum  = Crc16Ccitt( data[:Packet.Header.kSize + Environmental.kDataSize] )
        if calc_checksum != parsed_trailer.checksum:
            return None

        # create object
        # parse data
        parsed_data = struct.unpack(Environmental.kStructFormat, 
                                    data[Packet.Header.kSize : 
                                         Packet.Header.kSize + Environmental.kDataSize])
        battery        = parsed_data[0]
        temperature    = parsed_data[1]
        pressure       = parsed_data[2]
        humidity       = parsed_data[3]
        gas_resistance = parsed_data[4]
        lux            = parsed_data[5]
        parsed_packet  = Environmental(battery, temperature, pressure, humidity, gas_resistance, lux)
        parsed_packet.trailer.checksum = parsed_trailer.checksum

        return parsed_packet        
