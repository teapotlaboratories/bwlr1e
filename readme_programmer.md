# Teapotlabs BWLR1E Programmer	

<p align="center">  <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/showcase_render.gif" alt="case_render"  width="50%" height="50%"/></p>
 
Teapotlabs BWLR1E Programmer is a device to support programming Teapotlabs BWLR1E without soldering to the device. The programmer interface to the device program pad via pogo-pins.
 
## Specification

- CH340E USB to UART Bridge for Teapotlabs BWLR1E Programming
- 1A Charge rate to Teapotlabs BWLR1E battery
- Pogo-pin to interface to Teapotlabs BWLR1E. Connects the following pin:
	- RXD2
	- TXD2
	- SWDIO
	- SWCLK
	- RST
	- BOOT
	- LIPO+
- On-board `boot` and `reset` button
- Pin header for external button

## Schematics

<p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/main/hardware/programmer/schematic.png" alt="schematic"/></p>

## Boards
 <p align="center">  <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/pcb_render.gif" alt="pcb_render"  width="50%" height="50%"/><br><b>PCB Render</b></p>

Built using KiCAD v7, the board is design to connect to the programming and charge pad of Teapotlabs BWLR1E via pogo-pins connector

The following design are based on the latest revision.
| Top Board | Bottom Board |
|--|--|
| <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/assembled_front.jpg" alt="assembled_front"  width="67%" height="67%"/></p> | <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/assembled_back.jpg" alt="assembled_back"  width="67%" height="67%"/></p> |
| <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/pcb_top.png" alt="pcb_front"  width="73%" height="73%"/></p> | <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/pcb_bottom.png" alt="pcb_bottom"  width="77%" height="77%"/></p> |

 <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/hardware/programmer/pcb.svg" alt="pcb"  width="50%" height="50%"/><br><b>PCB Top and Bottom Layout</b></p> 
  
### Case
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/programmer/showcase_render_exploded_view.png" width="30%" height="30%"><br>Teapotlabs BWLR1E Programmer Exploded View</p>

Built using [TinkerCAD](https://www.tinkercad.com). The case are 3D printable with any generic 3D printer with/without support. The STL files are available [here](https://github.com/teapotlaboratories/bwlr1e/tree/main/hardware/programmer/case)
The case is made of 3 parts:
- Main case: holds the Programmer.
- Separator: separate the programmer and Teapotlabs BWLR1E.
- Locks: locks Teapotlabs BWLR1E in place for charging the device.
 <p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/case_open.jpg" alt="drawing"  width="50%" height="50%"/><br><b>Case Open</b></p>

### Bill Of Materials
Most of the components are generic and can be bought from any electornics/semi-conductor distributor. The bill of materials can be downloaded [here](https://github.com/teapotlaboratories/bwlr1e/blob/main/hardware/programmer/bill_of_materials.csv)

## Programming Teapotlabs BWLR1E

 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/programmer/programming_bwlr1e.gif" width="30%" height="30%"><br>Setting-up Teapotlabs BWLR1E Programmer</p>
 
Using the programmer, Teapotlabs BWLR1E can be programmed via the USB-C available in the programmer.
Out of the factory, the Teapotlabs BWLR1E chip ships with an **AT firmware** that can be tested by sending AT commands using the programmer USB-C.

The following are some very good tutorial to start developing with the device:

- [Communicating with the AT firmware](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-lora-lorawan-modem-via-at-command)
 - [Programming with Arduino](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-stand-alone-device-using-rui3)
 - [Programming with STM32Cube](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development/#rak3172-on-stm32cubeide-with-stm32wl-sdk-v1-0-0)
 - [Programming with MbedOS](https://github.com/hallard/LoRa-E5-Tiny/blob/main/README.md#compile-and-flash-firmware)

### Available SDK
Through the USB-C, user can program Teapotlabs BWLR1E using the following SDK:

-   [Arduino RUI3 SDK](https://github.com/RAKWireless/RAKwireless-Arduino-BSP-Index)
-   [Arduino STM32duino SDK](https://github.com/stm32duino/Arduino_Core_STM32)
-   [MbedOS](https://github.com/hallard/LoRa-E5-Tiny/blob/main/README.md#compile-and-flash-firmware)
-   [STM32Cube](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development/#installation-of-stm32cube-ide)

Other than  [Arduino RUI3 SDK](https://github.com/RAKWireless/RAKwireless-Arduino-BSP-Index), to program the device, user need to put the device in Serial Bootloader mode by,

1.  Press and Hold  `BOOT`  and  `RESET`  button
2.  Release  `RESET`  button
3.  Wait few seconds ( try 2 seconds )
4.  Release  `BOOT`  button
5.  Device should be in Serial Bootloader mode.

You can also use  [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)  , instead of the SDK to upload a firmware

## License
The product is open-source! However, some part of library used under **src**, might have it's own license.
Please reach out or create a ticket to report any license violation.

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

