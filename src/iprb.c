#include "librosalind.h"
//#define NDEBUG 1
#include "dbg.h"
sds iprb_rosalind_interface(FILE* stream) {
	sds input = NULL;
	sds result = sdsempty();
	double _result = 0.0;
	char *line = NULL;
	ssize_t lineSize;
	size_t len = 0;
	sds *tokens;
	int num_tokens = 0;
	int k, m, n;

	lineSize = getline(&line, &len, stream);
	check(!(-1 == lineSize), "failed to getline");
	input = sdsnew(line);

	tokens = sdssplitlen(input, sdslen(input), " ", 1, &num_tokens);
	check((3 == num_tokens), "Expected 3 integers on a line");

	k = atoi(tokens[0]);
	m = atoi(tokens[1]);
	n = atoi(tokens[2]);
	log_info("k = %d, m = %d, n = %d", k, m, n);

	if (line != NULL) {
		free(line);
	}

	_result = calculate_mendelian_inheritance(k, m, n);
	log_info("raw result: %lf", _result);

	result = sdscatprintf(result, "%lf", _result);
	log_info("sds result: %s", result);
	return result;
error:
	if (line != NULL)
		free(line);
	abort();
} 

double calculate_mendelian_inheritance(double homozygous_dominant, double heterozygous, double homozygous_recessive) {
	double result = 0.0;
	double prob_hetero_plus_homozygous_recessive = 0.5;
	double prob_hetero_plus_hetero = 0.75;
	log_info("homozygous_dominant : %lf", homozygous_dominant);
	log_info("heterozygous        : %lf", heterozygous);
	log_info("homozygous_recessive: %lf", homozygous_recessive);

	double total_organisms = homozygous_dominant + homozygous_recessive + heterozygous;
	log_info("total_organisms: %lf", total_organisms);

	double choose_homozygous_dominant = (
			(homozygous_dominant / total_organisms)
	);
	log_info("choose_homozygous_dominant: %lf", choose_homozygous_dominant);

	double choose_homozygous_recessive_choose_homozygous_dominant = (
		((homozygous_recessive / total_organisms) * (homozygous_dominant / (total_organisms - 1.0))) 
	);
	log_info("choose_homozygous_recessive_choose_homozygous_dominant: %lf", choose_homozygous_recessive_choose_homozygous_dominant);  

	double choose_homozygous_recessive_choose_heterzygous = (
		((homozygous_recessive / total_organisms) * (heterozygous / (total_organisms - 1.0)) * prob_hetero_plus_homozygous_recessive)
	);
	log_info("choose_homozygous_recessive_choose_heterzygous: %lf", choose_homozygous_recessive_choose_heterzygous);  

	double choose_heterozygous_choose_homozygous_dominant = (
		(heterozygous / total_organisms) * (homozygous_dominant / (total_organisms - 1.0))
	);
	log_info("choose_heterozygous_choose_homozygous_dominant: %lf", choose_heterozygous_choose_homozygous_dominant);

	double choose_heterozygous_choose_homozygous_recessive = (
		(heterozygous / total_organisms) * (homozygous_recessive / (total_organisms - 1.0)) * prob_hetero_plus_homozygous_recessive
	);
	log_info("choose_heterozygous_choose_homozygous_recessive: %lf", choose_heterozygous_choose_homozygous_recessive);

	double choose_heterozygous_choose_heterozygous = (
		(heterozygous / total_organisms) * (heterozygous - 1.0) / (total_organisms - 1.0) * prob_hetero_plus_hetero
	);
	log_info("choose_heterozygous_choose_heterozygous: %lf", choose_heterozygous_choose_heterozygous);

	result = choose_homozygous_dominant + \
		 choose_homozygous_recessive_choose_homozygous_dominant + \
		 choose_homozygous_recessive_choose_heterzygous + \
		 choose_heterozygous_choose_homozygous_dominant + \
		 choose_heterozygous_choose_homozygous_recessive + \
		 choose_heterozygous_choose_heterozygous;

	log_info("returning %lf", result);

	return result;
}
