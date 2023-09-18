#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "dbg.h"
#include "sds.h"
#include "librosalind.h"

typedef enum {
	dna,
	rna,
	prot,
	cons
} PROBLEM;

const static struct {
	PROBLEM val;
	const char *str;
} conversion [] = {
	{dna, "dna"},
	{rna, "rna"},
	{prot, "prot"},
	{cons, "cons"}
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

	int c;


	PROBLEM problem;
 
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

	if (dna == problem) {
		char *line = NULL;
		size_t len = 0;
		ssize_t lineSize = 0;
		sds result = sdsempty();

		lineSize = getline(&line, &len, stdin);

		result = count(line, len);
		printf("%s\n", result);

		free (line);
	}

	return 0;
}
