# Source files common to nrf52840
NODI_SRC_FILES += \
  $(NODI_ROOT)/device/nRF52840/gcc/gcc_startup_nrf52840.S\
  $(NODI_ROOT)/device/nRF52840/nodi_gpio_nrf52840.c \
  $(NODI_ROOT)/device/nRF52840/nodi_mnd_nrf52840.c \
  $(NODI_ROOT)/device/nRF52840/system_nrf52840.c \
  $(NODI_ROOT)/drivers/pwr_clk/nodi_pwr_clk.c \
  $(NODI_ROOT)/drivers/rtc/nodi_rtc.c \
  $(NODI_ROOT)/drivers/spim/nodi_spim.c \
  $(NODI_ROOT)/drivers/uarte/nodi_uarte.c


# Include folders common to nrf52840
NODI_INC_FOLDERS += \
  $(NODI_ROOT)/device/nRF52840 \
  $(NODI_ROOT)/drivers/pwr_clk \
  $(NODI_ROOT)/drivers/rtc \
  $(NODI_ROOT)/drivers/spim \
  $(NODI_ROOT)/drivers/uarte

LINKFILE_COMMON := $(NODI_ROOT)/device/nRF52840/gcc

#default MCU flags

CFLAGS += -DNODI_CHIP_NRF52840
CFLAGS += -DNRF52840_XXAA

