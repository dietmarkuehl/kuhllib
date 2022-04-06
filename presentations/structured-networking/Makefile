# -*-Makefile-*-

NAME = structured-networking

.PHONY: build build-code

build:
	cd ../..; $(MAKE) build

build-code:
	git checkout -B $(NAME)-tmp
	for f in $(NAME)-*.cpp; \
	do \
	    cp $$f ../../examples/$(NAME).cpp; \
	    git add ../../examples/$(NAME).cpp; \
	    git commit -m "$(NAME) step `echo $$f | sed -e 's/$(NAME)-\(.*\).cpp/\1/'`"; \
	done
	git log > $(NAME)-commits
