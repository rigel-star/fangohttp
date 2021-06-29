#include "./src/http/fangohttp.h"

struct webpage home( struct httpreq* req ) {
  return serve( req, "index.html", __func__ );
}

struct webpage about( struct httpreq* req ) {
  return serve( req, "about.html", __func__ );
}

int main( int argc, char **argv ) {

  if( argc < 2 ) {
    printf( "Usage: %s <PORT>\n", argv[0] );
    return -1;
  }

  if( listen_and_serve( AF_INET, SOCK_STREAM, 0, NULL, atoi( argv[1] ), 20 ) < 0 ) {
    fprintf( stderr, "Error opening socket.\n" );
    return 1;
  }

  //path( "/", home );
  path( "/about", about );
  return 0;
}
