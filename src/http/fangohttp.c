#include "fangohttp.h"

int accept_conn( struct sockaddr_in*, socklen_t* );

/*
* Serve given html file on given path.
*/
int serve_webpage( int, struct webpage );

//count number of webpaths added to application
static int webpcounter = 0;

//global socket descriptor for server
static int sockfd;

//
static char web_paths[10][100];
static struct webpage (*web_funcs[10])(struct httpreq*);

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

  struct sockaddr_in cli_addr;
  socklen_t cli_len;

  while( 1 ) {
      accept_conn( &cli_addr, &cli_len );
  }
  return 0;
}

int accept_conn( struct sockaddr_in *cli_addr, socklen_t* cli_len ) {
  int nsfd; //new socket file descriptor
	char buffer[2048]; //to store client request

  if(( nsfd = accept( sockfd, (struct sockaddr*)cli_addr, cli_len )) < 0 ) {
    fprintf( stderr, "%s\n", "New connection error!" );
    return -1;
  }

  memset( buffer, 0, 2048 );
  if( read( nsfd, buffer, 2047 ) < 0 ) {
    fprintf( stderr, "Error on reading request from client.\n" );
    close( nsfd );
    return -1;
  }

  printf( "\nClient request: \n\n%s\n", buffer );

  httpreq_t hreq;
  memset( &hreq, 0, sizeof hreq );
  hreq = parsehttpreq( buffer );

  char* file = rem_file_extn( hreq.file );

  struct webpage (*call)(httpreq_t*);
  struct webpage webp;

  if( file != NULL ) {
    int found = 0;
    if( strncmp( file, "index", 5 ) == 0 ) {
      printf( "DEBUG:: Request for index file.\n" );
      for( int idx = 0; idx < webpcounter; idx++ ) {
        if( strcmp( web_paths[idx], "/" ) == 0 ) {
            call = web_funcs[idx];
            webp = call( &hreq );
            found = 1;
            break;
        }
      }
    }
    else {
      for( int idx = 0; idx < webpcounter; idx++ ) {
        if( strcmp( web_paths[idx], file ) == 0 ) {
            call = web_funcs[idx];
            webp = call( &hreq );
            found = 1;
            break;
        }
      }
    }

    if( found ) {
      if( serve_webpage( nsfd, webp ) == -1 ) {
        fprintf( stderr, "Error serving HTML on path \n" );
        return -1;
      }
    }
    else {
      char notfound[100] =
              "HTTP/1.1 404 Not Found\r\n\n"
              "<h1>404 not found!</h1>\r\n";
      write( nsfd, notfound, strlen( notfound ));
    }
  }
  else {
    printf( "DEBUG:: Request for index file.\n" );
    for( int idx = 0; idx < webpcounter; idx++ ) {
      if( strcmp( web_paths[idx], "/" ) == 0 ) {
          call = web_funcs[idx];
          webp = call( &hreq );
          break;
      }
    }
    if( serve_webpage( nsfd, webp ) == -1 ) {
      fprintf( stderr, "Error serving HTML on path \n" );
      return -1;
    }
  }
  free( file );
  return 0;
}

//close server descriptor returns 0 on success or else -1
int sock_close( int sockfd ) {
  return close( sockfd );
}

void freewebpage( struct webpage* wp ) {
  free( wp->content );
}

struct webpage serve( httpreq_t* req, const char* f_name, const char* func_name ) {
  char* webpagestr = malloc( 8000 ); //stores web page
  memset( webpagestr, 0, 8000 );

  printf( "Request header: %s\n", req->head );
  printf( "Request file: %s\n", req->file );
  printf( "Function name:: %s\n", func_name );

  if( *f_name == '\0' ) {
    printf( "Empty file\n" );
  }

  if( read_html_file( req->file, webpagestr ) == -1 ) {
    char notfound[100] =
            "HTTP/1.1 404 Not Found\r\n\n"
            "<h1>404 not found!</h1>\r\n";
    strncpy( webpagestr, notfound, strlen( notfound ));
  }

  struct webpage webpg = {.content = webpagestr};
  return webpg;
}

void path( const char* serv_path, struct webpage (*serv_func)(httpreq_t*) ) {
  strcpy( web_paths[webpcounter], serv_path );
  web_funcs[webpcounter++] = serv_func;
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
