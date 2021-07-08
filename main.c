#include "./src/http/fangohttp.h"

struct webpage* home( httpreq_t* req ) {
  return serve( req, "index.html" );
}

struct webpage* about( httpreq_t* req ) {
  return serve( req, "about.html" );
}

struct webpage* projects( httpreq_t* req ) {
  return serve( req, "projects.html" );
}

int main( int argc, char **argv ) {

  if( argc < 2 ) {
    printf( "Usage: %s <PORT>\n", argv[0] );
    return -1;
  }

  path( "/", home );
  path( "/about", about );
  path( "/projects", projects );

  if( listen_and_serve( AF_INET, SOCK_STREAM, 0, NULL, atoi( argv[1] ), 20 ) < 0 ) {
    fprintf( stderr, "Error opening socket.\n" );
    return 1;
  }

  return 0;
}
