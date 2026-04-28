#include <stdio.h>
#include <string.h>

void rotate(const char *c, char* s){
    while(*c!='\0'){
        *s=*c;
        if(*c>='a' && *c<='z'){
            *s=((((*c)-'a')+13)%26)+'a';
        }
        if(*c>='A' && *c<='Z'){
            *s=((((*c)-'A')+13)%26)+'A';
        }
        c++; s++;
    }
}
     
int main(int argc, char* argv[]) {
    char s[41];
    memset(s, 0, 41);
    if(argc != 2) {
        printf("Usage: %s 'string to rot13'\n", argv[0]);
        return -1;
    }
    int sl = strlen(argv[1]);
    if(sl < 5 || sl > 40) return -2;
    rotate(argv[1],s);
    printf("%s",s);
    return 0;
}