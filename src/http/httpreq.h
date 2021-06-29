#ifndef HTTP_REQ_PARSE_H
#define HTTP_REQ_PARSE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct httpreq {
  char* head;
} httpreq_t;

/**
* Parse first line of the HTTP request.
* @param const char*   Request sent by client.
* @return              0 on success and -1 on failure.
*/
int parsehttpreq( const char*, struct httpreq* );

void freehttpreq( struct httpreq* );

#endif
