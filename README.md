# xc3746_ledpanel_8x16_driver
XC3746 8x16 LED Panel GPIO Character Driver.

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

## Loading the Module
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
