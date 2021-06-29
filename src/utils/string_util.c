#include "string_util.h"

void replace_w_array( char* str, char* old, char** arr, int n ) {

}

char* replace( char* s, char* oldW, char* newW ) {
  char* result;
  int newWlen = strlen( newW );
  int oldWlen = strlen( oldW );

  // Counting the number of times old word
  // occur in the string
  int i, cnt = 0;
  for( i = 0; s[i] != '\0'; i++ ) {
      if ( strstr(&s[i], oldW) == &s[i] ) {
          cnt++;
          // Jumping to index after the old word.
          i += oldWlen - 1;
      }
  }

  // Making new string of enough length
  result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

  i = 0;
  while (*s) {
      // compare the substring with the result
      if (strstr(s, oldW) == s) {
          newWlen = strlen( newW );
          strcpy(&result[i], newW );
          i += strlen(newW);
          s += oldWlen;
      }
      else
          result[i++] = *s++;
  }

  result[i] = '\0';
  //memset( s, 0, strlen( s ));
  //strncpy( s, result, strlen( result ));
  return result;
}
