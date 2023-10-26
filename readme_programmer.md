# Teapotlabs BWLR1E Programmer	
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/main/docs/images/programmer/device.jpg" alt="drawing"  width="50%" height="50%"/></p>
 
Teapot BWLR1E is a solar-powered wireless LoRa environmental sensor capable of sensing temperature, humidity, air pressure and air quality using the on-board BME688. 
With STM32WLE MCU as it's core and AEM10941 for solar charging, the device is capable of multi-year operation with the possibility of indefinite battery-life by utilizing the solar charging capability

Teapot BWLR1E is part of  [Teapot open-hardware project](https://github.com/teapotlaboratories). 
 
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
- Pinout for external button

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
<p align="center">  <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/showcase_render.gif" alt="case_render"  width="50%" height="50%"/></p>

Built using [TinkerCAD](https://www.tinkercad.com). The case are 3D printable with any generic 3D printer with/without support. The STL files are available [here](https://github.com/teapotlaboratories/bwlr1e/tree/main/hardware/programmer/case)
 <p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/programmer/case_open.jpg" alt="drawing"  width="50%" height="50%"/><br><b>Case Open</b></p>

### Bill Of Materials
Most of the components are generic and can be bought from any electornics/semi-conductor distributor. The bill of materials can be downloaded [here](https://github.com/teapotlaboratories/bwlr1e/blob/main/hardware/programmer/bill_of_materials.csv)

## Programming

 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/programmer/programming_bwlr1e.gif" width="30%" height="30%"><br>Setting-up Teapotlabs BWLR1E Programmer</p>
 
Programming the device can be done over the **UART2** or **SWD**, available on the right side of the board or uses the [BWLR1E Programmer](https://github.com/teapotlaboratories/bwlr1e/blob/main/readme_programmer.md).
Out of the factory, the RAK3172 chip ships with an **AT firmware** that can be tested by connecting a USB-to-UART bridge to the **UART2** port.

 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/programmer/showcase_render_exploded_view.png" width="30%" height="30%"><br>Teapotlabs BWLR1E Programmer Exploded View</p>

The following are some very good tutorial to start developing with the device:

- [Communicating with the AT firmware](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-lora-lorawan-modem-via-at-command)
 - [Programming with Arduino](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-stand-alone-device-using-rui3)
 - [Programming with STM32Cube](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development/#rak3172-on-stm32cubeide-with-stm32wl-sdk-v1-0-0)
 - [Programming with MbedOS](https://github.com/hallard/LoRa-E5-Tiny/blob/main/README.md#compile-and-flash-firmware)

For connecting to the **UART2** port, use any USB-to-UART bridge module. In testing, the [Sparkfun](https://www.sparkfun.com/products/14050) board is used for communication with AT firmware and programming over **Arduino**.
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/sparkfun_ftdi.jpeg" width="30%" height="30%"><br>Sparkfun USB-to-UART Bridge</p>

> :warning: **Be sure to only use 3.3V module. Do not 5V module** 

### Programming with Teapotlabs BWLR1E Programmer
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/programmer/programming_bwlr1e.gif" width="30%" height="30%"><br>Teapotlabs BWLR1E Programmer</p>

An alternative to solder-free programming is to use the Teapotlabs BWLR1E Programmer. A full detail of the device can be found [here](https://github.com/teapotlaboratories/bwlr1e/blob/main/readme_programmer.md).

## License
The product is open-source! However, some part of library used under **src**, might have it's own license.
Please reach out or create a ticket to report any license violation.

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
