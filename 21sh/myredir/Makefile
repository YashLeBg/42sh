CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla
RED = \\033[31m
GREEN = \\033[32m
YELLOW = \\033[33m

myredir :
	gcc -o myredir myredir.c

check :
	@echo -e "$(GREEN)\n\n\n****************TESSTING****************$(RESET)"
	@echo -e "$(RED)ATTENTION : le test $(YELLOW)ls nonexitent$(RED) dépend de la langue du terminal$(RESET)\n\n\n"
	./testsuite.sh

clean :
	${RM} myredir *.o *.txt
