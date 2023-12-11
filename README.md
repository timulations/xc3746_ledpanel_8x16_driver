# XC3746 Ledpanel 8x16 Linux Driver (Duinotech Compatible)
XC3746 8x16 LED Panel GPIO Character Linux Driver, designed to be run on Raspberry Pi.
https://www.jaycar.com.au/duinotech-arduino-compatible-8-x-16-led-matrix-display/p/XC3746

The device driver is a very simple character driver that only supports writing 16 bytes at a time, with the 16 bytes representing the entire screen. The 'clever' stuff is done in user space. 

![ledpanel_photo](https://github.com/timulations/xc3746_ledpanel_8x16_driver/assets/108556839/0be8a4e6-bbd3-460a-be80-19f3d0ff9546)


## Getting Started
Install the Linux Kernel headers:

### Raspbian
```
sudo apt install raspberrypi-kernel-headers
```

### Ubuntu
```
sudo apt install linux-headers-$(uname -r)
```

## Building the Kernel Module
```
cd xc3746_ledpanel_8x16_driver
sudo make
```

## Wire up the System
The LED Panel uses I2C communication with the Pi.
```
            GND   ------------  Ground (any)
LED Panel   VCC   ------------  5V Power         Raspberry Pi 4B
            SDA   ------------  GPIO 2 (SDA) 
            SCL   ------------  GPIO 3 (SCL)

```

## Loading the Module
Modify the script as to specify which GPIO pin is your SDA (`sda_gpio` param) and SCL (`scl_gpio`). The kernel module has been set with Raspberry Pi 4B defaults.
```
sudo ./load_ledpanel.sh

# check it's loaded:
ls /dev | grep ledpanel # check this exists
cat /proc/devices       # check 'ledpanel' exists under character devices
```

## Unloading the Module
```
sudo ./unload_ledpanel.sh
```

## Rebuild + Reload the Module
```
sudo ./reload.sh
```


## Userland Testing
```
cd userland
make
./ledpanel_write
```
