#include "exercise.h"

int main(int argc, char* argv[]) {
	for (int i=1; i<100; i++) {
		sds result = fizzbuzz(i);	
		printf("%s, ", result);
	}
	printf("\n");
}
