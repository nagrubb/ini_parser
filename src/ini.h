#ifndef __INI_H__
#define __INI_H__

#include <stdbool.h>

typedef enum {
	INI_RESULT_SUCCESS = 0,
	INI_RESULT_FAILED_TO_OPEN_INPUT_FILE,
	INI_RESULT_FAILED_TO_OPEN_OUTPUT_FILE,
	INI_RESULT_LINE_OVERFLOW
} ini_result_e;

typedef void* ini_app_data_t;
typedef bool (*ini_modify_cb)(const char *section, char* field, char* value, ini_app_data_t app_data);

ini_result_e ini_pipe_modify(const char* in_filename, ini_modify_cb app_callback, ini_app_data_t app_data, const char* out_filename);

#endif //__INI_H__
