#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
* Reads given file content into given string
*/
char* get_file_content( FILE* );

/*
* Get file name using file pointer
*/
char* get_file_name( FILE* );

/**
* Get file name without its extension
* @param const char*  File name.
*/
char* rem_file_extn( const char* );

/**
* Get file extension.
*@ const char*  File name.
*/
char* get_file_extn( const char* );

#endif
