#ifndef HTTP_REQ_PARSE_H
#define HTTP_REQ_PARSE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utils/file_util.h"

typedef struct httpreq {
  char* head;
  char* file;
} httpreq_t;

/**
* Parse first line of the HTTP request.
* @param const char*   Request sent by client.
* @return              0 on success and -1 on failure.
*/
httpreq_t parsehttpreq( const char[] );

void freehttpreq( httpreq_t* );

#endif
