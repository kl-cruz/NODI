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
	@echo "***********  NSD MAKEFILE HELP  **********"
	@echo ""
	@echo ""
	@echo "USAGE:"
	@echo "make help                                        Show this help"
	@echo "make configure                                   Configure environement for fixture generators"
	@echo "make distclean                                   Deconfigure environement, remove additional and builds files"
	@echo "make target=... clean_build                      Clean all compile files with target build directory"
	@echo "make target=... all                              Simply compile test for target using available files"

clean_build: clean
	@echo "Cleaning build $(target)"
	@rm -r -f build_$(target)
	@rm -r -f app/.dep
	@rm -r -f .dep

configure:
	@echo "Configure environement..."
	@if test -d env; then echo "env directory exist!"; else mkdir env; fi
	@echo "Setting env vonfig variables..."
	@./scripts/ENV_VARS.sh
	@echo "Downloading nRF cmd tools..."
	@./scripts/download_nrf_cmd_tools.sh
	@echo "Done!"

distclean:
	@echo "Deconfigure environement..."
	@rm -r -f env
	@echo "Done!"

all:
	@echo "Compiling example..."
	@cd examples/$(example)/$(target) && make clean && make all
	@echo "Done!"

flash:
	@echo "Compiling example..."
	@cd examples/$(example)/$(target) && make clean && make flash
	@echo "Done!"
