# spair - slim chess pairing tool
# see LICENSE file for license and copyright information
#
# This is the Makefile for spair. It defines the following targets.
#   compile (default)   Compile sources and link to spair binary
#   options             Print the used compiler and linker options
#   install             Install spair to PREFIX directory
#   uninstall           Remove installed files from PREFIX directory
#   dist                Create tarball containing all important files
#   clean               Clean all non-necessary output files

include config.mk

SRCS = spair.c
OBJS = $(SRCS:.c=.o)

DEP = .depend.mk


PRINTSTEP = printf " %-10s  %s\n"


.PHONY: compile options install uninstall dist clean

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
	@$(CC) -MM $(SRCS) -MF $@

-include $(DEP)

install: compile
	@$(PRINTSTEP) INSTALL $(DESTDIR)$(PREFIX)
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@mkdir -p $(DESTDIR)$(PREFIX)/share/doc/spair
	@cp -f spair $(DESTDIR)$(PREFIX)/bin
	@cp -f README $(DESTDIR)$(PREFIX)/share/doc/spair
	@cp -f LICENSE $(DESTDIR)$(PREFIX)/share/doc/spair
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/spair
	@chmod 644 $(DESTDIR)$(PREFIX)/share/doc/spair/README
	@chmod 644 $(DESTDIR)$(PREFIX)/share/doc/spair/LICENSE

uninstall:
	@$(PRINTSTEP) UNINSTALL $(DESTDIR)$(PREFIX)
	@rm -f $(DESTDIR)$(PREFIX)/bin/spair
	@rm -f $(DESTDIR)$(PREFIX)/share/doc/spair/README
	@rm -f $(DESTDIR)$(PREFIX)/share/doc/spair/LICENSE

dist: clean
	@$(PRINTSTEP) TAR spair-$(VERSION).tar.gz
	@mkdir -p spair-$(VERSION)
	@cp config.mk Makefile $(SRCS) LICENSE README spair-$(VERSION)
	@tar cf spair-$(VERSION).tar spair-$(VERSION)
	@gzip spair-$(VERSION).tar
	@rm -rf spair-$(VERSION)

clean:
	@$(PRINTSTEP) CLEAN
	@rm -f spair $(OBJS) $(DEP) spair-$(VERSION).tar.gz
