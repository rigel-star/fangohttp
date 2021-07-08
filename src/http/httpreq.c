#include "httpreq.h"

httpreq_t* parsehttpreq( const char* reqstr ) {

  if( *reqstr == '\0' || reqstr == NULL )
    return NULL;

  httpreq_t* hreq = malloc(sizeof(httpreq_t));

  char* head = strtok( (char*)reqstr, "\n" );
  const size_t lenhead = strlen( head );

  hreq->HEAD = malloc(lenhead);
  strncpy( hreq->HEAD, head, lenhead );
  hreq->HEAD[lenhead] = 0;

  const char* emethod = strchr( reqstr, ' ' );
  char method[emethod - reqstr];
  strncpy( method, reqstr, emethod - reqstr );

  const char *start_of_path = strchr(reqstr, ' ') + 1;
  const char *end_of_path = strchr(start_of_path, ' ');

  char path[end_of_path - start_of_path];
  strncpy(path, start_of_path,  end_of_path - start_of_path);

  char* ver = strchr( start_of_path, ' ' ) + 1;
  char* ever = strchr( ver, '\0' );
  char version[ever - ver];
  strncpy( version, ver, ever - ver );

  size_t path_sz = sizeof path;
  size_t meth_sz = sizeof method;
  size_t ver_sz = sizeof version;

  path[path_sz] = 0;
  method[meth_sz] = 0;
  version[ver_sz] = 0;

  hreq->URI = malloc(path_sz);
  memset( hreq->URI, 0, path_sz );
  strncpy( hreq->URI, path, path_sz );

  hreq->METHOD = malloc(meth_sz);
  memset( hreq->METHOD, 0, meth_sz );
  strncpy( hreq->METHOD, method, meth_sz );

  hreq->VERSION = malloc(ver_sz);
  memset( hreq->VERSION, 0, ver_sz );
  strncpy( hreq->VERSION, version, ver_sz );

  return hreq;
}

void freehttpreq( httpreq_t* hreq ) {
  free( hreq->HEAD );
  free( hreq->METHOD );
  free( hreq->URI );
  free( hreq->VERSION );
  free( hreq );
}
