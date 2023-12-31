# BSEC BME688 Test Example

Tested on:

 - Compiler ARMC6
   - With/without sleep trace enabled 
 - [BSEC2 Library](https://www.bosch-sensortec.com/software-tools/software/bme688-software/)

## Acknowledgement
Source code is based on https://github.com/boschsensortec/Bosch-BSEC2-Library

## TODO

- [x] Update BSEC library to use the latest version
- [x] Provide BSEC static library
  - `libalgobsec.a` for GCC_ARM
  - `libalgobsec.ar` for ARMC6 ( currently rename the .lib file to .ar from the BSEC library)
  - STM32WL55CC is Cortex-M4
- [x] Implement simple AQI reader without proper sleep
- [x] Rewrite BME688 high-level library
- [x] Implement proper AQI Reader in ULP mode with Deep-Sleep (STOP2)

## Notes

If using RAK3172, then set the following under `mbed_app.json`
  
    "RAK3172": {
        "stm32wl-lora-driver.rf_switch_config": 2,
        "stm32wl-lora-driver.crystal_select" : 0
    }

If using RAK3172-T ( with TCXO ), then set the following under `mbed_app.json`

    "RAK3172": {
        "stm32wl-lora-driver.rf_switch_config": 2,
        "stm32wl-lora-driver.crystal_select" : 1
    }