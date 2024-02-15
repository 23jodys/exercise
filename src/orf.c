#include "librosalind.h"
#include "libfasta.h"

sds orf_rosalind_interface(FILE* stream) {
	FastaStrings* input = FastaStrings_fromFile(stream);
	log_info("Found %d sequences", input->len);

	sds result[6];
	int result_len = 6;

	find_orf(input->sequences[0].sequence, result, result_len);

	return sdsjoinsds(result, result_len, "\n", 1);
}

void find_orf(sds input, sds result[], int result_len) {

}
