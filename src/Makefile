.PHONY: default run build clean
default: build

run:
	../build-$(COMPILER)/kuhllib_test

build:
	cd ..; $(MAKE) -j4

clean:
	cd ..; $(MAKE) clean
