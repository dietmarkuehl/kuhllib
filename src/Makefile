JOBS = 4

.PHONY: default run check build clean
default: build

run:
	../build-$(COMPILER)/es

clean check build:
	cd ..; $(MAKE) -j$(JOBS) $@

.PHONY: echo chat
echo chat:
	../build-$(COMPILER)/$@_server
