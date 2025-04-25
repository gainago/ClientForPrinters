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
int GetLoginAndPasswordFromKey(Key key, LoginAndPassword *log);

//function for start program
int Start(); 

//return  1 is match that no login and password in cache
//if returt value is 0 than cache contains log and password
int GetLoginAndPasswordFromCache(LoginAndPassword *log);

//if return is 1 that in functions happend a mistake
//0 is correct return code
int GetTokensFromLogin(LoginAndPassword *log, Tokens *tokens);


size_t WriteTokensCallback(void* contents, size_t sizeMember, size_t numderMembers, void* userPointer) {
    
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

int WriteLoginAndPasswordToChache(LoginAndPassword *log);

int Start(){

    //default value is OK
    int res = 0;

    LoginAndPassword *log = (LoginAndPassword*)malloc(sizeof(LoginAndPassword));
    if(ErrorAllocationMemory(log, "Allocation for log")){ return 1;} //1 is error code

    res = GetLoginAndPasswordFromCache(log);
    if(res == 1){ // if function returned error code

        Key key;
        key.key = (char*)malloc(sizeof(char)*1024); //key will not be more than 1000  exactly
        key.sizeKey = 0;
        printf(" Please Enter Key(Get it from Titar)\n"); // we need write it to somewhere, //white this logic later!
        scanf("%s", key.key);
        if(res == -1) {
            fprintf(stderr, "no char\n");
            return 1;
        }
        
        key.sizeKey = strlen(key.key);

        #ifdef DEBUG
        printf("key is:%s; length is %ld\n", key.key, key.sizeKey);
        #endif
            
        GetLoginAndPasswordFromKey(key, log);
        WriteLoginAndPasswordToChache(log);
        }

        Tokens *tokens = (Tokens*)malloc(sizeof(Tokens));
        GetTokensFromLogin(log, tokens);
    
    return 0;
    
}

int GetLoginAndPasswordFromCache(LoginAndPassword *log){
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

    char *jsonData = (char*)malloc(sizeOfFile + 1);
    if(ErrorAllocationMemory(jsonData, "in function GetLoginAndPasswordFromChache")){ return 1;}

    fread(jsonData, 1, sizeOfFile, fileRead);
    fclose(fileRead);
    jsonData[sizeOfFile] = '\0';

    struct json_object *parsedJson = json_tokener_parse(jsonData);//parse file to json format
    if(parsedJson == NULL){
        fprintf(stderr, "uncorrect data in file with LoginAndPassword\n");
        return 1;
    }

    struct json_object *loginJson, *passwordJson;
    int res = 0;
    res = json_object_object_get_ex(parsedJson, "login", &loginJson);//get value by correspond key

    if(res == 0){
        fprintf(stderr, "uncorrect login in file with LoginAndPassword\n");
        return 1;
    }

    res = json_object_object_get_ex(parsedJson, "password", &passwordJson);//get value by correspond key

    if(res == 0){
        fprintf(stderr, "uncorrect password in file with LoginAndPassword\n");
        return 1;
    }
    
    #ifdef DEBUG
        printf("Login and password is:%s and %s\n", json_object_get_string(loginJson), json_object_get_string(passwordJson));
        printf("Login strlen is %lu, json_object_get_string_len is %d\n", strlen(json_object_get_string(loginJson) )
                                                                    , json_object_get_string_len(loginJson));
    #endif
    
    
    log->login = (char*)calloc(sizeof(char), json_object_get_string_len(loginJson) + 1);
    if(ErrorAllocationMemory(log->login, "Login in GetLoginAndPasswordFromChache")) {return 1;}
    strcpy(log->login, json_object_get_string(loginJson));
    

    log->password = (char*)calloc(sizeof(char), json_object_get_string_len(passwordJson) + 1);
    if(ErrorAllocationMemory(log->password, "Password in GetLoginAndPasswordFromChache")) {return 1;}
    strcpy(log->password, json_object_get_string(passwordJson));

    
    #ifdef DEBUG
        printf("Login and password is:%s and %s\n", log->login, log->password);
    #endif

    json_object_put(parsedJson);
    free(jsonData);
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



int GetLoginAndPasswordFromKey(Key key, LoginAndPassword *log){

    
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
    log->login = (char*)malloc(sizeof(char) * 6);
    log->password = (char*)malloc(sizeof(char) * 9);
    log->sizeLogin = 6;
    log->sizePassword = 9;
    strcpy(log->login, "admin");
    strcpy(log->password, "admin123");
    return 0;
}

int WriteLoginAndPasswordToChache(LoginAndPassword *log){

    FILE *file;
    file = fopen("./../.LoginAndPassword.txt", "w");
    if(file == NULL){
        fprintf(stderr, "Couls npt open file .LoginAndPassword.txt");
    }
    
    // Create new JSON object
    struct json_object *json = json_object_new_object();
    
    // Add fields login and password
    json_object_object_add(json, "login", json_object_new_string("admin"));
    json_object_object_add(json, "password", json_object_new_string("admin123"));
        
    // Преобразуем JSON объект в строку и выводим
    const char *jsonString = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY);
    fprintf(file ,"%s\n", jsonString);
        
    // Освобождаем память
    json_object_put(json);
    return 0;
    
}

int GetTokensFromLogin(LoginAndPassword *log, Tokens *tokens){

    CURL *curlHandle;

    #ifdef DEBUG
    curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L);
    #endif

    CURLcode res;
    struct curl_slist *list = NULL;
    list = curl_slist_append(list, "Content-Type: application/json"); //add header

    curlHandle = curl_easy_init(); //create handle
    if(ErrorAllocationMemory(curlHandle, "curl init in GetTokensFromLogin")) {return 1;}

    curl_easy_setopt(curlHandle, CURLOPT_URL, "https://tbkdev.latit.ru/printer-service/api/auth/login");
    curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, list);//set heager

    // Create new JSON object
    struct json_object *json = json_object_new_object();
    
    // Add fields login and password
    json_object_object_add(json, "login", json_object_new_string(log->login));
    json_object_object_add(json, "password", json_object_new_string(log->password));
        
    // Преобразуем JSON объект в строку и выводим
    const char *jsonString = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY);
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, jsonString); //set body

    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteTokensCallback);//set function fo write response in memory
    char *responce = (char*)malloc(sizeof(char));
    if(ErrorAllocationMemory(responce, "Get Tokens From Login")) {return 1;}
    responce[0] = '\0';

    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, responce);//send parametr to callback function

    res = curl_easy_perform(curlHandle);//by default it will write all info to stdout //now server do not support that but will be

    if(res != CURLE_OK){
        fprintf(stderr, "could not made curl_easy_perform in GetTokensFromLogin\n");
        free(responce);
        return 1;
    }
        
    // Освобождаем память
    json_object_put(json);
    return 0;
}