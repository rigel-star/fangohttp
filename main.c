#include "./src/http/fangohttp.h"


/**
** This is example for using fangohttp to host HTML files over LAN.
**
** RULES
** 1) HTML files should be put on templates folder.
** 2) CSS, JS files should be put on static folder.
**
** As you can see below, home(), about(), and projects()
** are some functions which take httpreq_t* as argument and
** returns webpage*. This functions are created in order to
** serve HTML files at specific path.
**
** Function path:: path(char*, struct webpage* (*)(httpreq_t*))
** Available URL paths in application must be added before they
** can be accessed. First parameter of path() takes URL path and
** second one takes function pointer which handles the behavior of
** that URL(home(), about(), and projects() in our example).
**
** For example: 127.0.0.1/about.html will be handled by home() because
** in path() function below, "/about" is given as first argument to path
** and about() function is given as second argument.
**/



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
