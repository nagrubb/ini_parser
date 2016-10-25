#include "ini.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INI_MAX_STRING_LEN 256
#define INI_LINE_BUFFER_LEN 512

static bool ini_parse_new_section(const char* line, char section_name[INI_MAX_STRING_LEN]) {
	if (sscanf(line, "[%[^]]", section_name) == 1) {
		return true;
	}

	return false;
}

static bool ini_parse_key_value(const char* line, char key[INI_MAX_STRING_LEN], char value[INI_MAX_STRING_LEN]) {
	char equal = 0;
	int parsed_values = 0;

	parsed_values = sscanf(line, "%[^=]%[=]%[^\n]", key, &equal, value);

	if (parsed_values == 3) {
		return true;
	}

	return false;
}

ini_result_e ini_pipe_modify(const char* in_filename, ini_modify_cb app_callback, ini_app_data_t app_data, const char* out_filename) {
	FILE* in_fp = NULL;
	FILE* out_fp = NULL;
	char section[INI_MAX_STRING_LEN];
	char field[INI_MAX_STRING_LEN];
	char value[INI_MAX_STRING_LEN];
	char buffer[INI_LINE_BUFFER_LEN];
	size_t buffer_pos = 0;
	char new_char = 0;
	int rval = -1;
	bool first_section = true;

	in_fp = fopen(in_filename, "r");

	if (!in_fp) {
		rval = INI_RESULT_FAILED_TO_OPEN_INPUT_FILE;
		goto done;
	}

	out_fp = fopen(out_filename, "w");

	if (!out_fp) {
		rval = INI_RESULT_FAILED_TO_OPEN_OUTPUT_FILE;
		goto done;
	}

	do {
		new_char = getc(in_fp);

		if (new_char == EOF || new_char == '\n') {
			//process line
			buffer[buffer_pos] = '\0';

			if (buffer[0] != '#' && strcmp(buffer, "") != 0) {
				if (ini_parse_new_section(buffer, section)) {
					//Piping new section
					if (first_section) {
						strcpy(buffer, "[");
						first_section = false;
					} else {
						strcpy(buffer, "\n[");
					}
					strcat(buffer, section);
					strcat(buffer, "]\n");
					fputs(buffer, out_fp);
				} else if (ini_parse_key_value(buffer, field, value)) {
					//Piping key value pair
					if (app_callback(section, field, value, app_data)) {
						strcpy(buffer, field);
						strcat(buffer, "=");
						strcat(buffer, value);
						strcat(buffer, "\n");
						fputs(buffer, out_fp);
					}
				}
			}

			buffer_pos = 0;
		} else {
			if (buffer_pos == INI_LINE_BUFFER_LEN) {
				rval = INI_RESULT_LINE_OVERFLOW;
				goto done;
			}

			if (new_char != '\r') {
				buffer[buffer_pos++] = new_char;
			}
		}
	} while (new_char != EOF);

	rval = INI_RESULT_SUCCESS;

done:
	if (in_fp) {
		fclose(in_fp);
		in_fp = NULL;
	}

	if (out_fp) {
		fclose(out_fp);
		out_fp = NULL;
	}

	return rval;
}
