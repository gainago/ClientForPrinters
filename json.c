#include <stdio.h>     // printf
#include <string.h>    // strlen, strcat, strncat, memmove
#include <stdlib.h>    // malloc
#include <ctype.h>     // isspace

char * str2JsonFormat(char *prevStr, char *key, char *valueStr)
{
    char *out;           // formatted JSON return string
    int outSize  = 0;    // size of formatted JSON return string
    int overhead = 8;    // extra characters for JSON formatting
    
    if(prevStr != NULL){
        outSize = strlen(prevStr) + strlen(key) + strlen(valueStr) + overhead;
    }
    else{
        outSize = strlen(key) + strlen(valueStr) + overhead;
    }
    // need to malloc memory location
    out = (char *) malloc(sizeof(char) * outSize);

   // remove all white space characters (space, tab, CR, LF)
   //removeWhiteSpace(prevStr);

    //  if previous string is empty or empty object create default string
    if(prevStr == NULL){
        strcat(out, "{");  
    }
    else
    if ((strlen(prevStr) == 0) |
          ( (prevStr[0] == '{') & (prevStr[1] == '}') ) )
    {
        strcat(out, "{");                            // add a "{" (left brace)
    }
    // otherwise append previous string to formatted return string with slight modifications
    else
    {
        strncat(out, prevStr, strlen(prevStr)-1 );   // delete the last '}' (right brace)
        strcat(out, ",\n");                            // and add ',' (comma)
    }

    // concatenate new key / value pair with JSON formatting
    strcat(out, "\"");       // add a '"' (double quote)
    strcat(out, key);
    strcat(out, "\":\"");    // complete the (double quote), add ':' (colon), start a new (double quote)
    strcat(out, valueStr);
    strcat(out, "\"}");      // add a '}' (right brace)

    // return pointer to the new JSON formatted string
    return out;
}


// int main()
// {
//     char *lastString;
//     char *jsonTemp;


//      //lastString = (char *) malloc(sizeof(char) * strlen("") );
//      lastString = NULL;
//     // memmove(lastString, "        { \"key1\"      : \"data1\"   }       \0", strlen("        { \"key1\"      : \"data1\"   }       \0") );
//     // strcat(lastString, "\0");
//     // strcat(lastString, "        { \"key1\"      : \"data1\"   }       \0");


//     jsonTemp   = str2JsonFormat(lastString, "key2", "data2");
//     free(lastString);
//     lastString = (char *) malloc(sizeof(char) * strlen(jsonTemp) );
//     memmove(lastString, jsonTemp, strlen(jsonTemp) );
//     free(jsonTemp);

//     jsonTemp   = str2JsonFormat(lastString, "key3", "data3");
//     free(lastString);
//     lastString = (char *) malloc(sizeof(char) * strlen(jsonTemp) );
//     memmove(lastString, jsonTemp, strlen(jsonTemp) );
//     free(jsonTemp);

//     jsonTemp   = str2JsonFormat(lastString, "key4", "data4");
//     free(lastString);
//     lastString = (char *) malloc(sizeof(char) * strlen(jsonTemp) );
//     memmove(lastString, jsonTemp, strlen(jsonTemp) );
//     free(jsonTemp);



//     printf(" output length is %li \r\n", strlen(lastString) );
//     printf(" output is %s \r\n", lastString);
//     free(lastString);
    


//     return 0;
// }