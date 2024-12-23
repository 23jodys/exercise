#include <stdbool.h>
/* define _GNU_SOURCE to pull in POSIX 2008 so we have getline */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sds.h"
#include "libfasta.h"

#include "dbg.h"

/**
 * @brief Count DNA
 *
 * Counts the number of A, G, T, C in a string 
 *
 * Caller is responsible for freeing the result string
 *
 * @params [in] input
 * @returns sds result
 */
sds count(sds input);

/**
 * @brief Transform input DNA sds string into its reverse complement
 *
 * Input is allowed up to 1000 (inclusive) base pairs. Any larger input is not transformed and a NULL is returned.
 *
 * @param [in] input 
 * @returns sds OR NULL
 */
sds reverse_complement(sds input);

/** @brief convert a sds string of uppercase RNA bases to proteins. The sds string length must be divisible by 3.
 *
 * @param input sds string of RNA input
 * @returns sds protein string
 */
sds translate_rna_to_protein(sds input);


sds prot_rosalind_interface(FILE* stream); 

/* Related to solving the rna problem on rosalind */
sds transcribe_dna_to_rna(sds input); 

/** @brief calculate the total number of rabbit pairs after n months
 */

long calculate_breeding_pairs(int n, int k);

/** @brief stream interface for calling gc function from CLI
 */
sds gc_rosalind_interface(FILE* stream); 

double count_gc(sds);

int hamm_rosalind_interface(FILE* stream, sds result);

/**
 * @brief Calculate hamming distance of two sds strings
 *
 * Allowed input is two sds strings. If dissimilar in length
 * calculate the distance up to the length of the shorter one.
 *
 * Returns the hamming distance between the two strings.
 */
int hamming(sds input1, sds input2);

sds iprb_rosalind_interface(FILE* stream);

double calculate_mendelian_inheritance(double k, double m, double n);

/**
 * @brief Structure for returning results from substring functions
 */
typedef struct SubsResult {
	int len; /**< @param the number of substrings found */;
	int _size; 
	char* error; /**< Text error message */;
	int* substrings; /**< @param the array of results, dynamically allocated */
} SubsResult;

SubsResult* subs_find(sds string, sds substring);

sds subs_rosalind_interface(FILE* stream); 

/**
 * @brief ORF rosaline problem
 */
struct ProteinTranslation {
	int start;
	int end;
	sds protein;
	struct ProteinTranslation *next, *prev;
};

typedef struct ProteinTranslation ProteinTranslation;

sds orf_rosalind_interface(FILE* stream);
ProteinTranslation* build_orf(sds input);
int difference_orf(ProteinTranslation* a, ProteinTranslation* b, ProteinTranslation** missing); 

/**
 * @brief doubly linked list types for holding the overlap graph
 */
typedef struct ovl_Node {
	sds name1;
	sds name2;
	struct ovl_Node* next;
	struct ovl_Node* prev;
} ovl_Node;

typedef struct ovl_List {
	struct ovl_Node* head;
	struct ovl_Node* tail;
	size_t size;
} ovl_List;


/** 
 * @brief standard interface for calling this for the problem set 
 */
sds grph_rosalind_interface(FILE* stream);

/**
 * @brief - Determines if two strings overlap
 *
 * This function checks if the last `len` characters of `string1` overlap
 * with the first `len` characters of `string2`. It assumes that the strings
 * are managed with the Simple Dynamic Strings (SDS) library, which provides
 * functions like `sdslen` to determine string length.
 *
 * @string1: The first string to check.
 * @string2: The second string to check.
 * @len: The number of characters to consider for overlapping.
 *
 * Return: A boolean value indicating whether the two strings overlap.
 *         - `true` if the strings overlap.
 *         - `false` if the strings do not overlap or if either string is
 *           shorter than `len` characters.
 *
 * Note:
 * The function uses debugging statements to log the comparison process
 * for each character position considered in the overlapping check.
 */
bool _determine_if_overlapping(sds string1, sds string2, int n);

/**
 * @brief allocate a new list structure and create the overlap graph with with defined 
 * overlap amount
 */

/**
 * @brief Constructs an overlap graph from a set of sequences
 *
 * This function generates an overlap graph from the given set of sequences
 * by determining if any pair of sequences overlap by at least the specified
 * number of characters. The overlap graph is represented as an adjacency list
 * of overlapping sequences using a doubly linked list. This function handles
 * creation and allocation of memory of that list. Caller is responsible for freeing.
 *
 * @input: A pointer to a FastaStrings structure containing the sequences
 *         to be processed. Each sequence must have a name and a sequence
 *         string.
 * @overlap: The minimum number of overlapping characters required for two
 *           sequences to be considered overlapping.
 *
 * Return: A pointer to an ovl_List structure representing the overlap graph
 *         as an adjacency list, where each node in the list contains the
 *         names of overlapping sequences. This structure is allocated by this 
 *         function, caller is responsble for freeing.
 *
 * Note:
 * The function is at least O(n^2) 
 *
 * Note: compiled without NDEBUG, will output messages to stderr
 *
 */
ovl_List* ovl_overlap_graph(FastaStrings* input, int overlap);

/**
 * @brief initialize the overlap doubly linked list
 */
ovl_List* ovl_init(void);

/** 
 * @brief Given the doubly linked list, add a new node with the provided names
 */
ovl_List* ovl_add(ovl_List* nodes, sds name1, sds name2);

/**
 * Given two graph adjacency nodes sorted on name1 then name2, return a new linked list with the relationships that
 * are in nodes1 and not in nodes2. 
 *
 */
ovl_List* ovl_diff(ovl_List* nodes1, ovl_List* nodes2);

/** 
 * @brief Given two ovl nodes, return int as strcmp. This compare on the combination of name1 and name2. 
 */
/**
 * @brief Compares two ovl nodes based on their name name1 and name2
 *
 * This function compares two `ovl_Node` structures by their `name1` and `name2`
 * fields using lexicographical order. The comparison is performed first on the
 * `name1` field. If the `name1` fields are identical, it proceeds to compare
 * the `name2` fields.
 *
 * @node1: A pointer to the first `ovl_Node` to compare.
 * @node2: A pointer to the second `ovl_Node` to compare.
 *
 * Return: An integer less than, equal to, or greater than zero if `node1`
 *         is found, respectively, to be less than, to match, or to be greater
 *         than `node2`.
 *         - A negative value if `node1` is less than `node2`.
 *         - Zero if `node1` is equal to `node2`.
 *         - A positive value if `node1` is greater than `node2`.
 *
 * Note:
 * - Ensure that the `name1` and `name2` fields of `ovl_Node` are valid
 *   and non-null strings before invoking this function.
 */
int ovl_node_cmp(ovl_Node* node1, ovl_Node* node2);

/**
 * @brief Sorts a doubly linked list using insertion sort
 *
 * This function sorts an `ovl_List`, a doubly linked list, using the insertion
 * sort algorithm. It sorts based on ovl_node_cmp which orders on name1 and name2
 * in the ovl_Node.
 *
 * @list: A pointer to the `ovl_List` structure representing the list to be sorted.
 *        The list should be initialized prior to sorting, and it may be empty
 *        or contain one or more elements.
 *
 * Return: A pointer to the sorted `ovl_List`. 
 *
 * Note:
 *  - This sort works in place without requiring any additional ovl_Node to be
 *    created.
 *  - This is O(n^2) at the least.   	
 */
ovl_List* ovl_sort_insertion(ovl_List* nodes); 


/* @brief Formats a node's data into a string
 *
 * This function creates a formatted string representation of an `ovl_Node`.
 * It concatenates the `name1` and `name2` fields of the node into a single
 * string, separated by a space. The function uses the SDS (Simple Dynamic 
 * Strings) library for string manipulation.
 *
 * @to_print: A pointer to the `ovl_Node` whose data is to be formatted.
 *            It is assumed that `to_print` is not NULL and that the `name1`
 *            and `name2` fields are valid strings.
 *
 * Return: An `sds` string containing the formatted output of the node's data.
 *         The caller is responsible for freeing the returned `sds` string 
 *         using `sdsfree` when it is no longer needed.
 *
 * Note:
 * - This function assumes that the SDS library is properly linked and available
 *   for use.
 */
sds ovl_node_sdsprintf(ovl_Node* node);


void ovl_debug_list(ovl_Node* head);

/**
 * @brief Free all resources from the overlap graph:
 *  - the ovl_List
 *  - All ovl_Node
 *  - all sds strings referenced
 */
void ovl_free(ovl_List** nodes);
