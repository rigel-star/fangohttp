#include "re.h"
#include <stdio.h>

int match( const char* str, char* pattern ) {
  int    status;
  regex_t    re;

  if( regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) != 0 ) {
      return 0;      /* Report error. */
  }

  status = regexec( &re, str, (size_t) 0, NULL, 0 );
  regfree( &re );

  if( status != 0 ) {
      return 0;      /* Report error. */
  }
  return 1;
}
