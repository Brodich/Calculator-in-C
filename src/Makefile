CC = gcc -std=c11 -Wall -Wextra -Werror -g
OS = $(shell uname)
GCOVFLAGS = -L. --coverage

FILES_O = smart_calc.o
NAME_LIB = smart_calc.a
NAME_TEST = test_smart_calc

TARGET = smart_calc
SRCS = c_files/smart_calc.c c_files/stack.c
OBJS = $(SRCS:.c=.o)
LINUX_LIBRARY = -lm

ifeq ($(OS), Linux)
	FLAGS = -lcheck -lm -lpthread -lrt -lsubunit
	INSTALL_OS = ./qt_calc
else
	FLAGS = -lcheck -lm
	INSTALL_OS = qt_calc.app/Contents/MacOS/qt_calc
endif


all: clean clang install

%.o: %.c
	$(CC) -c -o $@ $<

gcov_report: create_tests $(NAME_LIB) test
	rm -f *.g*
	$(CC) $(SRCS) $(NAME_TEST).c $(NAME_LIB) $(FLAGS) $(GCOVFLAGS)  $(NAME_LIB) -o test
	./test
	lcov -t test -o rep.info -c -d .
	genhtml -o report rep.info
	rm -rf *.gcda *.gcno *.info
	open ./report/index.html
	
create_tests:
	checkmk clean_mode=1 c_files/$(NAME_TEST).check > $(NAME_TEST).c	

test: clean clang $(NAME_LIB) create_tests $(NAME_TEST).o
	$(CC) $(NAME_TEST).o $(NAME_LIB) $(FLAGS) -o test
	./test

$(NAME_TEST).o:
	$(CC) -w -c $(NAME_TEST).c

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LINUX_LIBRARY)

$(NAME_LIB): $(OBJS)
	ar -rc $(NAME_LIB) $(OBJS)
	ranlib $(NAME_LIB)
	rm -rf *.o

install:
	mkdir build && \
	cd build && \
	cmake .. && \
	make && \
	unset GTK_PATH && \
	$(INSTALL_OS)
	
uninstall:
	rm -rf build/ *.tar.gz

dist: clean
	tar -cvzf qt_calc.tar.gz c_files/ frontend/ doc/ qcustomplot/ CMakeLists.txt Makefile qcustomplot.cpp qcustomplot.h

dvi:
	cd doc && \
	doxygen && \
	open html/index.html

clean:
	rm -rf a.out $(NAME_TEST).c $(NAME_TEST).o smart_calc.a smart_calc test CMakeLists.txt.user
	rm -rf report/ test.dSYM/ ../a.out.dSYM/
	cd c_files/ && \
	rm -rf a.out $(NAME_TEST).c smart_calc.a smart_calc test && \
	rm -rf *.gcda *.gcno *.info *.o ./report ./a.out.dSYM ./output && \
	cd ../doc && rm -rf html/ latex/
	rm -rf build/ *.tar.gz

git: clean
	git add ../. && git commit -m add && git push

clang:
	clang-format -style=google -i frontend/*.cpp 
	clang-format -style=google -i c_files/*.c 
	clang-format -style=google -i c_files/*.h
