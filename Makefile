CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic
LFLAGS = -lcheck
CC = gcc

INSTALL_DIR := calculator_build

.PHONY: clean all rebuild install uninstall dvi dist

ALL_C_FILES := $(wildcard c_files/*.c)

SOURCES := $(filter-out c_files/s21_tests.c, $(ALL_C_FILES))
TESTS := c_files/s21_tests.c

OBJECTS := $(SOURCES:.c=.o)
TEST_OBJECTS := $(TESTS:.c=.o)

ifeq ($(shell uname), Linux)
	LFLAGS = -lpthread -lcheck -lsubunit -lm
endif

%.o: %.c s21_calc.h
	$(CC) $(CFLAGS) -c $< -o $@

all: test

install:
	mkdir -p $(INSTALL_DIR)
	cd $(INSTALL_DIR) && qmake6 ../calc_qt/calc_qt_qmake/calc_qt_qmake.pro && make
	cp $(INSTALL_DIR)/calc_qt_qmake .
	mv calc_qt_qmake calculator

uninstall:
	rm -rf $(INSTALL_DIR)
	rm calculator

dvi:
	doxygen Doxyfile
	open html/index.html

dist:
	mkdir -p ../calc_dist
	cp -r ../src ../calc_dist
	tar --totals -czvf calc_dist.tar.gz ../calc_dist
	rm -rf ../calc_dist

test: $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(OBJECTS) c_files/s21_tests.c $(LFLAGS) -o test
	./test

gcov_report: clean add_gcov_flags test
	./test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report
	open gcov_report/index.html

add_gcov_flags:
	$(eval CFLAGS += --coverage)
	$(eval LFLAGS += -lgcov)

clean:
	rm -rf *.o c_files/*.o test gcov_report c_files/*.gcda c_files/*.gcno coverage.info *.a a.out *.tar latex html calculator_build calculator

rebuild: clean all
