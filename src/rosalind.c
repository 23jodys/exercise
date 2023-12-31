#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "dbg.h"
#include "sds.h"
#include "exercise.h"
#include "librosalind.h"

typedef enum {
	null,
	cons,
	dna,
	fib,
	prot,
	revc,
	rna,
} PROBLEM;

const static struct {
	PROBLEM val;
	const char *str;
} conversion [] = {
	{dna, "dna"},
	{rna, "rna"},
	{revc, "revc"},
	{prot, "prot"},
	{cons, "cons"},
	{fib, "fib"},
};


PROBLEM str2problem(const char* str) {
	for (int i=0; i < sizeof(conversion) / sizeof(conversion[0]); i++) {
		if(!strcmp (str, conversion[i].str))
			return conversion[i].val;
	}
	log_err("no enum conversion for %s", str);
	abort();
}

int main(int argc, char *argv[]) {
	bool success = false;

	int c;

	PROBLEM problem = null;

	char *line = NULL;
	size_t len = 0;
	ssize_t lineSize;
	sds result = NULL;
	sds *tokens;
	int split_token = 0;
	int i, j;

	while(1) {
		static struct option long_options[] =
		{
			{"problem", required_argument, 0, 'p'},
			{"input", required_argument, 0, 'i'},
			{0,0,0,0}
		};

		int option_index = 0;

		c = getopt_long (argc, argv, "p:i:", long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
			case 'p':
				log_info("option -p with value '%s'", optarg);
				problem = str2problem(optarg);
				break;
			case 'i':
				log_info("option -i with value '%s'", optarg);
				problem = str2problem(optarg);
				break;
			default:
				abort();

		}
	}

	check(problem, "No problem specified")


	lineSize = getline(&line, &len, stdin);
	result = sdsnew(line);

	check(!(-1 == lineSize), "failed to getline");


	if (dna == problem) {
		result = count(result);
	} else if (rna == problem) {
		result = transcribe_dna_to_rna(result);
	} else if (revc == problem) {
		result = reverse_complement(result);
	} else if (fib == problem) {
		tokens = sdssplitlen(result, sdslen(result), " ", 1, &split_token);	
		check((2 == split_token), "Expected 2 integers on a line");
		i = atoi(tokens[0]);
		j = atoi(tokens[1]);
		log_info("i = %d, j = %d", i, j);

		long int_result = calculate_breeding_pairs(i, j);
		result = sdsfromlonglong(int_result);
	}
	printf("%s\n", result);

	success = true;

error:
	if (result)
		sdsfree(result);

	if (line != NULL)
		free(line);

	if (success) { 
		return 0;
	} else { 
		return -1;
	}

}
