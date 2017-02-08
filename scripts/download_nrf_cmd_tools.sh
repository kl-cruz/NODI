#!/bin/bash

source ENV_VARS.sh
# get shorter zip file -> it means sdk, not its docs
if [ -d "env/nrf_cmd_tools" ]; then
  rm -r -f env/nrf_cmd_tools
fi
mkdir env
cd env
mkdir nrf_cmd_tools
cd nrf_cmd_tools
wget $NRF5X_CMD_TOOLS_LINK -O nrf_cmd_tools.tar
tar -xvf nrf_cmd_tools.tar
rm nrf_cmd_tools.tar
