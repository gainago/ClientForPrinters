#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "json.h"

#define DEBUG 1

typedef struct {
    char *key;
    long int sizeKey;
} Key;

typedef struct {
    char *login;
    long int sizeLogin;
    char *password;
    long int sizePassword;
} LoginAndPassword;

typedef struct {
    char *accessToken;
    char *refreshToken;
} Tokens;

int ErrorAllocationMemory(void *prt, char* str);

//1 is error 0 is ok
int GetLoginAndPasswordFromKey(Key key, LoginAndPassword *log);

    //function for start program
int Start(); 

//return  1 is match that no login and password in chace
//if returt value is 0 log contain real log and password
int GetLoginAndPasswordFromCache(LoginAndPassword *log);

int main(int argc, char **argv){
    curl_global_init(CURL_GLOBAL_ALL);
    Start();
}

int Start(){

    //default value is OK
    int res = 0;

    LoginAndPassword *log = (LoginAndPassword*)malloc(sizeof(LoginAndPassword));
    if(ErrorAllocationMemory(log, "Allocation for log")){ return 1;} //1 is error code

        res = GetLoginAndPasswordFromCache(log);
        if(res == 1){ // if function returned error code

            Key key;
            key.key = NULL;
            key.sizeKey = 0;
            printf(" Please Enter Key(Get it from Titar)\n"); // we need write it to somewhere, //white this logic later!
            res = getline(&key.key, &key.sizeKey, stdin);
            if(key.key[res - 1] == '\n'){
                key.key[res - 1] = '\0';//get rid of '\n'
            }
            key.sizeKey = res - 1;

            #ifdef DEBUG
            printf("key is:%s; length is %ld\n res is %d", key.key, key.sizeKey, res);
            #endif

            if(res == -1) {fprintf(stderr, "no char\n");}
            
            GetLoginAndPasswordFromKey(key, log);
        }
    
}

int GetLoginAndPasswordFromCache(LoginAndPassword *log){
    //now he have not login and password
    return 1;                                                                                  //white this logic later!
}

//return 0 is correct behavior
int ErrorAllocationMemory(void *ptr, char* str){

    if(ptr == NULL){

        fprintf(stderr, "Wrong allocation memory in %s\n", str);
        return 1;
    }
    return 0;
}



int GetLoginAndPasswordFromKey(Key key, LoginAndPassword *log){

    
    CURL *curl_handle;

    #ifdef DEBUG
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    #endif

    CURLcode res;
    struct curl_slist *list = NULL;
    list = curl_slist_append(list, "Content-Type: application/json");

    //char* jsonObj = "{ \"activationKey\" : \"ACTIVATION-KEY-2571461c-fb57-4915-8f59-a431fcb903a9\"\n}";
    char* jsonObj = str2JsonFormat(NULL, "activationKey", key.key);
    
    /* init the curl session */
    curl_handle = curl_easy_init();
    
    
    /* specify URL to get */
    // url do not moved to other, global places, because there is one function when this is used
    res = curl_easy_setopt(curl_handle, CURLOPT_URL, "https://tbkdev.latit.ru/printer-service/api/printer-app/register");
    curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, jsonObj);

    //'Content-Type: application/json'
    /* get it! */
    res = curl_easy_perform(curl_handle);//by default it will write all info to stdout
}
