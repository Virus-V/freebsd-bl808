PWD:=$(shell pwd)
BOARD=Sipeed-M1s
DEFAULT_APPS=bsdinfo helloworld

all: crochet/work/env.sh $(addsuffix /install, $(addprefix apps/,$(DEFAULT_APPS)))
	@./crochet/crochet.sh -b $(BOARD)

INSTALL_LIBS_PATH:=$(PWD)/overlay/usr/local/lib
INSTALL_BIN_PATH:=$(PWD)/overlay/usr/local/bin

export INSTALL_BIN_PATH INSTALL_LIBS_PATH

crochet/work/env.sh:
	@./crochet/crochet.sh -b $(BOARD)

apps/%/install: crochet/work/env.sh
	@eval "app=$$(echo $@ | awk -F '/' '{print $$2}')" && \
	eval "env $$(cat crochet/work/env.sh) PATH=$(PATH) $(MAKE) -C apps/$${app} install"

apps/%/clean: FORCE
	@eval "app=$$(echo $@ | awk -F '/' '{print $$2}')" && \
	eval "env $$(cat crochet/work/env.sh) PATH=$(PATH) $(MAKE) -C apps/$${app} clean"

apps/%: crochet/work/env.sh FORCE
	@eval "app=$$(echo $@ | awk -F '/' '{print $$2}')" && \
	eval "env $$(cat crochet/work/env.sh) PATH=$(PATH) $(MAKE) -C apps/$${app}"

clean:
	chflags -R noschg crochet/work
	rm -rf crochet/work
	rm -rf opensbi/build opensbi/install
	rm -rf bl_mcu_sdk/build_* bl_mcu_sdk/out
	rm -rf out

FORCE:

.PHONY: clean all FORCE
