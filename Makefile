FLAGS = -Wall -Wextra -Werror -O2
CC = g++
SOURCES = *.cpp Operand.hpp
NAME = Abstract_VM

all: $(SOURCES)
	@$(CC) $(FLAGS) $(SOURCES) -o $(NAME)

clean:
	
test: $(NAME)
	@echo "\t====== test1 ========"
	@./Abstract_VM test/test1.txt
	@sleep 1
	@echo "\t====== test2 ========"
	@./Abstract_VM test/test2.txt
	@sleep 1
	@echo "\t====== test3 ========"
	@./Abstract_VM test/test3.txt
	@sleep 1
	@echo "\t====== test4 ========"
	@./Abstract_VM test/test4.txt
	@sleep 1
	@echo "\t====== test5 ========"
	@./Abstract_VM test/test5.txt
	@sleep 1
	@echo "\t====== test6 ========"
	@./Abstract_VM test/test6.txt
	@sleep 1
	@echo "\t====== test7 ========"
	@./Abstract_VM test/test7.txt
	@sleep 1
	@echo "\t====== test8 ========"
	@./Abstract_VM test/test8.txt
	@sleep 1
	@echo ""
fclean:
	@rm -f $(NAME)

re : fclean all