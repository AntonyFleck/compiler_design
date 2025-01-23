#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
const char *keywords[] = {
    "auto", "break", "case","define","char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", NULL};

int is_keyword(const char *word)
{
    for (int i = 0; keywords[i] != NULL; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1; // It's a keyword
        }
    }
    return 0; // It's not a keyword
}

void to_uppercase(char *word)
{
    for (int i = 0; word[i]; i++)
    {
        word[i] = word[i]+32;
    }
}

int main()
{
    FILE *fa, *fb;
    int ca, cb;
    fa = fopen("q4in.c", "r");
    if (fa == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    fb = fopen("q4out.c", "w");
    ca = getc(fa);
    
    while (ca != EOF)
    {
        if ((ca >= 'a' && ca <= 'z') || (ca >= 'A' && ca <= 'z')){
            char buf[100];
            for (int i = 0;(ca >= 'a' && ca <= 'z') || (ca >= 'A' && ca <= 'z'); i++)
            {
                buf[i]=ca;
                ca=getc(fa);
            }
            if (is_keyword(buf))
            {
                to_uppercase(buf);
            }

            for (int i = 0; i < strlen(buf); i++)
            {
                putc(buf[i], fb);
                printf("%c",buf[i]);
            }
            printf("\n");
            putc(ca,fb);
        }
        else{
            putc(ca,fb);
        }
        ca=getc(fa);
    }
    fclose(fa);
    fclose(fb);
    return 0;
}
