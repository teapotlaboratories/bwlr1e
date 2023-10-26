# Teapotlabs BWLR1E
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/main/docs/images/device.jpg" alt="drawing"  width="50%" height="50%"/></p>
 
Teapotlabs BWLR1E is a solar-powered wireless LoRa environmental sensor capable of sensing temperature, humidity, air pressure and air quality using the on-board BME688. 
With STM32WLE MCU as it's core and AEM10941 for solar charging, the device is capable of multi-year operation with the possibility of indefinite battery-life by utilizing the solar charging capability

Teapotlabs BWLR1E is part of  [Teapot open-hardware project](https://github.com/teapotlaboratories). 
  
## Disclaimer
- The 1KM range is based on [AERQ - Air Quality Monitoring](https://www.seeedstudio.com/blog/2022/04/27/monitoring-indoor-air-pollutants-the-silent-issue-for-smart-city-iot-using-seeed-lora-e5-and-fusion-pcba/) design, but have not been tested on this device yet

## Acknowledgement
The project won't be possible without the amazing work from people across the globe. The following are the reference to those awesome projects:

 - [LoRa e5 Tiny](https://github.com/hallard/LoRa-E5-Tiny)
 - [AERQ - Air Quality Monitoring](https://www.seeedstudio.com/blog/2022/04/27/monitoring-indoor-air-pollutants-the-silent-issue-for-smart-city-iot-using-seeed-lora-e5-and-fusion-pcba/)
	 - [Github](https://github.com/Mircerson/AERQ) 
 - [TSEM](https://hackaday.io/project/159139-tiny-solar-energy-module-tsem)

## Specification

- [RAK3172](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Overview/): An STM32WLE5CC module
- [AEM10941](https://e-peas.com/product/aem10941): Solar energy harvesting
- 3.3V only power/pin. 
- 3uA Deep-Sleep
- BME688 for Environmental Sensing
- Switchable TX Power. 14 dBm(50mA) or 22 dBm(140mA) ( on 915MHz frequency )
- Supports LoRaWAN 1.0.3
- 1KM+ Range
- UART2 breakout for **Arduino/Mbed OS/STM32Cube** programming 
- SWD breakout for debugging and programming
- iPEX antenna connector 
- 3.7 Volts LiPo Battery

## Schematics

<p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/main/hardware/main/schematic.png" alt="schematic"/></p>

## Boards
 <p align="center">  <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/pcb_render.gif" alt="pcb_render"  width="50%" height="50%"/><br><b>PCB Render</b></p>

Built using KiCAD, the board is design to be as small as possible with all components placed on the top side of the PCB.
The following are the lists of revision of the board:
- Revision 1: Initial design
- Revision 2: Better trace, use rounded trace and change solar cell wiring to parallel for all cell
- Revision 3: Rename 3V3 to PRIMIN and breaks out PRIMIN
- Revision 4: Improve layout and wiring, and add Qwiic connector
- Revision 5: Change C6 to 22uF

The following design are based on the latest revision.
| Top Board | Bottom Board |
|--|--|
| <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/assembled_front.jpg" alt="assembled_front"  width="67%" height="67%"/></p> | <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/assembled_back.jpg" alt="assembled_back"  width="70%" height="70%"/></p> |
| <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/pcb_top_kicad.png" alt="pcb_front"  width="73%" height="73%"/></p> | <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/pcb_bottom_kicad.png" alt="pcb_bottom"  width="77%" height="77%"/></p> |

 <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/hardware/main/pcb.svg" alt="pcb"  width="50%" height="50%"/><br><b>PCB Top and Bottom Layout</b></p> 
  
### Case
<p align="center">  <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/case_render.gif" alt="case_render"  width="50%" height="50%"/></p>

Built using [TinkerCAD](https://www.tinkercad.com). The case are 3D printable with any generic 3D printer with/without support. The STL files are available [here](https://github.com/teapotlaboratories/bwlr1e/tree/main/hardware/main/case)
 <p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/case_open.jpg" alt="drawing"  width="50%" height="50%"/><br><b>Case Open</b></p>

For adding additional sensors, Qwiic connector is also available on the left side of the board.
 <p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/using_qwiic.jpg" alt="drawing"  width="50%" height="50%"/><br><b>Connecting to Adafruit BME688</b></p>

The case is design to be as small as possible with an additional magnets in the back to ease the placement of the sensor. The following are the list of material used at the time of testing:
- 3.7v LiPo Battery, 850mAh 6mm x 30mm x 48mm
- 4 piece of 8mm x 2mm neodymium magnet

<p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/placement_showcase.gif" alt="placement_showcase"  width="50%" height="50%"/><br><b>Sensor Placement with Magnet</b></p>

### Measurement
Power consumption and solar charging current are measured using [Nordic PPK2](https://www.nordicsemi.com/Products/Development-hardware/Power-Profiler-Kit-2)  and [CurrentRanger](https://lowpowerlab.com/shop/product/152).
The following are the summary of the measurement:
- Transmit 14dBm:  305ms @ 20mA
- Deep-Sleep : 3.22 uA
- Direct Sunlight Solar Charge: 9mA
- Indirect Sunlight Solar Charge: 300uA

> :warning: **Tested on Revision 3, but should be similar to Revision 5** 

<p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/measurement/deep_sleep.png" alt="deep_sleep"  width="90%" height="90%"/><br><b>Deep-Sleep</b></p>

<p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/measurement/bme680_measure_and_lora_transmit.png" alt="bme688_measure_and_lora_transmit"  width="90%" height="90%"/><br><b>BME688 Measure and LoRa Transmit</b></p>

| Solar Charge - Direct Sunlight | Solar Charge - Indirect Sunlight |
|--|--|
| <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/measurement/charge_direct_sunlight.jpg" alt="assembled_front"  width="80%" height="80%"/></p> | <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/measurement/charge_indirect_sunlight.jpg" alt="assembled_back"  width="70%" height="70%"/></p> |

More measurement can be found [here](https://github.com/teapotlaboratories/bwlr1e/tree/main/docs/measurement)

### Bill Of Materials
Most of the components are generic and can be bought from any electornics/semi-conductor distributor. RAK3172 is the only component available in [RAKwireless store](https://store.rakwireless.com/products/wisduo-lpwan-module-rak3172?variant=40014759493830). The bill of materials can be downloaded [here](https://github.com/teapotlaboratories/bwlr1e/blob/main/hardware/main/bill_of_materials.csv)

> :warning: **Be sure to buy the RAK3172 variant without IPEX to use the On-Board Antenna** 

## Programming

> :warning: **Board can only be powered when LiPo battery is connected** 

Programming the device can be done over the **UART2** or **SWD**, available on the right side of the board or uses the [BWLR1E Programmer](https://github.com/teapotlaboratories/bwlr1e/blob/main/readme_programmer.md).
Out of the factory, the RAK3172 chip ships with an **AT firmware** that can be tested by connecting a USB-to-UART bridge to the **UART2** port.

The following are some very good tutorial to start developing with the device:

- [Communicating with the AT firmware](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-lora-lorawan-modem-via-at-command)
 - [Programming with Arduino](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-stand-alone-device-using-rui3)
 - [Programming with STM32Cube](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development/#rak3172-on-stm32cubeide-with-stm32wl-sdk-v1-0-0)
 - [Programming with MbedOS](https://github.com/hallard/LoRa-E5-Tiny/blob/main/README.md#compile-and-flash-firmware)

For connecting to the **UART2** port, use any USB-to-UART bridge module. In testing, the [Sparkfun](https://www.sparkfun.com/products/14050) board is used for communication with AT firmware and programming over **Arduino**.
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/sparkfun_ftdi.jpeg" width="30%" height="30%"><br>Sparkfun USB-to-UART Bridge</p>

> :warning: **Be sure to only use 3.3V module. Do not 5V module** 

### Available SDK
Through **UART2**, user can program the device using the following SDK:
- [Arduino RUI3 SDK](https://github.com/RAKWireless/RAKwireless-Arduino-BSP-Index)
- [Arduino STM32duino SDK](https://github.com/stm32duino/Arduino_Core_STM32)
- [MbedOS](https://github.com/hallard/LoRa-E5-Tiny/blob/main/README.md#compile-and-flash-firmware)
- [STM32Cube](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development/#installation-of-stm32cube-ide)

Other than [Arduino RUI3 SDK](https://github.com/RAKWireless/RAKwireless-Arduino-BSP-Index), to program the device, user need to put the device in Serial Bootloader mode by,
1. Press and Hold `BOOT` and `RESET` button
2. Release `RESET` button 
3. Wait few seconds ( try 2 seconds )
4. Release `BOOT` button
5. Device should be in Serial Bootloader mode.

You can also use [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) , instead of the SDK  to upload a firmware
  
### Programming with Teapotlabs BWLR1E Programmer
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/programmer/programming_bwlr1e.gif" width="30%" height="30%"><br>Teapotlabs BWLR1E Programmer</p>

An alternative to solder-free programming is to use the Teapotlabs BWLR1E Programmer. A full detail of the device can be found [here](https://github.com/teapotlaboratories/bwlr1e/blob/main/readme_programmer.md).
The programmer connects to UART2 of the device. 

## License
The product is open-source! However, some part of library used under **src**, might have it's own license.
Please reach out or create a ticket to report any license violation.

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
