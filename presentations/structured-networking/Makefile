# -*-Makefile-*-

NAME = structured-networking

.PHONY: default
default:
	for f in $(NAME)-*.cpp; \
	do \
	    cp $$f ../../examples/$(NAME).cpp; \
	    git add ../../examples/$(NAME).cpp; \
	    git commit -m "$(NAME) step `echo $$f | sed -e 's/$(NAME)-\(.*\).cpp/\1/'`"; \
	done
