JOBS = 4

.PHONY: default run build clean
default: build

run:
	../build-$(COMPILER)/kuhllib_test

build:
	cd ..; $(MAKE) -j$(JOBS)

clean:
	cd ..; $(MAKE) clean

.PHONY: echo
echo:
	../build-$(COMPILER)/echo_server
