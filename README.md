# nRF Simple Drivers

Assumptions:<br>
 - Each driver only control peripheral. Do not have memory. <br>
 - GPIO pins are not configured by driver. Must be configured before using driver. <br>
 <br> <br>
How to use NSD with repository? <br>
Try to execute 'make configure' into bash shell then try to flash your board using 'make example=spim target=pca10056 flash'.<br>
To test nsd examples SDK should be decompress in env directory. It is important to configure arm gcc compiler (according to SDK documentation) and install nrfjprog.<br>

├── NSD<br>
│   ├── examples<br>
│   │   └── spim<br>
│   ├── env<br>
│   │   └── sdk<br>
│   ├── nsd<br>
│   └── scripts<br>
