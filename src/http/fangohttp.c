#include "fangohttp.h"

//global socket descriptor for server
static int sockfd;

//
//static char web_paths[10][100];
//static struct webpage (*web_funcs[10])(struct httpreq*);

//returns 0 on success or else -1
int listen_and_serve( domain, type, protocol, addr, port, blog  )
int domain, type, protocol, port, blog; const char* addr; {

  if( domain == 0 ) domain = DEF_DOMAIN;
  if( type == 0 ) type = DEF_SOCKTYPE;
  if( blog == 0 ) blog = DEF_BACKLOG;
  if( addr == NULL ) addr = DEF_ADDR;

  struct sockaddr_in server;
  memset( &server, 0, sizeof server );

  server.sin_port = htons( port );
  server.sin_addr.s_addr = INADDR_ANY;//inet_addr( addr );
  server.sin_family = domain;

  sockfd = socket( domain, type, protocol );
  if( sockfd < 0 ) {
    return -1;
  }

  fprintf( stdout, "SOCK_FD: %d\n", sockfd );

  if( bind( sockfd, (struct sockaddr*)&server, sizeof server )) {
    fprintf( stderr, "Bind: unccessfull \nPORT: %d\nADDR: %s\n", port, addr );
    close( sockfd );
    return -1;
  }
  else
    fprintf( stdout, "Bind: successfull \nPORT: %d\nADDR: %s\n", port, addr );

  if( listen( sockfd, blog ) < 0 ) {
    close( sockfd );
    return -1;
  }
  fprintf( stdout, "Server successfully started. CTRL + C to exit.\n\n\n" );
  return 0;
}

//close server descriptor returns 0 on success or else -1
int sock_close( int sockfd ) {
  return close( sockfd );
}

void freewebpage( struct webpage* wp ) {
  free( wp->content );
}

struct webpage serve( struct httpreq* req, const char* f_name, const char* func_name ) {
  char* webpagestr = malloc( 8000 ); //stores web page
  memset( webpagestr, 0, 8000 );

  printf( "Request header: %s\n", req->head );
  printf( "Function name:: %s\n", func_name );

  char* actfname = req->head + 5;
  char* act = strchr( actfname, ' ' );
  long int len = act - actfname;
  char final[len];
  strncpy( final, actfname, len );
  final[len] = '\0';

  if( read_html_file( len <= 1 ? "index.html" : final, webpagestr ) == -1 ) {
    char notfound[100] =
            "HTTP/1.1 404 Not Found\r\n\n"
            "<h1>404 not found!</h1>\r\n";
    strncpy( webpagestr, notfound, strlen( notfound ));
  }
  struct webpage webpg = {.content = webpagestr};
  return webpg;
}

//static int webpcounter = 0;
void path( const char* serv_path, struct webpage (*serv_func)(struct httpreq*) ) {
  //strcpy( web_paths[webpcounter], serv_path );
  //web_funcs[webpcounter++] = serv_func;

  struct httpreq hreq;

  int nsfd; //new socket file descriptor
	char buffer[2048]; //to store client request

  struct sockaddr_in cli_addr;
  socklen_t cli_len = sizeof cli_addr;

  //listening for requests on this path
  while( 1 ) {
    memset( &cli_addr, 0, sizeof cli_addr );
    if(( nsfd = accept( sockfd, (struct sockaddr*)&cli_addr, &cli_len )) < 0 ) {
      fprintf( stderr, "%s\n", "New connection error!" );
			continue;
		}

    memset( buffer, 0, 2048 );
		if( read( nsfd, buffer, 2047 ) < 0 ) {
      fprintf( stderr, "Error on reading request from client.\n" );
      close( nsfd );
      continue;
		}

    parsehttpreq( buffer, &hreq );
    printf( "After parsing:: %s\n", hreq.head );
    struct webpage webp = serv_func( &hreq );
    if( serve_webpage( nsfd, webp ) == -1 ) {
      fprintf( stderr, "Error serving HTML on path %s \n", serv_path );
    }
  }
}

//render html page returns 0 on success or else -1
int serve_webpage( int nsfd, struct webpage webp ) {
  if (write( nsfd, webp.content, strlen( webp.content )) < 0 ) {
    return -1;
  }
  sock_close( nsfd );
  freewebpage( &webp );
  return 0;
}
