# Source files common to nrf52840
NSD_SRC_FILES += \
  $(NSD_ROOT)/device/nRF52840/startup_nrf52840.c\
  $(NSD_ROOT)/device/nRF52840/nsd_gpio_nrf52840.c \
  $(NSD_ROOT)/device/nRF52840/nsd_mnd_nrf52840.c \
  $(NSD_ROOT)/device/nRF52840/system_nrf52840.c


# Include folders common to nrf52840
NSD_INC_FOLDERS += \
  $(NSD_ROOT)/device/nRF52840

LINKFILE_COMMON := $(NSD_ROOT)/device/nRF52840/gcc

#default MCU flags

CFLAGS += -DNSD_CHIP_NRF52840
CFLAGS += -DNRF52840_XXAA

