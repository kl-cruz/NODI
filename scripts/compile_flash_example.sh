#!/bin/bash

echo $(pwd)
export PATH="$(pwd)/env/nrf_cmd_tools/nrfjprog:$PATH"
cd examples/$1/$2
make flash
