
  
  
# Teapot BWLR1E
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/main/docs/images/device.jpg" alt="drawing"  width="50%" height="50%"/></p>
 
Teapot BWLR1E is a solar-powered wireless LoRa environmental sensor capable of sensing temperature, humidity, air pressure and air quality using the on-board BME688. 
With STM32WLE MCU as it's core and AEM10941 for solar charging, the device is capable of multi-year operation with the possibility of indefinite battery-life by utilizing the solar charging capability

Teapot BWLR1E is part of  [Teapot open-hardware project](https://github.com/teapotlaboratories). 
  
## Disclaimer
- The 1KM range is based on [AERQ - Air Quality Monitoring](https://www.seeedstudio.com/blog/2022/04/27/monitoring-indoor-air-pollutants-the-silent-issue-for-smart-city-iot-using-seeed-lora-e5-and-fusion-pcba/) design, but have not been tested on this device yet
- The position of the BME680 sensor on the board might not be the most efficient

## Specification

- [RAK3172](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Overview/): An STM32WLE5CC module
- [AEM10941](https://e-peas.com/product/aem10941): Solar energy harvesting
- 3.3V only power/pin. 
- 3uA Deep-Sleep
- BME688 for Environmental Sensing
- Switchable TX Power. 14 dBm(50mA) or 22 dBm(140mA) ( on 915MHz frequency )
- Supports LoRaWAN 1.0.3
- 1KM+ Range
- UART2 breakout for **Arduino** progamming
- SWD breakout for **Mbed OS/STM32Cube** programming
- iPEX antenna connector 
- 3.7 Volts LiPo Battery

## Schematics

<p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/main/hardware/schematic.png" alt="schematic"/></p>

## Boards
 <p align="center">  <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/pcb_render.gif" alt="pcb_render"  width="50%" height="50%"/><br><b>PCB Render</b></p>

Built using KiCAD, the board is design to be as small as possible with all components placed on the top side of the PCB.
The following are the lists of revision of the board:
- Revision 1: Initial design
- Revision 2: Better trace, use rounded trace and change solar cell wiring to parallel for all cell
- Revision 3: Rename 3V3 to PRIMIN and breaks out PRIMIN

The following design are based on the latest revision.
| Top Board | Bottom Board |
|--|--|
| <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/assembled_front.jpg" alt="assembled_front"  width="77%" height="77%"/></p> | <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/assembled_back.jpg" alt="assembled_back"  width="70%" height="70%"/></p> |
| <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/pcb_front.png" alt="pcb_front"  width="77%" height="77%"/></p> | <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/pcb_back.png" alt="pcb_bottom"  width="77%" height="77%"/></p> |

 <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/hardware/pcb.png" alt="pcb"  width="50%" height="50%"/><br><b>PCB Top and Bottom Layout</b></p> 
  
### Case
<p align="center">  <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/case_render.gif" alt="case_render"  width="50%" height="50%"/></p>

Built using [TinkerCAD](https://www.tinkercad.com), the cases are available with 2 variant, with or without the programming port. The cases are 3D printable with any generic 3D printer with/without suppport (depends on the orientation). The STL files are available [here](https://github.com/teapotlaboratories/bwlr1e/tree/main/hardware/case)
 <p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/case_open.jpg" alt="drawing"  width="50%" height="50%"/><br><b>Case Open</b></p>

The case is design to be as small as possible with an additional magnets in the back to ease the placement of the sensor. The following are the list of material used at the time of testing:
- 3.7v LiPo Battery, 400 mAh 50mm x 25mm x 35mm ( it is possible to use 2 of this in parallel )
- 4 piece of 8mm x 2mm neodymium magnet

<p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/images/placement_showcase.gif" alt="placement_showcase"  width="50%" height="50%"/><br><b>Sensor Placement with Magnet</b></p>

### Measurement
Power consumption and solar charging current are measured using [Nordic PPK2](https://www.nordicsemi.com/Products/Development-hardware/Power-Profiler-Kit-2)  and [CurrentRanger](https://lowpowerlab.com/shop/product/152).
The following are the summary of the measurement:
- Transmit 14dBm:  305ms @ 20mA
- Deep-Sleep : 3.22 uA
- Direct Sunlight Solar Charge: 9mA
- Indirect Sunlight Solar Charge: 300uA

<p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/measurement/deep_sleep.png" alt="deep_sleep"  width="90%" height="90%"/><br><b>Deep-Sleep</b></p>

<p align="center"><img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/measurement/bme680_measure_and_lora_transmit.png" alt="bme688_measure_and_lora_transmit"  width="90%" height="90%"/><br><b>BME688 Measure and LoRa Transmit</b></p>

| Solar Charge - Direct Sunlight | Solar Charge - Indirect Sunlight |
|--|--|
| <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/measurement/charge_direct_sunlight.jpg" alt="assembled_front"  width="80%" height="80%"/></p> | <p align="center"> <img src="https://github.com/teapotlaboratories/bwlr1e/raw/main/docs/measurement/charge_indirect_sunlight.jpg" alt="assembled_back"  width="70%" height="70%"/></p> |



More measurement can be found [here](https://github.com/teapotlaboratories/bwlr1e/tree/main/docs/measurement)


### Bill Of Materials
Most of the components are generic and can be bought from any electornics/semi-conductor distributor. RAK3172 is the only component available in [RAKwireless store](https://store.rakwireless.com/products/wisduo-lpwan-module-rak3172?variant=40014759493830). The bill of materials can be downloaded [here](https://github.com/teapotlaboratories/bwlr1e/raw/main/hardware/bill_of_materials.csv)

> :warning: **Be sure to buy the RAK3172 variant without IPEX to use the On-Board Antenna** 

|Id |Designator      |Package                                |Quantity|Designation   |
|---|----------------|---------------------------------------|--------|--------------|
|1  |BT1             |JST_PH_S2B-PH-K_1x02_P2.00mm_Horizontal|1       |3.7v LiPo     |
|2  |C17,C8,C14,C13  |C_0603                                 |4       |10uF 10VDC    |
|3  |C2,C4           |CP_EIA-3528-15_AVX-H                   |2       |330uF         |
|4  |C5,C12,C11,C1,C3|C_0603                                 |5       |100nF 10VDC   |
|5  |C6              |C_1210_3225Metric                      |1       |150uF 10VDC   |
|6  |C7              |C_0603                                 |1       |22uF 10VDC    |
|7  |D1              |LED_0603_1608Metric                    |1       |RED           |
|8  |D2              |LED_0603_1608Metric                    |1       |GREEN         |
|9  |E1              |XDCR_ANT-915-USP410                    |1       |ANT-915-USP410|
|10 |L2              |IND_LPS4012-103MRB                     |1       |10uH 0.55A    |
|11 |L3              |L_0603                                 |1       |10uH          |
|12 |Q1              |SOT-23                                 |        |PJA3407       |
|13 |Q2              |SOT-323_SC-70                          |1       |DMG1012UW     |
|14 |R1              |R_0603                                 |1       |4.7M          |
|15 |R11             |R_0603                                 |1       |100K          |
|16 |R12,R5,R13      |R_0603                                 |3       |1K            |
|17 |R14             |R_0603                                 |1       |400K          |
|18 |R16             |R_0603                                 |2       |0             |
|19 |R4              |R_0603                                 |1       |10M           |
|20 |R7,R2,R3,R6     |R_0603                                 |4       |10K           |
|21 |R9,R8,R15,R10   |R_0603                                 |4       |1M            |
|22 |SC4,SC1,SC3,SC2 |KXOB25-05X3F                           |4       |KXOB25-05X3F  |
|23 |SW3             |SW_SPST_TL3342                         |1       |RESET         |
|24 |SW4             |SW_SPST_TL3342                         |1       |BOOT          |
|25 |U1              |BME688                                 |1       |BME688        |
|26 |U2              |RAK3172                                |1       |RAK3172       |
|27 |U3              |QFN-28-1EP_5x5mm_P0.5mm_EP3.35x3.35mm  |1       |AEM10941-QFN  |
|28 |U4              |SOT-23-5                               |1       |XC6215        |


## Programming

> :warning: **Board can only be powered using the LiPo Battery** 

Programming the device can be done over the **UART2** or **SWD**, available on the right side of the board.
Out of the factory, the RAK3172 chip ships with an **AT firmware** that can be tested by connecting a USB-to-UART bridge to the **UART2** port.

The following are some very good tutorial to start developing with the device:

- [Communicating with the AT firmware](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-lora-lorawan-modem-via-at-command)
 - [Programming with Arduino](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-stand-alone-device-using-rui3)
 - [Programming with STM32Cube](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development/#rak3172-on-stm32cubeide-with-stm32wl-sdk-v1-0-0)
 - [Programming with MbedOS](https://github.com/hallard/LoRa-E5-Tiny/blob/main/README.md#compile-and-flash-firmware)

For connecting to the **UART2** port, use any USB-to-UART bridge module. In testing, the [Sparkfun](https://www.sparkfun.com/products/14050) board is used for communication with AT firmware and programming over **Arduino**.
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/sparkfun_ftdi.jpeg" width="30%" height="30%"><br>Sparkfun USB-to-UART Bridge</p>

> :warning: **Be sure to only use 3.3V module. Do not 5V module** 

For connecting to the **SWD** port, use ST-Link v2  in-circuit debugger and programmer from STM. In testing, ST-Link v2 clone will not work. The ST-Link v2 should atleast be reconizeable by the [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html).
A cheap and alternative way to get an authorized ST-Link is to buy a Nucleo board, cut the top part which contain the ST-Link and use it as an external programmer.
 <p align="center"> <img src="https://raw.githubusercontent.com/teapotlaboratories/bwlr1e/master/docs/images/nucleo_st-linkv2.jpeg" width="70%" height="70%"><br>ST-Link v2 from a Nucleo Development Board</p>
Here are some good tutorial to convert a Nucleo to and external ST-Link v2:

 - https://www.radioshuttle.de/en/turtle-en/nucleo-st-link-interface-en/
 - https://jeelabs.org/book/1547a/index.html

## Notes
There are some issue, notes, and behavior that was discovered at the time of testing and development. The following are those discovery:
- Soldering the solar cell is better to be done manually using a soldering iron. Without proper reflow oven, it may damage the solar cell and reduces it's efficiency
- Using Arduino RUI3 framework may introduce some-instability after programming. It is observed that by randomly power-cycling the board in-short interval after flashing, causes the board to hang in Boot mode
- PRIMIN ( or 3V3 in old revision ) is available to use as the input for AEM10941 Primary Battery input. See schematic for more detail
- PA15 or ADC5 in Arduino RUI3 is currently not working. This causes the battery measurement to not work as well. An alternative is to use Mbed OS.

## Reference
The project won't be possible without the amazing work from people across the globe. The following are the reference to those awesome projects:

 - [LoRa e5 Tiny](https://github.com/hallard/LoRa-E5-Tiny)
 - [AERQ - Air Quality Monitoring](https://www.seeedstudio.com/blog/2022/04/27/monitoring-indoor-air-pollutants-the-silent-issue-for-smart-city-iot-using-seeed-lora-e5-and-fusion-pcba/)
 - [TSEM](https://hackaday.io/project/159139-tiny-solar-energy-module-tsem)


## License
The product is open-source! However, some part of library used under **src**, might have it's own license.
Please reach out or create a ticket to report any license violation.

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
