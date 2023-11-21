CXX = g++
CXXFLAGS = #-Wall -Werror -Wextra -std=c++17
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
LFLAGS = -lgtest -lgtest_main -fprofile-arcs

INSTALL_DIR := calculator_build

.PHONY: clean all rebuild install uninstall dvi dist

ifeq ($(shell uname), Linux)
	LFLAGS += -lpthread -lcheck -lsubunit -lm
endif

s21_model.o: cpp_files/s21_model.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@


tests.o: cpp_files/tests.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

all: tests

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

tests: s21_model.o tests.o
	$(CXX) $(CXXFLAGS) $^ $(LFLAGS) -o tests
	./tests

gcov_report: clean test_gcov
	./tests
	lcov -c -d . -o coverage.info --no-external
	lcov --remove coverage.info '/usr/*' --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report
	open gcov_report/index.html

test_gcov: tests.o
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) cpp_files/s21_model.cpp $^ $(LFLAGS) -o tests

clean:
	rm -rf *.o cpp_files/*.o tests gcov_report *.gcda *.gcno coverage.info a.out *.tar latex html calculator_build calculator

rebuild: clean all
