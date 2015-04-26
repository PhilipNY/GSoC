#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node {
    char* url;
    struct node* next;
};

unsigned int djb2(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % 98317;
}

unsigned int murmur(const char *key, unsigned int len, unsigned int seed) {
    static const unsigned int c1 = 0xcc9e2d51;
    static const unsigned int c2 = 0x1b873593;
    static const unsigned int r1 = 15;
    static const unsigned int r2 = 13;
    static const unsigned int m = 5;
    static const unsigned int n = 0xe6546b64;
 
    unsigned int hash = seed;
 
    const int nblocks = len / 4;
    const unsigned int *blocks = (const unsigned int *) key;
    int i;
    for (i = 0; i < nblocks; i++) {
        unsigned int k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
 
        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }
 
    const unsigned char *tail = (const unsigned char *) (key + nblocks * 4);
    unsigned int k1 = 0;
 
    switch (len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
 
        k1 *= c1;
        k1 = (k1 << r1) | (k1 >> (32 - r1));
        k1 *= c2;
        hash ^= k1;
    }
 
    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);
 
    return hash % 98317;
}

void insert(struct node* hashtable, unsigned int hash_value, char* url) {
    
    if (NULL == hashtable[hash_value].url) {

        hashtable[hash_value].url = strdup(url);

    } else if (0 != strcmp(hashtable[hash_value].url, url)) {
        
        printf("%u %s\n", hash_value, url);    
    
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
        last -> next -> next = NULL;

    }
}

int main() {
    
    struct node hashtable[98317];
    
    int i = 0; 
    for (; i < 98317; i++) {
        hashtable[i].url = NULL;
        hashtable[i].next = NULL;
    }
    
    char* line;
    size_t len = 0;
    FILE* urllist = fopen("../io/urllist", "r");
    
    while ((getline(&line, &len, urllist)) != -1) {
        line = strtok(line, "\n");
        //unsigned int hash_value = djb2(line);
        unsigned int hash_value = murmur(line, strlen(line), 191);
        insert(hashtable, hash_value, line);
    }

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

}
