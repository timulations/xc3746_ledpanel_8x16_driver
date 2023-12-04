#!/bin/sh
module="ledpanel"
device="ledpanel"
mode="664"
group="sudo"

/sbin/insmod ./$module.ko $* || exit 1

# retrieve major number
major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

# delete node and re-add
rm -f /dev/${device}
mknod /dev/${device} c $major 0
chgrp $group /dev/${device}
chmod $mode  /dev/${device}
