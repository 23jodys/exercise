#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "sds.h"
#include "librosalind.h"

int main(int argc, char *argv[]) {

	int c;

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
				printf("option -p with value '%s'\n", optarg);
				break;
			case 'i':
				printf("option -i with value '%s'\n", optarg);
				break;
			default:
				abort();

		}
	}


	/*
	char *line = NULL;
	size_t len = 0;
	ssize_t lineSize = 0;
	sds result = sdsempty();

	lineSize = getline(&line, &len, stdin);

	result = count(line, len);
	printf("line: %s\n", result);

	free (line);
	*/

	return 0;
}
