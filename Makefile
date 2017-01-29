export PREFIX=${HOME}/software
export BINDIR=$(PREFIX)/bin

all:
	$(MAKE) -C gzstream
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	$(MAKE) -C gzstream clean

install: all
	$(MAKE) -C src install

test: all
	$(MAKE) -C test

.PHONY: all clean install test

