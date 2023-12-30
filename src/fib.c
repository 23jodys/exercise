#include "librosalind.h"

int calculate_breeding_pairs(int n, int k) {
	int last_total = 1;
	for (int i=0; i <= n; i++) {
		last_total = last_total + k;	
	}	
	return last_total;
}
