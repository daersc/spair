# spair - slim chess pairing tool
# see LICENSE file for license and copyright information
#
# This is the Makefile for spair. It defines the following targets.
#   compile (default)   Compile sources and link to spair binary
#   options             Print the used compiler and linker options
#   locale              Generate updated MO files
#   install             Install spair to PREFIX directory
#   uninstall           Remove installed files from PREFIX directory
#   dist                Create tarball containing all important files
#   clean               Clean all non-necessary output files

include config.mk

SRCS = spair.c types.c roundrobin.c
HDRS =         types.h roundrobin.h gettext.h
OBJS = $(SRCS:.c=.o)

POTSRC  = spair.pot
POSRCS  = de.po
MOOBJS  = $(POSRCS:.po=.mo)
LOCALES = $(POSRCS:.po=)

DEP = .depend.mk

CPPFLAGS += -DVERSION=\"$(VERSION)\"

ifeq ($(USING_GETTEXT),1)
CPPFLAGS += -DUSING_GETTEXT
CPPFLAGS += -DLOCALEDIR=\"$(LOCALEDIR)\"
endif


PRINTSTEP = printf " %-10s  %s\n"


.PHONY: compile options locale install uninstall dist clean

compile: spair

options:
	@$(PRINTSTEP) CPPFLAGS "$(CPPFLAGS)"
	@$(PRINTSTEP) CFLAGS   "$(CFLAGS)"
	@$(PRINTSTEP) LDFLAGS  "$(LDFLAGS)"
	@$(PRINTSTEP) LDLIBS   "$(LDLIBS)"

spair: $(OBJS) config.mk Makefile
	@$(PRINTSTEP) "CC -o" $@
	@$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

%.o: %.c config.mk Makefile $(DEP)
	@$(PRINTSTEP) "CC -c" $<
	@$(CC) -c $< $(CPPFLAGS) $(CFLAGS)

$(DEP): $(SRCS)
	@$(CC) -MM $(SRCS) > $@

-include $(DEP)

locale: $(MOOBJS)

%.mo: %.po
	@$(PRINTSTEP) MSGFMT $@
	@msgfmt -o $@ $<

%.po: $(POTSRC)
	@$(PRINTSTEP) MSGMERGE $@
	@msgmerge -U $@ $<
	@touch $@

$(POTSRC): $(SRCS)
	@$(PRINTSTEP) XGETTEXT $@
	@xgettext -o $@ -k_ $(SRCS)

install: compile
	@$(PRINTSTEP) INSTALL $(DESTDIR)$(PREFIX)
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	@mkdir -p $(DESTDIR)$(PREFIX)/share/doc/spair
	@cp -f spair $(DESTDIR)$(PREFIX)/bin
	@sed "s/VERSION/$(VERSION)/g" < spair.1 > $(DESTDIR)$(MANPREFIX)/man1/spair.1
	@cp -f README $(DESTDIR)$(PREFIX)/share/doc/spair
	@cp -f LICENSE $(DESTDIR)$(PREFIX)/share/doc/spair
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/spair
	@chmod 644 $(DESTDIR)$(MANPREFIX)/man1/spair.1
	@chmod 644 $(DESTDIR)$(PREFIX)/share/doc/spair/README
	@chmod 644 $(DESTDIR)$(PREFIX)/share/doc/spair/LICENSE
	@if (( $(USING_GETTEXT) )); then \
		for loc in $(LOCALES) ; \
		do \
			mkdir -p ${DESTDIR}${LOCALEDIR}/$$loc/LC_MESSAGES ; \
			cp -f $$loc.mo ${DESTDIR}${LOCALEDIR}/$$loc/LC_MESSAGES/spair.mo ; \
			chmod 644 ${DESTDIR}${LOCALEDIR}/$$loc/LC_MESSAGES/spair.mo ; \
		done; \
	fi

uninstall:
	@$(PRINTSTEP) UNINSTALL $(DESTDIR)$(PREFIX)
	@rm -f $(DESTDIR)$(PREFIX)/bin/spair
	@rm -f $(DESTDIR)$(MANPREFIX)/man1/spair.1
	@rm -f $(DESTDIR)$(PREFIX)/share/doc/spair/README
	@rm -f $(DESTDIR)$(PREFIX)/share/doc/spair/LICENSE
	@if (( $(USING_GETTEXT) )); then \
		for loc in $(LOCALES) ; \
		do \
			rm -f  ${DESTDIR}${LOCALEDIR}/$$loc/LC_MESSAGES/spair.mo ; \
		done \
	fi

dist: clean
	@$(PRINTSTEP) TAR spair-$(VERSION).tar.gz
	@mkdir -p spair-$(VERSION)
	@cp config.mk Makefile $(SRCS) $(POTSRC) $(POSRCS) $(MOOBJS) $(HDRS) \
		LICENSE README spair.1 spair-$(VERSION)
	@tar cf spair-$(VERSION).tar spair-$(VERSION)
	@gzip spair-$(VERSION).tar
	@rm -rf spair-$(VERSION)

clean:
	@$(PRINTSTEP) CLEAN
	@rm -f spair $(OBJS) $(DEP) spair-$(VERSION).tar.gz
