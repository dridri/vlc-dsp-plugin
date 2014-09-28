PREFIX = /usr
LD = ld
CC = cc
INSTALL = install
CFLAGS = -g3 -Wall -Wextra
LDFLAGS =
VLC_PLUGIN_CFLAGS := $(shell pkg-config --cflags vlc-plugin)
VLC_PLUGIN_LIBS := $(shell pkg-config --libs vlc-plugin)

libdir = $(PREFIX)/lib
plugindir = $(libdir)/vlc/plugins

override CC += -std=gnu99
override CPPFLAGS += -DPIC -I. -Isrc -g3
override CFLAGS += -fPIC -g3
override LDFLAGS += -Wl,-no-undefined,-z,defs

override CPPFLAGS += -DMODULE_STRING=\"dsp\"
override CFLAGS += $(VLC_PLUGIN_CFLAGS)

TARGETS = libdsp_plugin.so

all: libdsp_plugin.so

install: all
	mkdir -p -- $(DESTDIR)$(plugindir)/audio_filter
	$(INSTALL) --mode 0755 libdsp_plugin.so $(DESTDIR)$(plugindir)/audio_filter

install-strip:
	$(MAKE) install INSTALL="$(INSTALL) -s"

uninstall:
	rm -f $(plugindir)/audio_filter/libdsp_plugin.so

clean:
	rm -f -- libdsp_plugin.so *.o

mostlyclean: clean

SOURCES = dsp.c

$(SOURCES:%.c=%.o):

libdsp_plugin.so: $(SOURCES:%.c=%.o)
	$(CC) $(LDFLAGS) -shared -o $@ $^ $(VLC_PLUGIN_LIBS) -pthread -lm

.PHONY: all install install-strip uninstall clean mostlyclean
