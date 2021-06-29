#include "httpreq.h"

int parsehttpreq( const char reqstr[], struct httpreq* hreq ) {

  if( reqstr == NULL ) return -1;

  memset( hreq, 0, sizeof *hreq ); //nullify http request in case
                                  //if it is populated already

  hreq->head = malloc( 100 );
  // hreq->host = malloc( 50 );
  // hreq->user_agent = malloc( 150 );
  // hreq->accept = malloc( 100 );
  // hreq->accept_language = malloc( 30 );
  // hreq->accept_encoding = malloc( 30 );
  // hreq->connection = malloc( 20 );
  // hreq->referer = malloc( 50 );
  // hreq->cookie = malloc( 80 );

  const char* delim = "\n";

  //int index = 0;
  //char tokens[9][100];
  //memset( tokens, 0, sizeof tokens );

  char* token = strtok( reqstr, delim );
  const size_t len = strlen( token );
  strncpy( hreq->head, token, len );
  hreq->head[(int)len] = '\0';

  // do {
  //   if( token ) {
  //     //const char* tmp = strchr( token, ':' );
  //     printf( "%s\n", token );
  //   }
  // } while((( token = strtok( NULL, delim )), token != NULL ));

  // strncpy( hreq->head, tokens[0], strlen( tokens[0] ));
  // strncpy( hreq->host, tokens[1], strlen( tokens[1] ));
  // strncpy( hreq->user_agent, tokens[2], strlen( tokens[2] ));
  // strncpy( hreq->accept, tokens[3], strlen( tokens[3] ));
  // strncpy( hreq->accept_language, tokens[4], strlen( tokens[4] ));
  // strncpy( hreq->accept_encoding, tokens[5], strlen( tokens[5] ));
  // strncpy( hreq->connection, tokens[6], strlen( tokens[6] ));
  // strncpy( hreq->referer, tokens[7], strlen( tokens[7] ));
  // strncpy( hreq->cookie, tokens[8], strlen( tokens[8] ));

  return 0;
}

void freehttpreq( struct httpreq* hreq ) {
  free( hreq->head );
  // free( hreq->host );
  // free( hreq->user_agent );
  // free( hreq->accept );
  // free( hreq->accept_language );
  // free( hreq->accept_encoding );
  // free( hreq->connection );
  // free( hreq->referer );
  // free( hreq->cookie );
}
