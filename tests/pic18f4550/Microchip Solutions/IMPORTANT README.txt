PIC USB Benchmark Firmware

DIRECTORY TREE:
 Device_Benchmark  - PIC USB Benchmark Firmware
 Microchip         - Original MCP USB Stack 2.6a files used by the benchmark
                     firmware. 

REQUIRED:
 * MCP USB Stack 2.6a
 * Microchip PIC USB Demo/Test Board

GETTING STARTED:
 * Move the "Device_Benchmark" folder to your 
   'Microchip Application Libraries' installation directory. This is normally 
   "C:\Microchip Solutions".
 
 * This firmware can use either libusb or winusb for a device driver.
 
 * By default, the VID/PID is set to 0x04D8 / 0x0053.  This is the same as the
   'WinUSB - High Bandwidth Demo'. It can be changed in "usb_descriptors.c".
   
FINDING THE DRIVERS:
 The require drivers have already been provided with the MCP usb stack.  The 
 benchmark firmware can use either WinUSB or libusb-win32 as its device
 driver.
 
USING WINUSB  -  Set the Product ID in "usb_descriptors.c" to 0x0053.
 Use drivers from:
 "USB Device - WinUSB - High Bandwidth Demo\Driver and INF"
                  
USING LIBUSB  -  Set the Product ID in "usb_descriptors.c" to 0x0204.
 Use drivers from:
 "USB Device - LibUSB - Generic Driver Demo\Windows Application\Driver and INF"
 