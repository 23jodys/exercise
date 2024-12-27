UNAME_S := $(shell uname -s)
HEADERS := $(wildcard include/*.h) sds/sds.h
SOURCES := $(wildcard src/*.c)
VPATH := src:include:tests:sds

ifeq ($(UNAME_S),Darwin)
GCOV := xcrun llvm-cov gcov
LDFLAGS += -L/opt/homebrew/lib
CFLAGS += -I/opt/homebrew/include
LDFLAGS += -L/opt/homebrew/opt/curl/lib
CFLAGS += -I/opt/homebrew/opt/curl/include
CTAGS := /opt/homebrew/bin/ctags
else
GCOV := llvm-cov gcov
endif

ifeq ($(UNAME_S),OpenBSD)
CFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib
endif

CC   := clang

# Set NODEBUG=1 in environment to turn off debug() messages
CFLAGS += $(if $(COVERAGE), -fprofile-arcs -ftest-coverage )
CFLAGS += $(if $(NODEBUG), -DNDEBUG=1 )
CFLAGS += -Werror -Iinclude -Isds -g -gdwarf-4


LDLIBS += $(if $(or $(COVERAGE),$(DEBUG)), -g )
LDLIBS += $(if $(COVERAGE), --coverage )

LDFLAGS += -lpcre2-8 -lcurl

rosalind: rosalind.o sds.o dna.o rna.o revc.o fib.o gc.o libfasta.o iprb.o prot.o subs.o hamm.o orf.o grph.o mprt.o 

test_cons: cons.o libfasta.o test_cons.o sds.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o test_cons -lcmocka

test_gc: gc.o libfasta.o test_gc.o sds.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o test_gc -lcmocka

test_orf: orf.o libfasta.o test_orf.o sds.o prot.o revc.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o test_orf -lcmocka

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

test_%: %.o libfasta.o sds.o test_%.o 
	$(CC) $(CFLAGS) $(LDFLAGS)  $^ -o test_$* -lcmocka 

.PHONY: test
test: test_fizzbuzz test_rna test_hamm test_revc test_subs test_prot test_cons test_libfasta test_dna test_fib test_gc test_iprb test_orf test_grph test_mprt
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
	./test_iprb
	./test_grph


all: test

.PHONY: valgrind
valgrind: valgrind_test_rna valgrind_test_fizzbuzz valgrind_test_hamm valgrind_test_revc valgrind_test_subs valgrind_test_prot valgrind_test_cons valgrind_test_libfasta valgrind_test_orf

valgrind_%: %
	valgrind --leak-check=full --error-exitcode=1 ./$* 

coverage: COVERAGE=1
coverage: test
	$(GCOV) $(SRCS)

TAGS: $(SRCS) $(HEADERS) $(SOURCES) tests/test_*.[ch]
	echo $^
	ctags $^

docs: $(HEADERS)
	doxygen

.PHONY: clean
clean:
	rm -rf *.o *.gcda *.gcno fizz test_* *.dSYM cm_*.xml html/ latex/
