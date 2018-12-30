/*
 * Utility Functions
 * 
 * these functions are frequently used as helpers,
 * yet they are not independent modules.
 */

#include <stdio.h>
#include <string.h>

char *bold(char *dest, char *src) {
	// enclose string <src> with HTML <b> tag
	sprintf(dest, "<b>%s</b>", src);
	return dest;
}

char *json_int_value(const unsigned char *key, int value, char *dest) {
	// create a K:V json string where value is integer
	sprintf(dest, "\"%s\":%d", key, value);
	return dest;
}

char *json_str_value(const unsigned char *key, const unsigned char *value, char *dest) {
	// create a K:V json string where value is string
	sprintf(dest, "\"%s\":\"%s\"", key, value);
	return dest;
}

int equal(char *str1, char *str2) {
	// return true if two strings are exactly equivalent to each other
	return !strcmp(str1, str2);
}

void header(char *key, char *value) {
	// create HTTP headers as parts of response
	printf("%s: %s\r\n", key, value);
}

void parse_post_data(char *post_data, int *math, int *physics, int *language) {
	// parse POST data & store scores in target integers
	int i; char *ptr;
	if((ptr = strstr(post_data, "math=")) != NULL)
		*math = strtol(ptr + 5, NULL, 10);
	else *math = 0;
	if((ptr = strstr(post_data, "physics=")) != NULL)
		*physics = strtol(ptr + 8, NULL, 10);
	else *physics = 0;
	if((ptr = strstr(post_data, "language=")) != NULL)
		*language = strtol(ptr + 9, NULL, 10);
	else *language = 0;
}

void parse_query_string(char *query_string, char *id, char *name) {
	// parse QUERY_STRING like `id=<id>&name=<name>` & store in target strings
	int i; char *ptr;
	// parsing string <id>
	if((ptr = strstr(query_string, "id=")) != NULL)
		for(ptr += 3, i = 0; ; i++)
			if(ptr[i] != '\0' && ptr[i] != '&')
				id[i] = ptr[i];
			else { id[i] = '\0'; break; }
	else id = "\0";
	// parsing string <name>
	if((ptr = strstr(query_string, "name=")) != NULL)
		for(ptr += 5, i = 0; ; i++)
			if(ptr[i] != '\0' && ptr[i] != '&')
				name[i] = ptr[i];
			else { name[i] = '\0'; break; }
	else name = "\0";
}

