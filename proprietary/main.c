#include <stdio.h>
#include <string.h>
#include "lib/ftd3xx.h"

int main(int argc, char* argv[]){
    /* get_version */
    unsigned int version;

	FT_GetDriverVersion(NULL, &version);
	printf("Driver version:%d.%d.%d\r\n", version >> 24, (uint8_t)(version >> 16), version & 0xFFFF);

	FT_GetLibraryVersion(&version);
	printf("Library version:%d.%d.%d\r\n", version >> 24, (uint8_t)(version >> 16), version & 0xFFFF);

    /* turn_off_thread_safe */
    FT_TRANSFER_CONF conf;
	memset(&conf, 0, sizeof(FT_TRANSFER_CONF));
	conf.wStructSize = sizeof(FT_TRANSFER_CONF);
	conf.pipe[FT_PIPE_DIR_IN].fNonThreadSafeTransfer = true;
	conf.pipe[FT_PIPE_DIR_OUT].fNonThreadSafeTransfer = true;
	for (int i = 0; i < 4; i++)
		FT_SetTransferParams(&conf, i);


    /* main */
    FT_HANDLE handle = NULL;
	FT_Create(0, FT_OPEN_BY_INDEX, &handle);
    if (!handle) {
        printf("Failed to create device\r\n");
        return -1;
    }
    FT_Close(handle);


    return 0;
}
