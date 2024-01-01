#include <stdbool.h>
/* define _GNU_SOURCE to pull in POSIX 2008 so we have getline */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "dbg.h"
#include "sds.h"
#include "librosalind.h"
#include "cons.h"

typedef enum {
	null,
	cons,
	dna,
	fib,
	prot,
	revc,
	rna,
	gc,
	hamm,
	iprb,
	subs,
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
	{gc, "gc"},
	{hamm, "hamm"},
	{iprb, "iprb"},
	{subs, "subs"},
};


PROBLEM str2problem(const char* str) {
	for (int i=0; i < sizeof(conversion) / sizeof(conversion[0]); i++) {
		if(!strcmp (str, conversion[i].str))
			return conversion[i].val;
	}
	log_err("no enum conversion for %s", str);
	abort();
}

sds get_single_sds_line(FILE* stream) {
	char *line = NULL;
	ssize_t lineSize;
	size_t len = 0;

	lineSize = getline(&line, &len, stream);
	check(!(-1 == lineSize), "failed to getline");

	sds result = sdsnew(line);
	if (line != NULL)
		free(line);
	return result;
error: 
	free(line);
	abort();
}

int main(int argc, char *argv[]) {
	bool success = false;

	int c;

	PROBLEM problem = null;

	sds result = sdsempty();
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

	check(problem, "No problem specified");


	if (dna == problem) {
		sds input = sdsempty();
		input = get_single_sds_line(stdin);
		result = count(input);
	} else if (rna == problem) {
		sds input = sdsempty();
		input = get_single_sds_line(stdin);
		result = transcribe_dna_to_rna(input);
	} else if (revc == problem) {
		sds input = sdsempty();
		input = get_single_sds_line(stdin);
		result = reverse_complement(input);
	} else if (fib == problem) {
		sds input = sdsempty();
		input = get_single_sds_line(stdin);
		tokens = sdssplitlen(input, sdslen(input), " ", 1, &split_token);	
		check((2 == split_token), "Expected 2 integers on a line");
		i = atoi(tokens[0]);
		j = atoi(tokens[1]);
		log_info("i = %d, j = %d", i, j);

		long int_result = calculate_breeding_pairs(i, j);
		result = sdsfromlonglong(int_result);
	} else if (gc == problem) {
		result = gc_rosalind_interface(stdin);
	} else if (hamm == problem) {
		hamm_rosalind_interface(stdin, result);
	} else if (iprb == problem) {
		result = iprb_rosalind_interface(stdin);
	} else if (prot == problem) {
		result = prot_rosalind_interface(stdin);
	} else if (subs == problem) {
		result = subs_rosalind_interface(stdin);
	} else if (cons == problem) {
		result = cons_rosalind_interface(stdin);
	} else {
		success = false;
		goto error;
	}
	printf("%s\n", result);
	success = true;

error:
	if (result)
		sdsfree(result);

	if (success) { 
		return 0;
	} else { 
		return -1;
	}

}
