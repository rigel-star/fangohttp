#ifndef FANGOHTTP_H
#define FANGOHTTP_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "httpreq.h"
#include "../utils/file_util.h"

#define DEF_PORT 10000
#define DEF_ADDR "127.0.0.1"
#define DEF_BACKLOG 10
#define DEF_DOMAIN AF_INET
#define DEF_SOCKTYPE SOCK_STREAM

extern const char* HTTP_404_NOT_FOUND;
extern const char* HTTP_200_OK;
extern const char* HTTP_501_NOT_IMPLTD;

typedef struct webpage {
  char* content;
} webpage_t;

int listen_and_serve( int, int, int, const char*, const int, int );

//close socket
int sock_close( int );

/**
* @param struct httpreq*
* @param const char*    File name to serve.
*/
struct webpage* serve( httpreq_t*, const char* );

void freewebpage( struct webpage* );

/**
* @param const char*    URL path to serve a given http request.
*/
void path( const char*, struct webpage* (*)(httpreq_t*) );

#endif
