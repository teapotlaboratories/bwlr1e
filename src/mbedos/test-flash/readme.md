# RAK3172 Flash IAP Example

Uses the internal flash as user data. This example uses first `245760` bytes as application and remaining `16384` bytes as user data that user can access. Some details about the size as follow:
- Sector size: `2048` bytes
- Application size: `sector size * 120` = `2048 * 120` = `245760` bytes 
- User data size: remaining flash space = `2048 * 8` = `16384` bytes 

Below configuration needs to be enabled to limit the application size to specific size. Under `mbed_app.json` set the following:

    "target_overrides": {
        "*": {
        "target.features_add": ["STORAGE"],
        "target.components_add": ["FLASH", "FLASHIAP"],
        "target.restrict_size": "0x3C000"
        ...
        ...
        }
    }

`"target.features_add": ["STORAGE"]` and `"target.components_add": ["FLASH", "FLASHIAP"]` might be optional. 
`"target.restrict_size": "0x3C000"` limit the application size to `245760` bytes

## Reference

The following are some good reference:
- https://tronche.com/blog/2020/03/mbed-flashiap-tdbstore-and-stm32f4-internal-flash/
- https://os.mbed.com/docs/mbed-os/v6.16/program-setup/creating-and-using-a-bootloader.html
- https://forums.mbed.com/t/how-can-i-provision-a-persistent-array-of-int-in-flash-memory-and-access-it-as-an-array-of-int-from-mbed-c/6927/8
- https://forums.mbed.com/t/saving-config-data-with-flashiap-on-stm32l476rg-isnt-persisting-the-data/14721