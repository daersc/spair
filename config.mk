# spair - slim chess pairing tool
# see LICENSE file for license and copyright information

# spair version
VERSION = "0.0.1"

# directories
PREFIX    = /usr/local
LOCALEDIR = $(PREFIX)/share/locale

# compiler and linker flags
CPPFLAGS = -D_POSIX_C_SOURCE=200809L
CFLAGS   = -std=c99 -pedantic -Wall
LDFLAGS  =
LDLIBS   =

# compilers
CC = cc

# Gettext support
USING_GETTEXT = 1
ifeq ($(USING_GETTEXT),1)
	#LDLIBS += -lintl
endif
