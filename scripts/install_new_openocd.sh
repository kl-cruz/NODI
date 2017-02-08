#!/bin/bash

cd tools
git clone http://repo.or.cz/openocd.git
cd openocd
./bootstrap
./configure
make
if [ ! -e "/etc/udev/rules.d/99-openocd.rules" ]; then
  echo "Copying udev rules for openocd. It requiers superuser privileges."
  sudo cp contrib/99-openocd.rules /etc/udev/rules.d/99-openocd.rules
  sudo udevadm control --reload-rules
fi
