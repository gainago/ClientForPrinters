#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int res = 0;
    char *key = NULL;
    long int size = 0;
    printf(" Please Enter Key(Get it from Titar)\n"); // we need write it to somewhere, //white this logic later!
    res = getline(&key, &size, stdin);
    key[res - 1] = '\0';
    printf("key is:%s; length is %ld\n res is %d, strlen is: %ld", key, size, res, strlen(key));
    free(key);
}