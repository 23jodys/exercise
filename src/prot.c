#include "exercise.h"

/*
 * The naive approach would be substring matching every possible codon. A less
 * naive approach might be regex (but I'm trying not to use to many libraries.
 * So this model is that we have a three level tree structure. Each path through
 * the tree is a different codon string. The bottom most vertices contain the 
 * protein character.
 *
 * There is a fair amount of stupid stuff going on here to translate a char into
 * something we can index with.
 */

typedef enum base { baseA, baseU, baseC, baseG, baseNone } base; 

struct {
	base base;
	char upper;
} base_lookup[] = {
	[baseA].base = baseA, [baseA].upper = 'A',
	[baseU].base = baseU, [baseU].upper = 'U',
	[baseG].base = baseG, [baseG].upper = 'G',
	[baseC].base = baseC, [baseC].upper = 'C'
};


typedef struct Node {
	struct Node* nodes[4];
	char value;
} Node;

struct Node A = {.value = 'A'};
struct Node C = {.value = 'C'};
struct Node D = {.value = 'D'};
struct Node E = {.value = 'E'};
struct Node F = {.value = 'F'};
struct Node G = {.value = 'G'};
struct Node H = {.value = 'H'};
struct Node I = {.value = 'I'};
struct Node K = {.value = 'K'};
struct Node L = {.value = 'L'};
struct Node M = {.value = 'M'};
struct Node N = {.value = 'N'};
struct Node P = {.value = 'P'};
struct Node Q = {.value = 'Q'};
struct Node R = {.value = 'R'};
struct Node S = {.value = 'S'};
struct Node T = {.value = 'T'};
struct Node V = {.value = 'V'};
struct Node W = {.value = 'W'};
struct Node Y = {.value = 'Y'};
struct Node Stop = {.value = '0'};

Node root = {
	.nodes[baseG] = &((Node)
		{
			.nodes[baseG] = &((Node) {.nodes[baseG] = &G, .nodes[baseA] = &G, .nodes[baseU] = &G, .nodes[baseC] = &G}),
			.nodes[baseA] = &((Node) {.nodes[baseG] = &E, .nodes[baseA] = &E, .nodes[baseC] = &D, .nodes[baseU] = &D}),
			.nodes[baseC] = &((Node) {.nodes[baseG] = &A, .nodes[baseA] = &A, .nodes[baseC] = &A, .nodes[baseU] = &A}),
			.nodes[baseU] = &((Node) {.nodes[baseG] = &V, .nodes[baseA] = &V, .nodes[baseC] = &V, .nodes[baseU] = &V}),
		}
	),
	.nodes[baseA] = &((Node)
		{
			.nodes[baseG] = &((Node) {.nodes[baseG] = &R, .nodes[baseA] = &R, .nodes[baseC] = &S, .nodes[baseU] = &S}),
			.nodes[baseA] = &((Node) {.nodes[baseG] = &K, .nodes[baseA] = &K, .nodes[baseC] = &N, .nodes[baseU] = &N}),
			.nodes[baseC] = &((Node) {.nodes[baseG] = &T, .nodes[baseA] = &T, .nodes[baseC] = &T, .nodes[baseU] = &T}),
			.nodes[baseU] = &((Node) {.nodes[baseG] = &M, .nodes[baseA] = &I, .nodes[baseC] = &I, .nodes[baseU] = &I}),
		}
	),
	.nodes[baseC] = &((Node)
		{
			.nodes[baseG] = &((Node) {.nodes[baseA] = &R, .nodes[baseU] = &R, .nodes[baseC] = &R, .nodes[baseG] = &R}),
			.nodes[baseA] = &((Node) {.nodes[baseA] = &Q, .nodes[baseU] = &H, .nodes[baseC] = &H, .nodes[baseG] = &Q}),
			.nodes[baseC] = &((Node) {.nodes[baseG] = &P, .nodes[baseA] = &P, .nodes[baseC] = &P, .nodes[baseU] = &P}),
			.nodes[baseU] = &((Node) {.nodes[baseG] = &L, .nodes[baseA] = &L, .nodes[baseC] = &L, .nodes[baseU] = &L}),
		}
	),
	.nodes[baseU] = &((Node)
		{
			.nodes[baseG] = &((Node) {.nodes[baseG] = &W, .nodes[baseA] = &Stop, .nodes[baseC] = &C, .nodes[baseU] = &C}),
			.nodes[baseA] = &((Node) {.nodes[baseG] = &Stop, .nodes[baseA] = &Stop, .nodes[baseC] = &Y, .nodes[baseU] = &Y}),
			.nodes[baseC] = &((Node) {.nodes[baseG] = &S, .nodes[baseA] = &S, .nodes[baseC] = &S, .nodes[baseU] = &S}),
			.nodes[baseU] = &((Node) {.nodes[baseG] = &L, .nodes[baseA] = &L, .nodes[baseC] = &F, .nodes[baseU] = &F}),
		}
	),
};

/** 
 * @brief Private function for performing the lookup converting char to the enum
 */
base lu(char input) {
	base value = baseNone;
	for (int j = 0; j < 4; j++) {
		if (input == base_lookup[j].upper) {
			value = base_lookup[j].base;
		}
	}
	return value;
}

sds prot(sds input) {
	if (sdslen(input) % 3 != 0) {
		return NULL;
	}
	sds result = sdsempty();
	
	for (int i = 0; i < sdslen(input); i += 3) {
		debug("i = %d, looking at substring: %.3s", i, &(input[i]));
		base x = lu(input[i+0]);
		debug("input[i+0] = %c", input[i+0]);
		base y = lu(input[i+1]);
		debug("input[i+1] = %c", input[i+1]);
		base z = lu(input[i+2]);
		debug("input[i+0] = %c", input[i+2]);

		if (x == baseNone || y == baseNone || z == baseNone) {
			sdsfree(result);
			return NULL;
		}
		char protein = root.nodes[x]->nodes[y]->nodes[z]->value;
		if (protein != '0') {
			debug("catting '%c' to result %s", protein, result);
			result = sdscatlen(result, &(protein), 1);
		}
	}

	debug("result: %s", result);

	return result;
}
