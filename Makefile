ifneq "$(MAKECMDGOALS)" "autoconf" 
include config/Makefile.include
endif

all::
	@cd src ; $(MAKE) all ; cd ..

install:: install-exec install-includes install-docs

install-exec:
	@cd src ; $(MAKE) install ; cd ..
	$(INSTALL) -d $(bindir)
	$(INSTALL) --mode=755 $(vigra_builddir)/config/vigra-config $(bindir)

install-includes:
	if test $(includedir) != "$(vigra_builddir)/include" ; then \
          $(INSTALL) -d $(includedir)/vigra ; \
          $(INSTALL) --mode=644 $(vigra_builddir)/include/vigra/*.hxx $(includedir)/vigra ; \
          $(INSTALL) --mode=644 $(vigra_builddir)/include/vigra/*.h $(includedir)/vigra ; \
        fi

install-docs:
	$(INSTALL) --mode=644 LICENSE $(docdir)
	if test $(prefix) != $(vigra_builddir) ; then \
          $(INSTALL) -d $(docdir)/documents ; \
          $(INSTALL) --mode=644 \
            $(vigra_builddir)/doc/*.html \
            $(vigra_builddir)/doc/classvigra*.gif $(vigra_builddir)/doc/form*.gif \
            $(vigra_builddir)/doc/doxygen.gif $(vigra_builddir)/doc/doxygen.css \
            $(docdir) ; \
          $(INSTALL) --mode=644 \
            $(vigra_builddir)/doc/documents/*.ps \
            $(vigra_builddir)/doc/documents/*.gif \
            $(docdir)/documents ; \
        fi

examples::
	@cd src ; $(MAKE) examples ; cd ..

doc::
	cd docsrc; $(MAKE) VIGRA_VERSION=$(VIGRA_VERSION)

docclean::
	cd docsrc; $(MAKE) -i clean

clean::
	@cd src ; $(MAKE) clean ; cd ..

distclean: clean
	rm -f config/vigra-config config/Makefile.include
	rm -f config.log config.cache config.status libtool
	cp config/Makefile.include.empty config/Makefile.include

maintainer-clean: distclean docclean
	rm -f config/aclocal.m4 configure

autoconf:
	cd config && \
	aclocal --acdir=. && \
	autoconf --output=../configure

.PHONY: autoconf all clean distclean doc docclean examples install install-docs install-exec install-includes
