# -*-Makefile-*-

NAME = structured-networking

.PHONY: default
default:
	for f in $(NAME)-*.cpp; \
	do \
	    echo cp $$f ../../examples/$(NAME).cpp; \
	    echo git add ../../examples/$(NAME).cpp; \
	    echo git commit -m "$(NAME) step `echo $$f | sed -e 's/$(NAME)-\(.*\).cpp/\1/'`"; \
	done
