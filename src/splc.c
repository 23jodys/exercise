#include <assert.h>
#include "librosalind.h"
#include "libfasta.h"
#include "utlist.h"

#include "dbg.h"

sds orf_rosalind_interface(FILE* stream) {
	FastaStrings* input = FastaStrings_fromFile(stream);
	log_info("Found %d sequences", input->len);

	sds result = sdsempty();

	return result;
}


	
