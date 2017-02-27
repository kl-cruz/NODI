# nRF Opensource Drivers Implementation

Assumptions:<br>
 - Each driver only control peripheral. Do not have memory. <br>
 - GPIO pins are not configured by driver. Must be configured before using driver. <br>
 - Only assert guards against bad states. <br>
