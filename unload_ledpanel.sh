#!/bin/sh
module="ledpanel"
device="ledpanel"

# invoke rmmod with all arguments we got
/sbin/rmmod $module $* || exit 1

# Remove stale nodes
rm -f /dev/${device}
