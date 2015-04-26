#include<stdio.h>
#include<stdlib.h>
#include<string.h>

unsigned int djb2(unsigned char* str) {
    unsigned long hash = 5381;
    int c;
    
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; 
    }
    
    return hash % 98317;
}

int main(int argc, char *argv[]){
    FILE* hashtable_store = fopen("../io/hashtable_store", "rb"); 

    unsigned hash_value = djb2(argv[1]);

    if (0 != fseek(hashtable_store, 4 * hash_value, SEEK_SET)) {
        
        printf("fseek error!");
        exit(1);

    } else {

        int* test = malloc(sizeof(int));
        fread(test, sizeof(int), 1, hashtable_store);
        
        if (*test == -1) {
    
            printf("Not in Blacklist!\n");
        
        } else {

            fseek(hashtable_store, 4*98317+(*test), SEEK_SET);
            int c = fgetc (hashtable_store);

            int i = 0;
            while (c != '^') {
                if (i < strlen(argv[1])) {
                    if (c == argv[1][i]) {
                        i++;
                    } else if (c == ' ') {
                        i = 0;
                    } 
                } else if (i == strlen(argv[1]) && c == ' ') {
                    printf("In Blacklist!\n");
                    return 0;
                }
                c = fgetc(hashtable_store);
            }
            
            printf("Not in Blacklist!\n");
        }
    }
    return 0;   
}
