#include "exercise.h"
#include <assert.h>

typedef struct Node {
	struct Node* baseA;
	struct Node* baseU;
	struct Node* baseC;
	struct Node* baseG;
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
	.baseG = &((Node)
		{
			.baseG = &((Node) {.baseG = &G, .baseA = &G, .baseC = &G, .baseU = &G}),
			.baseA = &((Node) {.baseG = &E, .baseA = &E, .baseC = &D, .baseU = &D}),
			.baseC = &((Node) {.baseG = &A, .baseA = &A, .baseC = &A, .baseU = &A}),
			.baseU = &((Node) {.baseG = &V, .baseA = &V, .baseC = &V, .baseU = &V}),
		}
	),
	.baseA = &((Node)
		{
			.baseG = &((Node) {.baseG = &R, .baseA = &R, .baseC = &S, .baseU = &S}),
			.baseA = &((Node) {.baseG = &K, .baseA = &K, .baseC = &N, .baseU = &N}),
			.baseC = &((Node) {.baseG = &T, .baseA = &T, .baseC = &T, .baseU = &T}),
			.baseU = &((Node) {.baseG = &M, .baseA = &I, .baseC = &I, .baseU = &I}),
		}
	),
	.baseC = &((Node)
		{
			.baseG = &((Node) {.baseA = &R, .baseU = &R, .baseC = &R, .baseG = &R}),
			.baseA = &((Node) {.baseA = &Q, .baseU = &H, .baseC = &H, .baseG = &Q}),
			.baseC = &((Node) {.baseG = &P, .baseA = &P, .baseC = &P, .baseU = &P}),
			.baseU = &((Node) {.baseG = &L, .baseA = &L, .baseC = &L, .baseU = &L}),
		}
	),
	.baseU = &((Node)
		{
			.baseG = &((Node) {.baseG = &W, .baseA = &Stop, .baseC = &C, .baseU = &C}),
			.baseA = &((Node) {.baseG = &Stop, .baseA = &Stop, .baseC = &Y, .baseU = &Y}),
			.baseC = &((Node) {.baseG = &S, .baseA = &S, .baseC = &S, .baseU = &S}),
			.baseU = &((Node) {.baseG = &L, .baseA = &L, .baseC = &F, .baseU = &F}),
		}
	),
};

sds prot(sds input) {
	assert(sdslen(input) % 3 == 0);
	for (int i = 0; i < sdslen(input); i += 3) {
		printf("char: %c\n", root.base
}
