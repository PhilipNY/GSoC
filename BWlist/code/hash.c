#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hash_function.h"

struct node {
    char* url;
    struct node* next;
};

void insert(int* hashT, struct node* hashtable, unsigned int hash_value, char* url) {
    
    if (NULL == hashtable[hash_value].url) {

        hashtable[hash_value].url = strdup(url);

        
        if(0 == hashT[hash_value]) {   
            hashT[hash_value] = 1;
        } else {
            printf("F\n");
            //printf("%u ", hash_value);
            printf("%s\n", url);
        }

    } else if (0 != strcmp(hashtable[hash_value].url, url)) {
        
        /* 
        struct node* next = &hashtable[hash_value];    
        struct node* last = next;

        while (NULL != next) {
            if (0 == strcmp(next->url, url)) {
                return;
            }
            last = next;
            next = next -> next;
        }
        
        last -> next = (struct node*)malloc(sizeof(struct node));
        last -> next -> url = strdup(url);
        last -> next -> next = NULL;*/
        
        unsigned int djb2_value = djb2(url);
        if( 0 == hashT[djb2_value] ) {
            hashT[djb2_value] = 1;
        } else {
            printf("C\n");
            printf("%s\n", url);
        }

        if( NULL == hashtable[djb2_value].url) {
            hashtable[djb2_value].url = strdup(url);
        } else {
            struct node* next = &hashtable[djb2_value];    
            struct node* last = next;

            while (NULL != next) {
                if (0 == strcmp(next->url, url)) {
                    return;
                }
                last = next;
                next = next -> next;
            }
        
            last -> next = (struct node*)malloc(sizeof(struct node));
            last -> next -> url = strdup(url);
            last -> next -> next = NULL;
        }
        
    }
}

/*
void insert(int* hashtable, unsigned int hash_value, char* url){
    if (0 == hashtable[hash_value]) {
        hashtable[hash_value] = 1;
    } else {
        //hashtable[hash_value] = 1;
        printf("PO\n");
        unsigned int djb2_value = djb2(url);
        if( 0 == hashtable[djb2_value]) {
            hashtable[djb2_value] = 1;
        } else {
            printf("C!");
        }
    }
}
*/

int main() {
    
    struct node hashtable[98317];
    int hashtable2[98317];
    
    int i = 0;
     
    for (; i < 98317; i++) {
        hashtable[i].url = NULL;
        hashtable[i].next = NULL;
    }
    
    for(i=0 ; i < 98317; i++) {
        hashtable2[i] = 0;
    }

    char* line;
    size_t len = 0;
    FILE* urllist = fopen("../io/urllist", "r");
    
    while ((getline(&line, &len, urllist)) != -1) {
        line = strtok(line, "\n");
        //unsigned int hash_value = djb2(line);
        unsigned int hash_value = murmur(line, strlen(line), 191);
        insert(hashtable2, hashtable, hash_value, line);
    }

    FILE* hashtable_store = fopen("../io/hashtable_store_v2", "wb");
    for (i = 0; i < 98317; i++) {
        fwrite( &hashtable2[i], sizeof(int), 1, hashtable_store);
    }

    /*
    FILE* hashtable_store = fopen("../io/hashtable_store", "wb");
    int counter = 0;
    int not_in_BW = -1;

    for (i = 0; i < 98317; i++) {
        struct node* next = &hashtable[i];
        if (NULL != next -> url) {
            fwrite( &counter, sizeof(int), 1, hashtable_store);
            while (NULL != next) {
                counter += strlen(next -> url);
                next = next -> next;
                counter++;
            }
            counter++;
        } else {
            fwrite( &not_in_BW, sizeof(int), 1, hashtable_store);
        }
    }
    

    for (i = 0; i < 98317; i++) {
        struct node* next = &hashtable[i];
        if (NULL != next -> url) {
            while (NULL != next) {
                fwrite (next -> url, sizeof(char), strlen(next -> url), hashtable_store);
                char* same_hash_value = " ";
                fwrite (same_hash_value, sizeof(char), 1, hashtable_store);
                next = next -> next; 
            }
            char* invalid_in_url = "^";
            fwrite (invalid_in_url, sizeof(char), 1, hashtable_store);
        }
    }
    */
}
