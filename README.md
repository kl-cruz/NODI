# nRF Simple Drivers

Assumptions:<br>
 - Each driver only control peripheral. Do not have memory. <br>
 - GPIO pins are not configured by driver. Must be configured before using driver. <br>
 - Only assert guards against bad states. <br>
 <br> <br>
How to use NSD with repository? <br>
Try to execute 'make configure' into bash shell then try to flash your board using 'make example=spim target=pca10056 flash'.<br>
