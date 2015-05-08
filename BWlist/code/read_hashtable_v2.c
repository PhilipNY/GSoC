#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hash_function.h"

int main(int argc, char *argv[]){
    FILE* hashtable_store = fopen("../io/hashtable_store_v2", "rb"); 

    unsigned hash_value = murmur(argv[1], strlen(argv[1]), 191);
    //unsigned hash_value = djb2(argv[1]);

    if (0 != fseek(hashtable_store, 4 * hash_value, SEEK_SET)) {
        
        printf("fseek error!");
        exit(1);

    } else {

        int* test = malloc(sizeof(int));
        fread(test, sizeof(int), 1, hashtable_store);
        
        if (*test == 0) {
    
            printf("Not in Blacklist!\n");
        
        } else {
            
            unsigned hash_value_djb2 = djb2(argv[1]);
            if(0 != fseek(hashtable_store, 4 * hash_value, SEEK_SET)) {
                printf("fseek error!");
                exit(1);
            }

            fread(test, sizeof(int), 1, hashtable_store);
            if(*test == 1) {

                printf("In blacklist!\n");
            
            } else {
            
                printf("Not in Blacklist!\n");
        
            }
        }
    }
    return 0;   
}
