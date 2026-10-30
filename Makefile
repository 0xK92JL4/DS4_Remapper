BIN_DIR   := bin
SRC_DIR   := src
OBJ_DIR   := .obj

CXX      := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -MMD -MP -Iinclude $(shell pkg-config --cflags libevdev)
LDFLAGS  := $(shell pkg-config --libs libevdev)

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

TARGET := $(BIN_DIR)/ds4-remapper

PREFIX           ?= /usr/local
BINDIR           ?= $(PREFIX)/bin
UDEV_RULES_DIR   ?= /etc/udev/rules.d
MODULES_LOAD_DIR ?= /etc/modules-load.d
SYSTEMD_USER_DIR ?= /usr/lib/systemd/user

all: $(TARGET)


$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking $@"
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@


-include $(OBJS:.o=.d)

clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

re: clean all

run: all
	./$(TARGET)


install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)$(BINDIR)/ds4-remapper
	install -Dm644 packaging/70-ds4-remapper.rules $(DESTDIR)$(UDEV_RULES_DIR)/70-ds4-remapper.rules
	install -Dm644 packaging/ds4-remapper.conf $(DESTDIR)$(MODULES_LOAD_DIR)/ds4-remapper.conf
	install -d $(DESTDIR)$(SYSTEMD_USER_DIR)
	sed 's|@BINDIR@|$(BINDIR)|' packaging/ds4-remapper.service > $(DESTDIR)$(SYSTEMD_USER_DIR)/ds4-remapper.service
ifeq ($(DESTDIR),)
	-modprobe uinput
	udevadm control --reload-rules
	udevadm trigger --action=add --subsystem-match=input --subsystem-match=leds --subsystem-match=misc
	systemctl --global enable ds4-remapper.service
	@echo
	@echo "Installed. Start it now with:  systemctl --user start ds4-remapper"
	@echo "(it will start by itself at your next login)"
endif

uninstall:
ifeq ($(DESTDIR),)
	-systemctl --global disable ds4-remapper.service
endif
	rm -f $(DESTDIR)$(BINDIR)/ds4-remapper
	rm -f $(DESTDIR)$(UDEV_RULES_DIR)/70-ds4-remapper.rules
	rm -f $(DESTDIR)$(MODULES_LOAD_DIR)/ds4-remapper.conf
	rm -f $(DESTDIR)$(SYSTEMD_USER_DIR)/ds4-remapper.service
ifeq ($(DESTDIR),)
	udevadm control --reload-rules
endif


.PHONY: all clean re run install uninstall
