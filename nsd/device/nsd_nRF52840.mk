# Source files common to nrf52840
NSD_SRC_FILES += \
  $(NSD_ROOT)/device/nRF52840/nsd_mnd_nrf52840.c \
  $(NSD_ROOT)/device/nRF52840/startup_nrf52840.c


# Include folders common to nrf52840
NSD_INC_FOLDERS += \
  $(NSD_ROOT)/device/nRF52840 \


# NSD_ROOT := ../../../../../../../NSD
# include $(NSD_ROOT)/nsd.makefile
# INC_FOLDERS += $(NSD_INC_FOLDERS)
# SRC_FILES += $(NSD_SRC_FILES)
