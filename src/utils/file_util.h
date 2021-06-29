#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
* Read file name pointed by first arg into second arg
* return 0 on success or else -1
*/
int read_html_file( const char[], char[] );

/*
* Reads given file content into given string
*/
int read_file( FILE*, char[] );

/*
* Get file name using file pointer
*/
char* get_file_name( FILE* );

#endif
