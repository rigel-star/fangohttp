#include "httpreq.h"

httpreq_t parsehttpreq( const char reqstr[] ) {

  if( reqstr == NULL ) {
    httpreq_t hr = {.head="GET / HTTP/1.1", .file="index.html"};
    return hr;
  }

  httpreq_t hreq;

  memset( &hreq, 0, sizeof hreq ); //nullify http request in case
                                  //if it is populated already
  hreq.head = malloc( 100 );
  hreq.file = malloc( 50 );

  char* token = strtok( (char*)reqstr, "\n" );
  const size_t lenhead = strlen( token );
  strncpy( hreq.head, token, lenhead );
  hreq.head[(int)lenhead] = '\0';

  char* actfname = token + 5;
  char* act = strchr( actfname, ' ' );
  long int lenfile = act - actfname;
  char final[30];
  memset( final, 0, 30 );
  strncpy( final, actfname, lenfile );
  final[lenfile] = '\0';

  (lenfile <= 1 || final == NULL) ? strcpy( hreq.file, "index.html\0" ) :
                  strcpy( hreq.file, final );

  return hreq;
}

void freehttpreq( httpreq_t* hreq ) {
  free( hreq->head );
  free( hreq->file );
}
