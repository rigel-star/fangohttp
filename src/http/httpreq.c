#include "httpreq.h"

int parsehttpreq( const char reqstr[], struct httpreq* hreq ) {

  if( reqstr == NULL ) return -1;

  memset( hreq, 0, sizeof *hreq ); //nullify http request in case
                                  //if it is populated already

  hreq->head = malloc( 100 );;

  const char* delim = "\n";

  char* token = strtok( reqstr, delim );
  const size_t len = strlen( token );
  strncpy( hreq->head, token, len );
  hreq->head[(int)len] = '\0';
  return 0;
}

void freehttpreq( struct httpreq* hreq ) {
}
