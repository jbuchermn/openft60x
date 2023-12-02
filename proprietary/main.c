#include <stdio.h>
#include <string.h>
#include "lib/ftd3xx.h"

int main(int argc, char* argv[]){
    /* get_version */
    unsigned int version;

	FT_GetDriverVersion(NULL, &version);
	printf("[MAIN] > Driver version: %d.%d.%d\n", version >> 24, (uint8_t)(version >> 16), version & 0xFFFF);

	FT_GetLibraryVersion(&version);
	printf("[MAIN] > Library version: %d.%d.%d\n", version >> 24, (uint8_t)(version >> 16), version & 0xFFFF);


    /* #<{(| device info - from manual |)}># */
    /* FT_STATUS ftStatus; */
    /* DWORD numDevs = 0; */
    /* ftStatus = FT_CreateDeviceInfoList(&numDevs); */
    /* if (!FT_FAILED(ftStatus) && numDevs > 0) */
    /* { */
    /*     FT_HANDLE ftHandle = NULL; */
    /*     DWORD Flags = 0; */
    /*     DWORD Type = 0; */
    /*     DWORD ID = 0; */
    /*     char SerialNumber[16] = { 0 }; */
    /*     char Description[32] = { 0 }; */
    /*     printf("[MAIN] List of Connected Devices:\n\n"); */
    /*     for (DWORD i = 0; i < numDevs; i++) */
    /*     { */
    /*         ftStatus = FT_GetDeviceInfoDetail(i, &Flags, &Type, &ID, NULL, */
    /*                 SerialNumber, Description, &ftHandle); */
    /*         if (!FT_FAILED(ftStatus)) */
    /*         { */
    /*             printf("[MAIN] Device[%d]\n", i); */
    /*             printf("[MAIN] \tFlags: 0x%x %s | Type: %d | ID: 0x%08X | ftHandle=0x%x\n", */
    /*                     Flags, */
    /*                     Flags & FT_FLAGS_SUPERSPEED ? "[USB 3]" : */
    /*                     Flags & FT_FLAGS_HISPEED ? "[USB 2]" : */
    /*                     Flags & FT_FLAGS_OPENED ? "[OPENED]" : "", */
    /*                     Type, */
    /*                     ID, */
    /*                     ftHandle); */
    /*             printf("[MAIN] \tSerialNumber=%s\n", SerialNumber); */
    /*             printf("[MAIN] \tDescription=%s\n", Description); */
    /*         } */
    /*     } */
    /* } */

    /* turn_off_thread_safe */
    /* printf("[MAIN] turn_off_thread_safe\n"); */
    /* FT_TRANSFER_CONF conf; */
	/* memset(&conf, 0, sizeof(FT_TRANSFER_CONF)); */
	/* conf.wStructSize = sizeof(FT_TRANSFER_CONF); */
	/* conf.pipe[FT_PIPE_DIR_IN].fNonThreadSafeTransfer = true; */
	/* conf.pipe[FT_PIPE_DIR_OUT].fNonThreadSafeTransfer = true; */
	/* for (int i = 0; i < 4; i++) */
	/* 	FT_SetTransferParams(&conf, i); */


    /* main */
    printf("[MAIN] FT_Create\n");
    FT_HANDLE handle = NULL;
	FT_Create(0, FT_OPEN_BY_INDEX, &handle);
    if (!handle) {
        printf("[MAIN] > Failed to create device\n");
        return -1;
    }

    /* FT_GetVIDPID(handle, NULL, NULL); */

    /* #<{(| get chip configuration |)}># */
    /* printf("[MAIN] FT_GetChipConfiguration\n"); */
    /* FT_60XCONFIGURATION configuration; */
    /* FT_GetChipConfiguration(handle, &configuration); */
    /* printf("[MAIN] %04X %04X: FIFO-Clock=%d FIFO-Mode=%d ChannelConfig=%d\n",  */
    /*         configuration.VendorID, configuration.ProductID, */
    /*         configuration.FIFOClock, configuration.FIFOMode, configuration.ChannelConfig); */

    /* #<{(| set chip configuration |)}># */
    /* configuration.FIFOMode = CONFIGURATION_FIFO_MODE_245; */
    /* configuration.ChannelConfig = CONFIGURATION_CHANNEL_CONFIG_1; */
    /*  */
    /* printf("[MAIN] Writing configuration\n"); */
    /* FT_SetChipConfiguration(handle, &configuration); */
    /* #<{(| Need to reopen handle! |)}># */
    /* return 0; */



    /* #<{(| get pipe information |)}># */
    /* FT_PIPE_INFORMATION info; */
    /* printf("[MAIN] FT_GetPipeInformation\n"); */
    /* for(int interface = 0; interface < 2; interface++){ */
    /*     for(int pipe = 0; pipe < 256; pipe++){ */
    /*         if(FT_OK != FT_GetPipeInformation(handle, interface, pipe, &info)) continue; */
    /*         printf("[MAIN] interface %d pipe %d has id=%02X, type=%d\n", interface, pipe, info.PipeId, info.PipeType); */
    /*     } */
    /* } */


    /* #<{(| write |)}># */
    /* { */
    /*     const unsigned long buffer_len = 512 + 2; */
    /*     unsigned char buffer[buffer_len]; */
    /*     for(int i=0 ; i<buffer_len; i++) buffer[i] = i % 256; */
    /*  */
    /*     #<{(| FT_FlushPipe(handle, 0x02); |)}># */
    /*     FT_SetStreamPipe(handle, 1, 0, 0x02, buffer_len); */
    /*  */
    /*     for (int i=0; i<10; i++) { */
    /*         unsigned int count = 0; */
    /*         int res; */
    /*         if (FT_OK != (res = FT_WritePipe(handle, 0x02, buffer, buffer_len, &count, 1000))) { */
    /*             printf("Error transmitting: %d %d\n", res, count); */
    /*         }else{ */
    /*             printf("Transmitted: %d\n", count); */
    /*         } */
    /*     } */
    /*  */
    /*     #<{(| printf("%d\n", FT_FlushPipe(handle, 0x02)); |)}># */
    /* } */

    /* read */
    {
        const unsigned long buffer_len = 32;
        unsigned char buffer[buffer_len];

        FT_SetStreamPipe(handle, 0, 1, 0x82, buffer_len);

        unsigned char counter = 255;
        int success_counter = 0;

        /* FT_FlushPipe(handle, 0x82); */
        for (int i=0; i<100; i++) {
            printf("---\n");
            unsigned int count = 0;
            int res;
            if (FT_OK != (res = FT_ReadPipeEx(handle, 0, buffer, buffer_len, &count, 1000))) {
                printf("E%d\n", res);
            }

            for(int i=0; i<count; i+=2){
                if(buffer[i] != (unsigned char)(counter+1)){
                    printf("S[%4d]: %02X-%02X(%d)\n", success_counter, counter, buffer[i], (buffer[i] - counter + 255)%256);
                    success_counter = 0;
                }
                else
                    success_counter++;
                counter = buffer[i];
            }
        }
    }

    /* get firmware version */
    /* printf("[MAIN] FT_GetFirmwareVersion\n"); */
    /* FT_GetFirmwareVersion(handle, &version); */
    /* printf("[MAIN] > FT_GetFirmwareVersion: %d\n", version); */

    /* unsigned int buffer[1000]; */
    /* FT_SetGPIOPull(handle, 255, 0); */


    /* close */
    printf("[MAIN] FT_Close\n");
    FT_Close(handle);


    return 0;
}
