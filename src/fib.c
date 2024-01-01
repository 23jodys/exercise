#include "librosalind.h"

long calculate_breeding_pairs(int n, int k) {
	long F1 = 1;
	long F2 = 1;
	long F3 = 1;
	debug("i=0, F1 = %ld, F2 = %ld, F3 = %ld", F1, F2, F3);
	debug("i=1, F1 = %ld, F2 = %ld, F3 = %ld", F1, F2, F3);
	for (int i=2; i < n; i++) {
		F3 = (F1 * k) + F2;
		F1 = F2;
		F2 = F3;
		debug("i=%d, F1 = %ld, F2 = %ld, F3 = %ld", i, F1, F2, F3);
	}
	return F3;
}
