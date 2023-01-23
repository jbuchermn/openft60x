#include <stdio.h>
#include <string.h>

#include <libusb-1.0/libusb.h>

/*
 * bmRequestType
 * C0 = Proprietary transfer device-to-host
 * 40 = Proprietary transfer host-to-device
 */

#define CHECK_RESULT(func, text) ({ int res; if(0 > (res = func)){ printf("Error during " #text  ": %d\n", res); goto cleanup; } res; })

int main(int argc, char* argv[]){
    struct libusb_device_handle* handle = NULL;

    CHECK_RESULT(libusb_init(NULL), "libusb_init");
    handle = libusb_open_device_with_vid_pid(NULL, 0x0403, 0x601E);
    if(!handle){
        printf("Error opening device\n");
        goto cleanup;
    }

    unsigned char control_data[152];
    memset(control_data, 0, 152);

    /********* during FT_Create */
    /* proprietary transfer 1 --> yields 09 01 00 00... = firmware version*/
    CHECK_RESULT(libusb_control_transfer(handle, 0xC0, 0xF1, 0, 0x0000, control_data, 4, 0), "PT1"); 
    printf("PT1 yields: \n");
    for(int i=0; i<4; i++) printf("%02X ", control_data[i]);
    printf("\n");

    /* proprietary transfer 2 --> yields 00 00 17 00... = ???*/
    CHECK_RESULT(libusb_control_transfer(handle, 0xC0, 0x03, 1, 0x8400, control_data, 4, 0), "PT2"); 
    printf("PT2 yields: \n");
    for(int i=0; i<4; i++) printf("%02X ", control_data[i]);
    printf("\n");

    /* proprietary transfer 3 --> yields something different each time, looks like --> 00 (reboot-counter) (03 or 23) 01*/
    CHECK_RESULT(libusb_control_transfer(handle, 0xC0, 0x03, 1, 0x8000, control_data, 4, 0), "PT3"); 
    printf("PT3 yields: \n");
    for(int i=0; i<4; i++) printf("%02X ", control_data[i]);
    printf("\n");

    /* proprietary transfer 4 --> second two bytes seem related to PT3, probably setting values PT3 returns, values appear to not be stored after reboot 0x03, 0x13, 0x23, 0x43 for first byte, initialised to 0x03 0x01, FT_Create sets it to 23 01, does not care about first two bytes */
    control_data[0] = 0x00;
    control_data[1] = 0x00;
    control_data[2] = 0x23;
    control_data[3] = 0x01;
    CHECK_RESULT(libusb_control_transfer(handle, 0x40, 0x03, 1, 0x8000, control_data, 4, 0), "PT4"); 

    /********* during FT_GetChipConfiguration */
    /* proprietary transfer - works even without PT1-4 */
    CHECK_RESULT(libusb_control_transfer(handle, 0xC0, 0xCF, 1, 0x0000, control_data, 152, 0), "PT-get_chip_configuration"); 
    printf("PT-get_chip_configuration yields: \n");
    for(int i=0; i<152; i++) printf("%02X ", control_data[i]);
    printf("\n");

    /********* during FT_SetChipConfiguration */
    /* proprietary transfer */
    /* CHECK_RESULT(libusb_control_transfer(handle, 0x40, 0xCF, 1, 0x0000, control_data, 152, 0), "PT-set_chip_configuration");  */

    /* claim interfaces */

    /********* during FT_WritePipe */
    /* one initial endpoint=1 of length 20 */
    /* 00 00 00 00 82 03 00... */

    /* at every packet: endpoint=1 of length 20 */
    /* 00 00 00 00 02 01 00 00 00 04 00... for 1024B */
    /* 00 00 00 00 02 01 00 00 00 08 00... for 2048B */
    /* 00 00 00 00 02 01 00 00 01 00 00... for 1B */
    /* at every packaet: endpoint=2 with nothing but the data */

    /********* during FT_ReadPipe */
    /* one initial endpoint=1 of length 20 */
    /* 00 00 00 00 82 03 00... */

    /* at every packet: endpoint=1 of length 20 */
    /* 00 00 00 00 82 01 00 00 00 02 00... for 512B */

cleanup:
    /* release interface */

    libusb_close(handle);
    libusb_exit(NULL);

    return 0;
}
