#!/bin/bash

function sdk_remove {
  if [ -d $1 ]; then
    rm -r -f $1
  fi
}

sdk_remove external

