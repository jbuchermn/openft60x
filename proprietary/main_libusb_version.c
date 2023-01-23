/*
 * gcc -c main_libusb_version.c -o main_libusb_version.o
 * gcc main_libusb_version.o lib/libftd3xx-static.a -o main_libusb_version
 * ./main_libusb_version
 *
 * >--- FT60x ---
 * > libusb version linked into libftd3xx-static.a: 1.0.22.11312 (http://libusb.info)
 *
 */

#include <stdio.h>
#include "libusb_version.h"

int main(int argc, char* argv[]){
    const struct libusb_version* v = libusb_get_version();

    printf("--- FT60x ---\n");
    printf("libusb version linked into libftd3xx-static.a: %d.%d.%d.%d%s (%s)\n", v->major, v->minor, v->micro, v->nano, v->rc, v->describe);

    return 0;
}
