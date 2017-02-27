# All configuration
ifeq ($(MAKECMDGOALS),)
ifeq ($(example),)
    $(error Please specify example! make example=... all)
endif

ifeq ($(target),)
    $(error Please specify target! make target=... all)
else
    BUILDDIR = build_$(target)
    FWINC += targets/$(target)
endif
endif

help:
	@echo "***********  NODI MAKEFILE HELP  **********"
	@echo ""
	@echo ""
	@echo "USAGE:"
	@echo "make help                                        Show this help"
	@echo "make configure                                   Configure environement for fixture generators"
	@echo "make distclean                                   Deconfigure environement, remove additional and builds files"
	@echo "make target=... clean_build                      Clean all compile files with target build directory"
	@echo "make target=... all                              Simply compile test for target using available files"

configure:
	@echo "Configure environement..."
	@if test -d env; then echo "env directory exist!"; else mkdir env; fi
	@echo "Setting env config variables... - EXPERIMENTAL"
	@./scripts/ENV_VARS.sh
	@echo "Downloading nRF cmd tools..."
	@./scripts/download_nrf_cmd_tools.sh
	@echo "Installing newest gcc toolchain..."
	@./scripts/install_new_gcc.sh
	@echo "Done!"

distclean:
	@echo "Deconfigure environement..."
	@git clean -xdf
	@echo "Done!"

all:
	@echo "Compiling example..."
	@cd examples/$(example)/$(target) && make clean && make all
	@echo "Done!"

flash: all
	@echo "Compiling and flashing example..."
	@./scripts/compile_flash_example.sh $(example) $(target)
	@echo "Done!"
