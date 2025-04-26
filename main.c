#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <json-c/json.h>

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
int GetLoginAndPasswordFromKey(Key key, char **responceJson);

int SetLoginAndPassword(char *dataJson, LoginAndPassword *log);

int SetTokens(char *dataJson, Tokens *Tokens);

//function for start program
int Start(); 

//return  1 is match that no login and password in cache
//if returt value is 0 than cache contains log and password
int GetLoginAndPasswordFromCache(char **jsonData);

//if return is 1 that in functions happend a mistake
//0 is correct return code
int GetTokensFromLogin(LoginAndPassword *log, char **jsonBuffer);


static size_t WriteTokensCallback(void* contents, size_t sizeMember, size_t numderMembers, void* userPointer) {
    
    size_t totalSize = sizeMember * numderMembers;
    char *buffer = (char*)userPointer;
    strncat(buffer, (char*)contents, totalSize);
    
    return totalSize;
}

int main(int argc, char **argv){
    curl_global_init(CURL_GLOBAL_ALL);
    Start();
    return 0;
}

int WriteLoginAndPasswordToCache(LoginAndPassword *log);

int Start(){

    //default value is OK
    int res = 0;

    LoginAndPassword *log = (LoginAndPassword*)malloc(sizeof(LoginAndPassword));
    if(ErrorAllocationMemory(log, "Allocation for log")){ return 1;} //1 is error code

    char *jsonBuffer = NULL;
    res = GetLoginAndPasswordFromCache(&jsonBuffer);
    if(res == 1){ // if function returned error code

        Key key;
        key.key = (char*)malloc(sizeof(char)*1024); //key will not be more than 1000  exactly
        if(ErrorAllocationMemory(key.key, "Allocation for key")){ return 1;}
        strncpy(key.key, "someKey", strlen("someKey") + 1);
        
        // key.sizeKey = 0;
        // printf(" Please Enter Key(Get it from Titar)\n"); // we need write it to somewhere, //white this logic later!
        // scanf("%s", key.key);
        // if(res == -1) {
        //     fprintf(stderr, "no char\n");
        //     return 1;
        // }
        
        // key.sizeKey = strlen(key.key);

        // #ifdef DEBUG
        // printf("key is:%s; length is %ld\n", key.key, key.sizeKey);
        // #endif

        GetLoginAndPasswordFromKey(key, &jsonBuffer);
        free(key.key);
        SetLoginAndPassword(jsonBuffer, log);
        WriteLoginAndPasswordToCache(log);
        }
        SetLoginAndPassword(jsonBuffer, log);

        free(jsonBuffer);
        jsonBuffer = NULL;
        

        Tokens *tokens = (Tokens*)malloc(sizeof(Tokens));
        if(ErrorAllocationMemory(tokens, "Allocation for token")){ return 1;}
        GetTokensFromLogin(log, &jsonBuffer);
        #ifdef DEBUG
        printf("\njsonBuffer is:%s\n", jsonBuffer);
        #endif
        SetTokens(jsonBuffer, tokens);
        printf("\nother string\n");

        free(log->login);
        free(log->password);
        free(log);
    
    return 0;
    
}

int GetLoginAndPasswordFromCache(char **jsonData){

    FILE *fileRead;
    fileRead = fopen("./../.LoginAndPassword.txt", "r");
    if(fileRead == NULL){
        fprintf(stderr, "Couls not open file ./../.LoginAndPassword.txt");
        return 1;
    }

    fseek(fileRead, 0, SEEK_END);  // устанавливаем указатель на конец файл
    long sizeOfFile = ftell(fileRead);
    if(sizeOfFile < 0){
        fprintf(stderr, "Couls not open file ./../.LoginAndPassword.txt");
        return 1;
    }
    fseek(fileRead, 0, SEEK_SET);

    *jsonData = (char*)malloc(sizeOfFile + 1);
    if(ErrorAllocationMemory(*jsonData, "in function GetLoginAndPasswordFromChache")){ return 1;}

    fread(*jsonData, 1, sizeOfFile, fileRead);
    fclose(fileRead);
    (*jsonData)[sizeOfFile] = '\0';

    #ifdef DEBUG
    printf("Login and Password in GetLoginAndPasswordFromCache is:%s\n", *jsonData);
    #endif

   
    return 0;                                                                                    
}

//return 1 if ptr is NULL
int ErrorAllocationMemory(void *ptr, char* str){

    if(ptr == NULL){

        fprintf(stderr, "Wrong allocation memory in %s\n", str);
        return 1;
    }
    return 0;
}



int GetLoginAndPasswordFromKey(Key const key, char **jsonBufferLoginAndPassword){

    
    // CURL *curl_handle;

    // #ifdef DEBUG
    // curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    // #endif

    // CURLcode res;
    // struct curl_slist *list = NULL;
    // list = curl_slist_append(list, "Content-Type: application/json");

    // //char* jsonObj = "{ \"activationKey\" : \"ACTIVATION-KEY-2571461c-fb57-4915-8f59-a431fcb903a9\"\n}";
    // char* jsonObj = str2JsonFormat(NULL, "activationKey", key.key);
    
    // /* init the curl session */
    // curl_handle = curl_easy_init();
    
    
    // /* specify URL to get */
    // // url do not moved to other, global places, because there is one function when this is used
    // res = curl_easy_setopt(curl_handle, CURLOPT_URL, "https://tbkdev.latit.ru/printer-service/api/printer-app/register");
    // curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "PUT");
    // curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, list);
    // curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, jsonObj);

    // //'Content-Type: application/json'
    // /* get it! */
    // res = curl_easy_perform(curl_handle);//by default it will write all info to stdout //now server do not support that but will be
    
    // responce = malloc()
    // log->login = (char*)malloc(sizeof(char) * 6);
    // log->password = (char*)malloc(sizeof(char) * 9);
    // log->sizeLogin = 6;
    // log->sizePassword = 9;
    // strcpy(log->login, "admin");
    // strcpy(log->password, "admin123");
    // return 0;
     // Create new JSON object
    struct json_object *json = json_object_new_object();
    if(ErrorAllocationMemory((json), "could not allocate json in GetLoginAndPasswordFromKey\n")) {return 1;}
    
    // Add fields login and password
    json_object_object_add(json, "login", json_object_new_string("admin"));
    json_object_object_add(json, "password", json_object_new_string("admin123"));
        

    const char *jsonString = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY);

    #ifdef DEBUG
    printf("jsonData in GetLoginAndPasswordFromKey is:%s\n strlen(jsonString) is:%lu\n", jsonString, strlen(jsonString));
    #endif

    (*jsonBufferLoginAndPassword) = (char*)malloc(sizeof(char)*strlen(jsonString) + 1);
    if(ErrorAllocationMemory((*jsonBufferLoginAndPassword), "could not allocate memory in GetLoginAndPasswordFromKey\n")) {return 1;}
    //(*jsonBufferLoginAndPassword)[0] = '\0';
    strncpy(*jsonBufferLoginAndPassword, jsonString, strlen(jsonString) + 1);

    json_object_put(json);

    #ifdef DEBUG
    printf("jsonBuffer in GetLoginAndPasswordFromKey is:%s\n", *jsonBufferLoginAndPassword);
    #endif

    return 0;
}

int WriteLoginAndPasswordToCache(LoginAndPassword *log){

    FILE *file;
    file = fopen("./../.LoginAndPassword.txt", "w");
    if(file == NULL){
        fprintf(stderr, "Couls npt open file .LoginAndPassword.txt");
    }

    #ifdef DEBUG
    printf("log->login and log->password that passed in WriteLoginAndPasswordToCache is: %s and %s\n", log->login, log->password);
    #endif
    
    // Create new JSON object
    struct json_object *json = json_object_new_object();
    
    // Add fields login and password
    json_object_object_add(json, "login", json_object_new_string(log->login));
    json_object_object_add(json, "password", json_object_new_string(log->password));
        
    // convert JSON object to string
    const char *jsonString = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY);

    #ifdef DEBUG
    printf("string that will write to chache in WriteLoginAndPasswordToCache is: %s\n", jsonString);
    #endif

    fprintf(file ,"%s\n", jsonString);
        
    // Освобождаем память
    json_object_put(json);
    return 0;
    
}

int GetTokensFromLogin(LoginAndPassword *log, char **jsonBufferTokens){

    CURL *curlHandle = curl_easy_init();//create handle

    #ifdef DEBUG
    curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L);//more info from libcurl
    #endif

    CURLcode res;
    struct curl_slist *list = NULL;
    list = curl_slist_append(list, "Content-Type: application/json"); //add header

    if(ErrorAllocationMemory(curlHandle, "curl init in GetTokensFromLogin")) {return 1;}
    //https://tbkdev.latit.ru/printer-service/api/auth/login
    curl_easy_setopt(curlHandle, CURLOPT_URL, "http://tbkdev.latit.ru/printer-service/api/auth/login");
    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, list);//set heager

    // Create new JSON object
    struct json_object *json = json_object_new_object();
    
    // Add fields login and password
    json_object_object_add(json, "login", json_object_new_string(log->login));
    json_object_object_add(json, "password", json_object_new_string(log->password));
        
    // Преобразуем JSON объект в строку и выводим
    const char *jsonString = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY);

    #ifdef DEBUG
    printf("body of http request in GetTokensFromLogin is: %s\n", jsonString);
    #endif

    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, jsonString); //set body

    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteTokensCallback);//set function fo write response in memory
    *jsonBufferTokens = (char*)malloc(sizeof(char) * 2);
    if(ErrorAllocationMemory(*jsonBufferTokens, "Get Tokens From Login")) {return 1;}
    (*jsonBufferTokens)[0] = '\0';

    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, *jsonBufferTokens );//send parametr to callback function

    res = curl_easy_perform(curlHandle);//by default it will write all info to stdout //now server do not support that but will be

    //printf("----------\n%s\n", *jsonBufferTokens);
    printf("\n");
    if(res != CURLE_OK){
        fprintf(stderr, "could not made curl_easy_perform in GetTokensFromLogin\n");
        free(*jsonBufferTokens);
        return 1;
    }

    #ifdef DEBUG
        printf("jsonBuffer in GetTokensFromLogin is %s\n", *jsonBufferTokens);
    #endif
    //now we got buffer with answer

        
    // Освобождаем память
    json_object_put(json);
    printf("before curl_easy_cleanup\n");
    curl_easy_cleanup(curlHandle);
    printf("after curl_easy_cleanup\n");
    return 0;
}

int SetLoginAndPassword(char *jsonData, LoginAndPassword *log){

    if(jsonData == NULL){
        fprintf(stderr, "burrer pointing to NULL in SetLoginAndPassword\n");
        return 1;
    }

    struct json_object *parsedJson = json_tokener_parse(jsonData);//parse dataJson to json format
    if(parsedJson == NULL){
        fprintf(stderr, "uncorrect format in jsonData in SetLoginAndPassword\n");
        return 1;
    }

    struct json_object *loginJson, *passwordJson;
    int res = 0;
    res = json_object_object_get_ex(parsedJson, "login", &loginJson);//get value by correspond key

    if(res == 0){
        fprintf(stderr, "uncorrect format in login in SetLoginAndPassword\n");
        return 1;
    }

    res = json_object_object_get_ex(parsedJson, "password", &passwordJson);//get value by correspond key

    if(res == 0){
        fprintf(stderr, "uncorrect format in password in SetLoginAndPassword\n");
        return 1;
    }
    
    #ifdef DEBUG
        printf("Login and password in SetLoginAndPassword is:%s and %s\n", json_object_get_string(loginJson), json_object_get_string(passwordJson));
        printf("Login strlen in SetLoginAndPassword is %lu, json_object_get_string_len is %d\n", strlen(json_object_get_string(loginJson) )
                                                                    , json_object_get_string_len(loginJson));
    #endif
    
    
    log->login = (char*)calloc(sizeof(char), json_object_get_string_len(loginJson) + 1);
    if(ErrorAllocationMemory(log->login, "Login in GetLoginAndPasswordFromChache")) {return 1;}
    strncpy(log->login, json_object_get_string(loginJson), json_object_get_string_len(loginJson) + 1);
    

    log->password = (char*)calloc(sizeof(char), json_object_get_string_len(passwordJson) + 1);
    if(ErrorAllocationMemory(log->password, "Password in GetLoginAndPasswordFromChache")) {return 1;}
    strncpy(log->password, json_object_get_string(passwordJson), json_object_get_string_len(passwordJson) + 1);

    
    #ifdef DEBUG
        printf("Login and password in SetLoginAndPassword in log is:%s and %s\n", log->login, log->password);
    #endif

    json_object_put(parsedJson);
    return 0;
}

int SetTokens(char *dataJson, Tokens *tokens){

    if(dataJson == NULL){
        fprintf(stderr, "burrer pointing to NULL in SetTokens\n");
        return 1;
    }
    printf("------\n%s\n", dataJson);

    struct json_object *parsedJson = json_tokener_parse(dataJson);//parse dataJson to json format
    if(parsedJson == NULL){
        fprintf(stderr, "uncorrect format in jsonData in SetTokens\n");
        return 1;
    }

    struct json_object *accessToken, *refreshToken;
    int res = 0;
    res = json_object_object_get_ex(parsedJson, "accessToken", &accessToken);//get value by correspond key

    if(res == 0){
        fprintf(stderr, "uncorrect format in accessToken in SetTokens\n");
        return 1;
    }

    res = json_object_object_get_ex(parsedJson, "refreshToken", &refreshToken);//get value by correspond key

    if(res == 0){
        fprintf(stderr, "uncorrect format in refreshToken in SetTokens\n");
        return 1;
    }
    
    #ifdef DEBUG
        printf("accessToken and refreshToken in SetTokens is:%s and %s\n", json_object_get_string(accessToken), json_object_get_string(refreshToken));
        printf("accessToken strlen in SetTokens is %lu, json_object_get_string_len is %d\n", strlen(json_object_get_string(accessToken) )
                                                                    , json_object_get_string_len(accessToken));
    #endif
    
    
    tokens->accessToken = (char*)calloc(sizeof(char), json_object_get_string_len(accessToken) + 1);
    if(ErrorAllocationMemory(tokens->accessToken, "accessToken in SetTokens")) {return 1;}
    strcpy(tokens->accessToken, json_object_get_string(accessToken));
    

    tokens->refreshToken = (char*)calloc(sizeof(char), json_object_get_string_len(refreshToken) + 1);
    if(ErrorAllocationMemory(tokens->refreshToken, "refreshToken in SetTokens")) {return 1;}
    strcpy(tokens->refreshToken, json_object_get_string(refreshToken));

    
    #ifdef DEBUG
        printf("accessToken and refreshToken in SetTokens in tokens is:%s and %s\n", tokens->accessToken, tokens->refreshToken);
    #endif

    json_object_put(parsedJson);
    return 0;

}