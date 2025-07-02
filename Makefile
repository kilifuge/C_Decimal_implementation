CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11

SRC = $(wildcard s21_*.c)
TEST_SRC = $(wildcard tests/s21_*.c) tests/main.c

OBJ= $(patsubst %.c,%.o,$(SRC))
TEST_OBJ= $(patsubst tests/%.c,%.o,$(TEST_SRC))
GCOV_TEST_OBJ= $(patsubst %, gcov_%, $(OBJ))

GCOV_FLAGS = --coverage
LIBS = -lcheck -lm -lsubunit

all: s21_decimal.a

s21_decimal.a: $(OBJ)
	ar rc $@ $(OBJ)
	ranlib $@

test: s21_decimal.a $(TEST_OBJ)
	$(CC) $(FLAGS) $(TEST_OBJ) s21_decimal.a -o ./unit_tests $(LIBS)
	./unit_tests

%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@

%.o: tests/%.c
	$(CC) -c $(FLAGS) $< -o $@

gcov_%.o: %.c
	$(CC) -c $(FLAGS) $(GCOV_FLAGS) $< -o $@

gcov_report: s21_decimal.a $(GCOV_TEST_OBJ) $(TEST_OBJ)
	$(CC) $(FLAGS) $(GCOV_FLAGS) $(GCOV_TEST_OBJ) $(TEST_OBJ) -o ./gcov_tests $(LIBS)
	./gcov_tests
	lcov -t "s21_decimal_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info

clean:
	rm -rf *.o
	rm -rf *.gcno *.gcda gcov_tests unit_tests *.info report/ s21_decimal.a

rebuild_gcov: clean all test gcov_report

rebuild: clean all test
