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



# test_fizzbuzz: LDLIBS += -lcmocka
# test_fizzbuzz: fizzbuzz.o test_fizzbuzz.o sds.o

#fizzbuzz.o: exercise.h sds.o

#fizz: fizzbuzz.o sds.o


#test_hamm: LDLIBS += -lcmocka
#test_hamm: hamm.o test_hamm.o sds.o

#test_revc: LDLIBS += -lcmocka
#test_revc: revc.o test_revc.o sds.o

#test_subs: LDLIBS += -lcmocka
#test_subs: subs.o test_subs.o sds.o

#test_prot: LDLIBS += -lcmocka
#test_prot: prot.o test_prot.o sds.o

#test_cons: LDLIBS += -lcmocka
#test_cons: cons.o test_cons.o sds.o libfasta.o

#test_libfasta: LDLIBS += -lcmocka
#test_libfasta: libfasta.o test_libfasta.o sds.o

#dna: dna.o sds.o

rosalind: rosalind.o sds.o dna.o rna.o revc.o fib.o gc.o libfasta.o

#test_dna: LDLIBS += -lcmocka
#test_dna: dna.o sds.o test_dna.o


#test_rna: LDLIBS += -lcmocka
#test_%: rna.o test_rna.o sds.o
test_%: %.o sds.o test_%.o
	$(CC) $^ -o test_$* -lcmocka

#test_rna.o: test_rna.c
#sds.o: sds.c
#test_%: LDLIBS += -lcmocka

.PHONY: test
test: test_fizzbuzz test_rna test_hamm test_revc test_subs test_prot test_cons test_libfasta test_dna
	./test_fizzbuzz
	./test_rna
	./test_hamm
	./test_revc
	./test_subs
	./test_prot
	./test_cons
	./test_libfasta
	./test_dna

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
