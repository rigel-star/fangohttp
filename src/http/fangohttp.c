#include "fangohttp.h"

const char* HTTP_404_NOT_FOUND = "HTTP/1.1 404 Not Found\r\n\n";
const char* HTTP_200_OK = "HTTP/1.1 200 OK\r\n\n";
const char* HTTP_501_NOT_IMPLTD = "HTTP/1.1 501 Not Implemented\r\n\n";

/*
* Accept incoming connection.
*/
static int accept_conn( struct sockaddr_in*, socklen_t* );

/*
* Serve file.
*/
static int serve_file( int, FILE* );

/*
* Serve given html file on given path.
*/
static int serve_webpage( int, struct webpage* );

//count number of webpaths added to application
static int webpcounter = 0;

//global socket descriptor for server
static int sockfd;

//
static char web_paths[10][100];
static struct webpage* (*web_funcs[10])(struct httpreq*);

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
      if( accept_conn( &cli_addr, &cli_len ) < 0 ) {
        fprintf( stderr, "Problem on accepting connection or sending.\n" );
      }
  }
  return 0;
}

static int accept_conn( struct sockaddr_in *cli_addr, socklen_t* cli_len ) {
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

  httpreq_t* hreq = parsehttpreq( buffer );

  struct webpage* (*call)(httpreq_t*);
  struct webpage* webp = malloc(sizeof(webpage_t));

  if( hreq == NULL ) {
    char notfound[] =
            "HTTP/1.1 501 Not Implemented\r\n\n"
            "<h1>Request not implemented.</h1>\r\n";
    if( write( nsfd, notfound, sizeof notfound ) < 0 ) {
      fprintf( stderr, "Error on writing\n" );
      return -1;
    }
    return 0;
  }

  if( strcmp( hreq->URI, "/favicon.ico" ) == 0 ) {
    int favfd = open( "./favicon.ico", O_RDONLY );
    if( sendfile( nsfd, favfd, NULL, 950 ) <= 0 ) {
      fprintf( stderr, "%s\n", "Error on sending favicon: favicon.ico" );
      close( favfd );
      return -1;
    }
    close( favfd );
    return 0;
  }

  char search_for[50];
  memset( search_for, 0, 50 );
  bool req_for_pg = false;

  if( strlen( hreq->URI ) <= 1 ) {
    search_for[0] = '/';
    req_for_pg = true;
    printf( "search_for: index\n" );
  }
  else {
    if( strncmp(get_file_extn(hreq->URI), "html", 5) != 0 ) {
      char actf_name[40];
      memset( actf_name, 0, 40 );
      strcat( actf_name, "./static/" );
      strcat( actf_name, hreq->URI + 1 );
      actf_name[sizeof actf_name] = 0;
      strncpy(search_for, actf_name, strlen(actf_name));
      printf( "search_for: static file\n" );
    }
    else {
      req_for_pg = true;
      strcat( search_for, hreq->URI );
      printf( "search_for: non_index.html\n" );
    }
  }

  search_for[strlen(search_for)] = 0;

  if( req_for_pg ) {
    char act_search[30];
    memset( act_search, 0, 30 );
    if( strcmp( search_for, "/" ) != 0 ) { //hoping html file name is greater than 1 char
      char* path = rem_file_extn( search_for );
      strcpy( act_search, path );
      free( path );
    }
    else
      strcpy( act_search, "/" );

    act_search[sizeof act_search] = 0;

    for( int i = 0; i < 10; i++ ) {
      if( strcmp( web_paths[i], act_search ) == 0 ) {
        call = web_funcs[i];
        webp = call( hreq );
        break;
      }
    }
    if( serve_webpage( nsfd, webp ) == -1 ) {
      fprintf( stderr, "Error serving HTML on path %s\n", hreq->URI );
    }
  }
  else {
    FILE* fp = fopen( search_for, "r" );
    if( fp == NULL ) {
      if( write( nsfd, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND) ) < 0 ) {
        fprintf(stderr, "Error on writing\n");
        fclose(fp);
        return -1;
      }
    }
    else {
      if( serve_file( nsfd, fp ) == -1 ) {
        fprintf( stderr, "Error serving file %s\n", search_for );
        fclose( fp );
        return -1;
      }
    }
    fclose( fp );
  }
  return 0;
}

struct webpage* serve( httpreq_t* req, const char* f_name ) {

  printf( "Request header: %s\n", req->HEAD );
  printf( "Request URI: %s\n", req->URI );
  printf( "File name to serve: %s\n", f_name );

  char actf_name[40];
  memset( actf_name, 0, 40 );
  strcat( actf_name, "./templates/" );
  strcat( actf_name, f_name );
  actf_name[sizeof actf_name] = 0;

  printf( "%s\n", actf_name );

  FILE* fp = fopen( actf_name, "r" );
  char* webpagestr = get_file_content( fp );

  struct webpage* webpg = malloc(sizeof(webpage_t));
  size_t wpsz = 0;

  if( webpagestr == NULL ) {
    wpsz = sizeof HTTP_404_NOT_FOUND;
    webpg->content = malloc(wpsz + 1);
    webpg->content[wpsz] = 0;

    strncpy( webpg->content, HTTP_404_NOT_FOUND, wpsz );
    strcat( webpg->content, "<h1>404 not found!</h1>\n" );
  }
  else {
    size_t hsz = strlen( HTTP_200_OK );
    wpsz = strlen( webpagestr );

    webpg->content = malloc(wpsz + hsz + 1);
    webpg->content[wpsz] = 0;

    strncat( webpg->content, HTTP_200_OK, hsz );
    strncat( webpg->content, webpagestr, wpsz );
  }
  return webpg;
}

void path( const char* serv_path, struct webpage* (*serv_func)(httpreq_t*) ) {
  strcpy( web_paths[webpcounter], serv_path );
  web_funcs[webpcounter++] = serv_func;
}

//close server descriptor returns 0 on success or else -1
int sock_close( int sockfd ) {
  return close( sockfd );
}

void freewebpage( struct webpage* wp ) {
  free( wp->content );
  free( wp );
}

//Sends webpage to clients. Returns 0 on success or else -1.
static int serve_webpage( int nsfd, struct webpage* webp ) {
  if (write( nsfd, webp->content, strlen( webp->content )) < 0 ) {
    return -1;
  }
  sock_close( nsfd );
  freewebpage( webp );
  return 0;
}

static int serve_file( int nsfd, FILE* fp ) {
  char* ct = get_file_content( fp );
  if (write( nsfd, ct, strlen( ct )) < 0 ) {
    return -1;
  }
  free( ct );
  sock_close( nsfd );
  return 0;
}
