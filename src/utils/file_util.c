#include "file_util.h"

char* get_file_content( FILE* fp ) {
  if ( fp == NULL ) {
    fprintf( stderr, "File missing.\n" );
    return NULL;
  }

  fseek( fp, 0, SEEK_END );
  long fsize = ftell( fp );
  fseek( fp, 0, SEEK_SET );  /* same as rewind(f); */

  char *string = malloc( fsize + 1 );
  fread( string, 1, fsize, fp );

  string[fsize] = 0;
  return string;
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

char* get_file_extn( const char* f_name ) {
  char* ext = strchr( f_name, '.' ) + 1;
  if( ext ) return ext;
  return NULL;
}
