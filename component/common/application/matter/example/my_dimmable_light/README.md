# my_dimmable_light-app Example
This example is an implementation of the *Dimmable Light* device type. You will need an LED.

| Peripheral | Pin |
| ----------- | ----------- |
| LED | PA_23 |

## How it works
The LED can be controlled by the Matter controller.
Thus, only 1 stream of communication (FreeRTOS Queues) is needed to control the LED:
  1. Uplink - Matter controller changes the On/Off attribute of the LED -> callback triggers and invokes the LED driver to toggle the LED

### Peripheral Initialization
Initialization of the LED is handled in `matter_drivers.cpp`.

### Matter Attribute Change Callback
Whenever the Matter controller changes the On/Off attribute of the LED, 2 types of callbacks will be invoked:
  1. MatterPreAttributeChangeCallback - Toggle the LED before updating the On/Off attribute (TBD)
  2. MatterPostAttributeChangeCallback - Toggle the LED after updating the On/Off attribute

These callbacks are defined in `core/matter_interaction.cpp`.
These callbacks will post an event to the uplink queue, which will be handled by `matter_driver_uplink_update_handler` in `matter_drivers.cpp`.
The driver codes will be called to carry out your actions (On/Off LED in this case) depending on the Cluster and Attribute ID received.
You may add clusters and attributes handling in `matter_driver_uplink_update_handler` if they are not present. 

## How to build

### Configurations
Enable `CONFIG_EXAMPLE_MATTER` and `CONFIG_EXAMPLE_MATTER_MY_DIMMABLE_LIGHT` in `platform_opts_matter.h`.
Ensure that `CONFIG_EXAMPLE_MATTER_CHIPTEST` is disabled.

### Setup the Build Environment
  
    cd connectedhomeip
    source scripts/activate.sh
  
### Build Matter Libraries

    cd ambz2_matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/
    make my_dimmable_light
    
### Build the Final Firmware

    cd ambz2_matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/
    make is_matter
    
### Flash the Image
Refer to this [guide](https://github.com/ambiot/ambz2_matter/blob/main/tools/AmebaZ2/Image_Tool_Linux/README.md) to flash the image with the Linux Image Tool

### Clean Matter Libraries

    cd ambz2_matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/
    make clean_matter_libs

## Clean Ameba Matter application

    cd ambz2_matter/project/realtek_amebaz2_v0_example/GCC-RELEASE/
    make clean_matter

## Using CHIP-Tool to control the LED

### Use AT commands to connect device to WIFI network
1. `ATW0=<WIFI_SSID>`
2. `ATW1=<WIFI_PASSWORD>`
3. `ATWC`

### Onnetwork
- Device already connected to WIFI network

#### Using setup code
```bash
./chip-tool pairing onnetwork 0x1234 20202021
```

#### Using setup code and discriminator
```bash
./chip-tool pairing onnetwork-long 0x1234 20202021 3840
```

#### Using QR code
```bash
./chip-tool pairing code 0x1234 MT:-24J042C00KA0648G00
```

### OnOff Cluster

#### On
```bash
./chip-tool onoff on 0x1234 1
```
#### Off
```bash
./chip-tool onoff off 0x1234 1
```
#### Toggle
```bash
./chip-tool onoff toggle 0x1234 1
```

### LevelControl Cluster

#### Read
```bash
./chip-tool levelcontrol read current-level 0x1234 1
```
#### move-to-level
- This will set the brightness level of the LED.
```bash
./chip-tool levelcontrol move-to-level 254 0 0 0 0x1234 1
```
