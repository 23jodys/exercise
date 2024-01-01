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
CFLAGS += -Werror -Iinclude -Isds -g -gdwarf-4

ifeq ($(UNAME_S),OpenBSD)
CFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib
endif

LDLIBS += $(if $(or $(COVERAGE),$(DEBUG)), -g )
LDLIBS += $(if $(COVERAGE), --coverage )


rosalind: rosalind.o sds.o dna.o rna.o revc.o fib.o gc.o libfasta.o iprb.o prot.o subs.o hamm.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^


test_%: %.o sds.o test_%.o
	$(CC) $(CFLAGS) $^ -o test_$* -lcmocka

test_cons: cons.o libfasta.o test_cons.o sds.o
	$(CC) $(CFLAGS) $^ -o test_cons -lcmocka

test_gc: gc.o libfasta.o test_gc.o sds.o
	$(CC) $(CFLAGS) $^ -o test_gc -lcmocka

.PHONY: test
test: test_fizzbuzz test_rna test_hamm test_revc test_subs test_prot test_cons test_libfasta test_dna test_fib test_gc
	./test_fizzbuzz
	./test_rna
	./test_hamm
	./test_revc
	./test_subs
	./test_prot
	./test_cons
	./test_libfasta
	./test_dna
	./test_fib
	./test_gc

.PHONY: valgrind
valgrind: valgrind_test_rna valgrind_test_fizzbuzz valgrind_test_hamm valgrind_test_revc valgrind_test_subs valgrind_test_prot valgrind_test_cons valgrind_test_libfasta

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
	rm -rf *.o *.gcda *.gcno fizz test_* *.dSYM cm_*.xml html/ latex/
