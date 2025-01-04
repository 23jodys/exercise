#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <curl/curl.h>

#include "librosalind.h"

sds mprt_protein_sequence_cleanup(sds raw); 


sds mprt_rosalind_interface(FILE* stream) {
	sds result = sdsempty();
	sds regex = sdsnew("N[^P][ST][^P]");
	log_info("Using motif pattern: '%s'", regex);

	/* read protein IDs from stream */
	size_t bufsize = 0; //susp
	char* buffer = NULL;
	ssize_t lineSize = 0; 
	int line_counter = 0;

	/* for each protein ID */
	while((lineSize = getline(&buffer, &bufsize, stream)) != -1) {
		line_counter++;
		sds protein_id = sdsnew(buffer);
		sdstrim(protein_id, " \t\n");
		debug("found protein_id on stdin: '%s'", protein_id);
		int count;
		sds* protein_id_parts = sdssplitlen(protein_id, sdslen(protein_id), "_", 1, &count);
		sds accession_id = protein_id_parts[0];

		/* fetch sequence from id */
		sds protein_sequence = mprt_protein_sequence_fetch(accession_id);
		debug("Fetched '%s' for protein '%s' with length %zu", protein_sequence, protein_id, sdslen(protein_sequence));

		/* find motifs */
		mprt_ResultList* motif_result = mprt_result_list_init();
		motif_result = mprt_motif_find(protein_sequence, regex, motif_result);

		/* make string and add to result */
		if (motif_result->size > 0) { 
			result = sdscat(result, protein_id);
			result = sdscat(result, "\n");
			result = sdscat(result, mprt_list_sprint(motif_result));
			result = sdscat(result, "\n");
		} else {
			debug("no matches found for protein_id: '%s", protein_id);
		}

		mprt_result_list_free(&motif_result);
	}

	sdstrim(result,"\n");

	/* return result */
	return result;
}

sds mprt_list_sprint(mprt_ResultList* list) {
	sds result = sdsempty();
	if (list == NULL) {
		goto outro;
	}

	mprt_ResultNode* node = list->head;

	while(node != NULL) {
		result = sdscatprintf(result, "%d ", node->value + 1);
		node = node->next;
	}

	sdsrange(result,0,sdslen(result) - 2);
	
	outro:
	return result;
}

struct MemoryStruct {
	sds memory;
	size_t size;
};

static size_t write_memory_callback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct*) userp;
	debug("realsize: %zu, size: %zu, nmemb: %zu", realsize, size, nmemb);
	mem->memory = sdscatlen(mem->memory, (char*) contents, realsize);
	mem->size += realsize;

	return realsize;
}


sds mprt_protein_sequence_fetch(sds protein_id) {
	sds url = sdsempty();
	url = sdscatprintf(url, "https://rest.uniprot.org/uniprotkb/%s.fasta", protein_id);

	struct MemoryStruct chunk = {.memory = sdsempty(), .size = 0};

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	debug("attempting to fetch from '%s'", url);
	CURLcode res = curl_easy_perform(curl_handle);

	if(res += CURLE_OK) {
		log_err("curl_easy_perform() failed: %s", curl_easy_strerror(res));
	} else {
		debug("%lu bytes retreived", (unsigned long) chunk.size);
	}

	curl_easy_cleanup(curl_handle);

	curl_global_cleanup();

	/* 
	 >sp|B5ZC00|SYG_UREU1 Glycine--tRNA ligase OS=Ureaplasma urealyticum serovar 10 (strain ATCC 33699 / Western) OX=565575 GN=glyQS PE=3 SV=1
	 MKNKFKTQEELVNHLKTVGFVFANSEIYNGLANAWDYGPLGVLLKNNLKNLWWKEFVTKQ
	 KDVVGLDSAIILNPLVWKASGHLDNFSDPLIDCKNCKARYRADKLIESFDENIHIAENSS
	 NEEFAKVLNDYEISCPTCKQFNWTEIRHFNLMFKTYQGVIEDAKNVVYLRPETAQGIFVN
	 FKNVQRSMRLHLPFGIAQIGKSFRNEITPGNFIFRTREFEQMEIEFFLKEESAYDIFDKY
	 LNQIENWLVSACGLSLNNLRKHEHPKEELSHYSKKTIDFEYNFLHGFSELYGIAYRTNYD
	 LSVHMNLSKKDLTYFDEQTKEKYVPHVIEPSVGVERLLYAILTEATFIEKLENDDERILM
	 DLKYDLAPYKIAVMPLVNKLKDKAEEIYGKILDLNISATFDNSGSIGKRYRRQDAIGTIY
	 CLTIDFDSLDDQQDPSFTIRERNSMAQKRIKLSELPLYLNQKAHEDFQRQCQK
	*/
	return mprt_protein_sequence_cleanup(chunk.memory);
}

sds mprt_protein_sequence_cleanup(sds raw) {
	int count;
	sds result = sdsempty();
	sds* tokens = sdssplitlen(raw, sdslen(raw), "\n", 1, &count); 

	if (count < 2) return raw;

	for (int i = 1; i < count; i++) {
		debug("token: %s", tokens[i]);
	}

	result = sdsjoinsds(tokens + 1, count - 1, "", 0);

	return result;
}
	
mprt_ResultList* mprt_result_list_init(void) {
	mprt_ResultList* result = malloc(sizeof(mprt_ResultList));
	if (result == NULL) {
		printf("out of memory\n");	
		exit(-1);
	}
	return result;
}

void mprt_result_list_free(mprt_ResultList** to_free) {


}

mprt_ResultList* mprt_result_list_add(mprt_ResultList* list, int value, sds matched) {
	if (list == NULL) return list;

	mprt_ResultNode* new_tail = malloc(sizeof(mprt_ResultNode));

	mprt_ResultNode* old_tail = list->tail;

	if(old_tail) {
		old_tail->next = new_tail;
	} else {
		list->head = new_tail;
	}

	new_tail->value = value;
	new_tail->matched = matched;

	new_tail->next = NULL;
	new_tail->prev = old_tail;

	list->tail = new_tail;

	list->size += 1;

	return list;
}

mprt_ResultList*  mprt_motif_find(sds protein_sequence, sds regex, mprt_ResultList* result) {
	if (result == NULL) return result;

	int errornumber;
	PCRE2_SIZE erroroffset;
	pcre2_code* re = pcre2_compile((PCRE2_SPTR)regex, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
	pcre2_match_data *match_data = NULL;

	if (re == NULL) {
		PCRE2_UCHAR buffer[256];
		pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
		log_err("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
		goto outro;
	}

	match_data = pcre2_match_data_create_from_pattern(re, NULL);

	debug("%zu length for '%s'", sdslen(protein_sequence), protein_sequence);
	PCRE2_SIZE start_offset = 0;
	for(;;) {

		int rc = pcre2_match(re, (PCRE2_SPTR)protein_sequence, sdslen(protein_sequence), start_offset, 0, match_data, NULL);
		debug("pcre2_match rc: %d", rc);
		if (rc < 0) {
			switch(rc) {
				case PCRE2_ERROR_NOMATCH: log_err("No match\n"); break;
				default: log_err("Matching error %d\n", rc); break;
			}
			goto outro;
		}

		PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);
		for (int i = 0; i < rc; i++) {
			sds matched = sdsdup(protein_sequence);
			sdsrange(matched, ovector[2*i],  ovector[2*i + 1] - 1);
			log_info("found match at %zu: '%s'", ovector[2*i], matched);
			mprt_result_list_add(result, ovector[2*i], matched);
			//start_offset = ovector[2*i + 1];
			start_offset = ovector[2*i] + 1;
		}
	}

	outro:
	pcre2_match_data_free(match_data);   /* Release memory used for the match */
	pcre2_code_free(re);                 /* data and the compiled pattern. */
	return result;
}

