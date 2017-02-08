#!/bin/bash

ADDRESS_BASE="https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v12.x.x/"
# get shorter zip file -> it means sdk, not its docs
LINK=`curl https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v12.x.x/ | grep -o -e 'nRF5_SDK_.\{1,14\}\.zip' | tail -n1`
LINK=$ADDRESS_BASE$LINK
wget $LINK -O SDK.zip
mkdir env
if [ -d "env/sdk" ]; then
  rm -r -f env/sdk
fi
unzip SDK.zip -d env/sdk
rm SDK.zip
