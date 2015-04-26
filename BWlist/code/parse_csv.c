#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
    
    FILE *origin_csv = fopen("../io/export.csv", "r");
    if (NULL == origin_csv) {
        printf("Open failed!");
        return 1;
    }

    char* line;
    ssize_t line_length;	
    size_t len = 0; 
    FILE *urllist = fopen("urllist", "w");   

    while ((getline(&line, &len, origin_csv)) != -1) {
        
        char* token;
        int counter = -1;
        int no_url = 0;
        
        token = strtok(line, ",\"");

        while (NULL != token) {
            counter++;

            if (1 == counter%3) {
                if (0 != strcmp(token, "-")) {
                    strcat(token, "\n");
                    fwrite(token, sizeof(char), strlen(token), urllist);
                } else {
                    no_url = 1;
                }
            } else if (2 == counter%3) {
                if (no_url) {
                    strcat(token, "\n");
                    fwrite(token, sizeof(char), strlen(token), urllist);
                }
                break;
            }

            token = strtok(NULL, ",\"");
        }
    }

    fclose(origin_csv);
    fclose(urllist);
    
    return 0;
}

