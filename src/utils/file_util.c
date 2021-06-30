#include "file_util.h"

//TODO: USE HASH TABLE
int read_html_file( const char f_name[], char dest[] ) {
  strcat( dest, "HTTP/1.1 200 OK\r\n\n" );
  char ff_name[40];
  memset( ff_name, 0, 40 );
  strcat( ff_name, "./templates/" );
  strncat( ff_name, f_name, strlen( f_name ));

  FILE* fp = fopen( ff_name, "r" );
  if( read_file( fp, dest ) < 0 ) {
    return -1;
  }
	fclose( fp );
  return 0;
}

int read_file( FILE* fp, char dest[] ) {
  char* f_name = get_file_name( fp );
  if ( fp == NULL ) {
    fprintf( stderr, "%s file missing.\n", f_name );
    return -1;
  }

  char line[100];
	while( fgets( line, 100, fp )) {
		strcat( dest, line );
	}
  free( f_name );
  return 0;
}

char* get_file_name( FILE* fp ) {
  int MAXSIZE = 0xFFF;
  char proclnk[0xFFF];
  char* filename = malloc( 255 );

  if ( fp != NULL ) {
      int fno = fileno( fp );
      sprintf(proclnk, "/proc/self/fd/%d", fno);
      ssize_t r = readlink( proclnk, filename, MAXSIZE );
      if ( r < 0 ) {
          fprintf( stderr, "Failed to readlink.\n" );
          return NULL;
      }
      filename[r] = '\0';
      return filename;
  }
  else {
    return NULL;
  }
}

char* rem_file_extn( const char* f_name ) {
  char* nm = strchr( f_name, '.' );
  if( nm ) {
    const long int len = nm - f_name;
    char* ret = malloc( len + 1 );
    strncpy( ret, f_name, len );
    ret[len] = '\0';
    return ret;
  }
  return NULL;
}
