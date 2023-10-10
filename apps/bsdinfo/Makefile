APP=bsdinfo

all: $(APP)

$(APP): bsdinfo-xterm-256.c
	$(CC) $^ -lkvm -lc -o $@

clean:
	rm $(APP)

install: all
	mkdir -p $(INSTALL_BIN_PATH)
	install -Cs -o root -g wheel -m 555 $(APP) $(INSTALL_BIN_PATH)/$(APP)

.PHONY: all install clean
