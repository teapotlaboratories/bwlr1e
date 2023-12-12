#include "mbed.h"

// this example define the remaining flash area as user data
#define FLASH_USER_DATA_ADDR POST_APPLICATION_ADDR
#define FLASH_USER_DATA_SIZE POST_APPLICATION_SIZE

Thread thread;
FlashIAP flash;

mbed::DigitalOut led0(PA_1);
mbed::DigitalOut led1(PA_15);

int main()
{
    led0 = 0;
    led1 = 0;

    flash.init();

    // print flash information
    uint32_t flash_start = flash.get_flash_start();
    uint32_t flash_size = flash.get_flash_size();
    uint32_t page_size = flash.get_page_size();
    printf("Flash start: 0x%x \r\n", flash_start);
    printf("Flash size: %d bytes\r\n", flash_size);
    printf("Page size: %d bytes\r\n", page_size);
    printf("Sector size: %d bytes\r\n", flash.get_sector_size(flash_start));
    printf("App start address: 0x%x\r\n", APPLICATION_ADDR);
    printf("App size: %d bytes\r\n", APPLICATION_SIZE);
    printf("Post App start address: 0x%x\r\n", POST_APPLICATION_ADDR);
    printf("Post App size: %d bytes\r\n", POST_APPLICATION_SIZE);
    // see: https://os.mbed.com/docs/mbed-os/v6.16/program-setup/bootloader-configuration.html
    // for information on application address and size

    int result;
    const char header_magic[] = "USRDAT1";
    uint8_t buf_read[1024];
    
    // read user-data area
    result = flash.read( buf_read, FLASH_USER_DATA_ADDR, sizeof(header_magic) );
    if ( result != 0) 
    {
        printf("failed to read user-data memory\r\n");
    } 
    else
    {
        printf("successfully read user-data memory\r\n");
    }

    // check if magic header exist in the flash
    result = -1;
    for( int i =0; i<(sizeof(header_magic)) ; i++)
    {
        if( buf_read[i] != header_magic[i] )
        {
            result = -1;
            break;
        }
    }
    if ( result != 0) 
    {
        printf("magic header not found\r\n");
    } 
    else
    {
        printf("magic header found\r\n");
        printf("got %s\r\n", buf_read);
    }

    // if magic header not found, then write it
    if ( result != 0 )
    {
        // erase all user-data
        result = flash.erase(FLASH_USER_DATA_ADDR, FLASH_USER_DATA_SIZE);
        if ( result != 0) 
        {
            printf("failed to erase user-data memory\r\n");
        } 
        else
        {
            printf("successfully erase user-data memory\r\n");
        }
        
        // write data to user-data area
        result = flash.program( header_magic, FLASH_USER_DATA_ADDR, sizeof(header_magic) );
        if ( result != 0) 
        {
            printf("failed to program user-data memory\r\n");
        } 
        else
        {
            printf("successfully program user-data memory\r\n");
        }
    }

    flash.deinit();

    // sleep to inifinity
    while ( 1 )
    {
        ThisThread::sleep_for(5000ms);
    }
}


