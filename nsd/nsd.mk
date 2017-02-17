# Source files common to all targets
NSD_SRC_FILES += \
  $(NSD_ROOT)/nsd.c \
  $(NSD_ROOT)/drivers/spim/nsd_spim.c \
  $(NSD_ROOT)/drivers/uarte/nsd_uarte.c \


# Include folders common to all targets
NSD_INC_FOLDERS += \
  $(NSD_ROOT)/ \
  $(NSD_ROOT)/device/ \
  $(NSD_ROOT)/drivers/common \
  $(NSD_ROOT)/drivers/spim \
  $(NSD_ROOT)/drivers/uarte


# NSD_ROOT := ../../../../../../../NSD
# include $(NSD_ROOT)/nsd.makefile
# INC_FOLDERS += $(NSD_INC_FOLDERS)
# SRC_FILES += $(NSD_SRC_FILES)
