#include "librosalind.h"
#include "libfasta.h"

sds gc_rosalind_interface(FILE* stream) {
	FastaStrings* input = FastaStrings_fromFile(stream);
	log_info("Found %d sequences", input->len);
	double best_gc = 0.0;
	double current_gc = 0.0;
	int best_index = -1;
	for (int i = 0; i < input->len; i++) {
		log_info("Counting GC in %s", input->sequences[i].sequence);
		current_gc = count_gc(input->sequences[i].sequence);
		log_info("Current gc: %lf", current_gc);
		if (current_gc > best_gc) {
			log_info("found best GC at %lf", current_gc);
			best_gc = current_gc;
			best_index = i;
		}
	}
	sds x = sdsempty();
	x = sdscatprintf(x, "%s\n%lf", input->sequences[best_index].name, best_gc);
	return x;
}

double count_gc(sds sequence) {
	int total_count = 0;
	int gc_count = 0; 
	for (int i = 0; i < sdslen(sequence); i++) {
		if (sequence[i] == 'G' || sequence[i] == 'C') {
			gc_count += 1;
		}
		total_count += 1;
	}
	double gc_percent = (double)gc_count / (double)total_count;
	log_info("gc count: %d, total Count: %d, result: %lf", gc_count, total_count, gc_percent);
	return gc_percent * 100;
}

