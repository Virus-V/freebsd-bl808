APP=./coremark/coremark.exe

all: $(APP)

$(APP):
	$(MAKE) -C coremark coremark.exe

clean:
	$(MAKE) -C coremark clean

install: all
	mkdir -p $(INSTALL_BIN_PATH)
	install -Cs -o root -g wheel -m 555 $(APP) $(INSTALL_BIN_PATH)/$(notdir $(APP))

.PHONY: all install clean
