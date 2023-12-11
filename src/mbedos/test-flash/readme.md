
# BSEC BME688 Test Example

Tested on:

 - Compiler ARMC6
   - With/without sleep trace enabled 
 - [BSEC2 Library](https://www.bosch-sensortec.com/software-tools/software/bme688-software/)

## Acknowledgement
Source code is based on https://github.com/Mircerson/AERQ

## TODO

- [ ] Update BSEC library to use the latest version
- [ ] Provide BSEC static library
  - `libalgobsec.a` for GCC_ARM
  - `libalgobsec.ar` for ARMC6 ( currently rename the .lib file to .ar from the BSEC library)
  - STM32WL55CC is Cortex-M4
- [ ] Implement simple AQI reader without proper sleep
- [ ] Rewrite BME688 high-level library
- [ ] Create a different repo for proper AQI reader with sleep
