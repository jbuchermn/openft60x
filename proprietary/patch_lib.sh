#!/bin/sh
cd libusb
make
cp ./libusb/.libs/libusb-1.0.a ../lib
cd ..

cd lib

mkdir -p libusb
cd libusb
ar x ../libusb-1.0.a
cp ../../libusb/libusb/os/.libs/libusb_1_0_la-linux*.o ./
mv libusb_1_0_la-hotplug.o libusb_1_0_la-libusb_hotplug.o # hotplug.o needs to be preserved
cd ..


mkdir -p libftd3xx-patched
cd libftd3xx-patched
ar x ../libftd3xx-static.a
cd ..

cd libusb
for filename in *.o; do
    new_filename=$(sed -E 's/^libusb_1_0_la-//' <<< "$filename")
    cp "$filename" "../libftd3xx-patched/$new_filename"
done
cd ..

ar r libftd3xx-patched-static.a libftd3xx-patched/*.o
