#!/usr/bin/env python3

from .Packet import Packet
from .Environmental import Environmental
from .Imu import Imu
from .Gnss import Gnss

def Parse( data: bytes ):

    parsed = []
    while True:
        packet = None
        size = 0

        if not Packet.IsValid( data ):
            break
        
        header = Packet.Header.FromBytes( data )
        if header == None:
            break
        
        # parse known packet
        if header.type == Environmental.kType:
            packet  = Environmental.FromBytes( data )
            size    = Environmental.kSize 
        elif header.type == Imu.kType:
            packet  = Imu.FromBytes( data )
            size    = Imu.kSize
        elif header.type == Gnss.kType:
            packet  = Gnss.FromBytes( data )
            size    = Gnss.kSize
        else:
            break

        if len(data) < size:
            break 
        
        parsed.append(packet)
        data = data[size:]

    return parsed