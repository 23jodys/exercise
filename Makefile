UNAME_S := $(shell uname -s)
HEADERS := $(wildcard *.h)
VPATH := src:include:tests:sds

ifeq ($(UNAME_S),Darwin)
GCOV := xcrun llvm-cov gcov
else
GCOV := llvm-cov gcov
endif

CC   := clang

CFLAGS += $(if $(COVERAGE), -fprofile-arcs -ftest-coverage )
CFLAGS += $(if $(DEBUG), -DDEBUG=1 )
CFLAGS += -Werror -Iinclude -Isds -g

ifeq ($(UNAME_S),OpenBSD)
CFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib
endif

LDLIBS += $(if $(or $(COVERAGE),$(DEBUG)), -g )
LDLIBS += $(if $(COVERAGE), --coverage )


test_fizzbuzz: LDLIBS += -lcmocka
test_fizzbuzz: fizzbuzz.o test_fizzbuzz.o sds.o

fizzbuzz.o: exercise.h sds.o

fizz: fizzbuzz.o sds.o

test_rna: LDLIBS += -lcmocka
test_rna: rna.o test_rna.o sds.o

.PHONY: test
test: test_fizzbuzz test_rna
	./test_fizzbuzz 
	./test_rna

.PHONY: valgrind
valgrind: valgrind_test_rna valgring_test_fizzbuzz

valgrind_%: %
	valgrind --leak-check=full --error-exitcode=1 ./$* 

coverage: COVERAGE=1
coverage: test
	$(GCOV) $(SRCS)

TAGS: $(SRCS) fizzbuzz.h test_*.[ch]
	etags $^

docs: $(HEADERS)
	doxygen

.PHONY: clean
clean:
	rm -rf *.o *.gcda *.gcno fizz test_rna test_fizzbuzz *.dSYM html/ latex/
