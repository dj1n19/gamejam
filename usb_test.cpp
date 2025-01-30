#include <hidapi/hidapi.h>
#include <iostream>
#include <iomanip>
#include <vector>

#define VID 0x046d  // Vendor ID
#define PID 0xc218  // Product ID

int main() {
    if (hid_init()) {
        std::cerr << "Failed to initialize HIDAPI\n";
        return -1;
    }

    // Open the device using its VID and PID
    hid_device* handle = hid_open(VID, PID, nullptr);
    if (!handle) {
        std::cerr << "Unable to open HID device\n";
        return -1;
    }

    // Set the report size based on your gamepad's report size, typically 64 bytes
    unsigned char data[64];
    int res;

    // Example: Read the report (you may need to adjust the report number)
    while (true) {
        res = hid_read(handle, data, sizeof(data));
        if (res < 0) {
            std::cerr << "Error reading from device\n";
            break;
        }

        std::cout << "Data received: ";
        
        
        std::cout << std::hex << (int)data[4] << " ";
        std::cout << std::dec << std::endl;
    }

    // Close the device handle
    hid_close(handle);

    // Finalize HIDAPI
    hid_exit();
    return 0;
}

// up -> 0
// left -> 6
// right -> 2
// down -> 4
// interact -> 18

/*
    position xy
    fuel
    item
    base
*/

/*
    algorithm to be close or far from a item
    -> close value of 10
    -> far away value of 0.1
*/

/*
    -> position of player
    -> fuel
    -> distance to item
    -> in base
    -> score
*/