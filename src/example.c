/*
 ============================================================================
 Name        : IniParser.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ini.h"

bool callback(const char *section, char* field, char* value, ini_app_data_t app_data);

int main(void) {
	ini_result_e rval =
		ini_pipe_modify("sample.conf", callback, "stuff", "out.conf");

	if (rval != INI_RESULT_SUCCESS) {
		printf("ini_pipe_modify failed (rval: %d)!\n", rval);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

bool callback(const char *section, char* field, char* value, ini_app_data_t app_data) {
	char* new_host_name = (char*) app_data;

	if (strcmp(section, "server") == 0) {
		if (strcmp(field, "host-name") == 0) {
			strcpy(value, new_host_name);
		}
	}

	return true;
}
