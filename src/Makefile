JOBS = 4

.PHONY: default run check build clean
default: check

run:
	../build-$(COMPILER)/kuhllib_test

clean check build:
	cd ..; $(MAKE) -j$(JOBS) $@

.PHONY: echo
echo:
	../build-$(COMPILER)/echo_server
