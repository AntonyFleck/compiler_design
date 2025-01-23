#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_LINE_LENGTH 256

// Define token types
typedef enum
{
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    NUMERIC,
    STRING_LITERAL,
    SPECIAL_SYMBOL,
    UNKNOWN
} TokenType;

// Token structure to store token information
typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
    int row;
    int col;
} Token;

// List of C keywords for comparison
const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if", "int",
    "long", "register", "return", "short", "signed", "sizeof", "static", "struct",
    "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

// Function to check if a string is a keyword
int is_keyword(char *str)
{
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return 1; // It's a keyword
        }
    }
    return 0; // Not a keyword
}

// Function to check if a string is an identifier
int is_identifier(char *str)
{
    if (!isalpha(str[0]) && str[0] != '_')
    {
        return 0; // Identifiers must start with a letter or underscore
    }
    for (int i = 1; str[i] != '\0'; i++)
    {
        if (!isalnum(str[i]) && str[i] != '_')
        {
            return 0; // Identifiers can only contain letters, digits, and underscores
        }
    }
    return 1; // It's a valid identifier
}

// Function to check if a string is a numeric constant
int is_numeric_constant(char *str)
{
    int i = 0;
    if (str[i] == '+' || str[i] == '-')
    {
        i++; // Skip the optional sign
    }
    int dot_count = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '.')
        {
            dot_count++;
            if (dot_count > 1)
            {
                return 0; // More than one dot is not allowed in a number
            }
        }
        else if (!isdigit(str[i]))
        {
            return 0; // Must be a digit or a single dot
        }
        i++;
    }
    return 1; // It's a valid numeric constant
}

// Function to check if a string is a string literal
int is_string_literal(char *str)
{
    return (str[0] == '"' && str[strlen(str) - 1] == '"');
}

// Function to check if the character is a special symbol (e.g., `(){}[];,.`)
int is_special_symbol(char c)
{
    const char special_symbols[] = "(){}[],;.";
    for (int i = 0; i < strlen(special_symbols); i++)
    {
        if (c == special_symbols[i])
        {
            return 1; // It's a special symbol
        }
    }
    return 0; // Not a special symbol
}

// Function to check if the character is an operator
int is_operator(char c)
{
    const char operators[] = "+-*/%=<>!&|^";
    for (int i = 0; i < strlen(operators); i++)
    {
        if (c == operators[i])
        {
            return 1; // It's an operator
        }
    }
    return 0; // Not an operator
}

// Function to get the next token from the file
Token getNextToken(FILE *file, int *row, int *col)
{
    char c;
    char token_value[MAX_TOKEN_LENGTH];
    int i = 0;

    Token token = {UNKNOWN, "", *row, *col};

    // Skip whitespace characters
    while ((c = fgetc(file)) != EOF)
    {
        (*col)++;
        if (isspace(c))
        {
            if (c == '\n')
            {
                (*row)++;
                *col = 0; // Reset column number for the new line
            }
            continue;
        }

        // Skip preprocessor directives
        if (c == '#')
        {
            while (c != '\n' && c != EOF)
            {
                c = fgetc(file);
                (*col)++;
            }
            if (c == '\n')
            {
                (*row)++;
                *col = 0; // Reset column number for the new line
            }
            continue;
        }

        // Single-line comment (//)
        if (c == '/')
        {
            c = fgetc(file);
            (*col)++;
            if (c == '/')
            {
                while (c != '\n' && c != EOF)
                {
                    c = fgetc(file);
                    (*col)++;
                }
                if (c == '\n')
                {
                    (*row)++;
                    *col = 0; // Reset column number for the new line
                }
                continue;
            }
            else
            {
                ungetc(c, file);
                (*col)--; // Rollback the column number for the non-comment
            }
        }

        // Multi-line comment (/*...*/)
        if (c == '/')
        {
            c = fgetc(file);
            (*col)++;
            if (c == '*')
            {
                while (1)
                {
                    c = fgetc(file);
                    (*col)++;
                    if (c == '*' && (c = fgetc(file)) == '/')
                    {
                        (*col)++;
                        break;
                    }
                    if (c == EOF)
                    {
                        return token; // Return EOF token if no closing */
                    }
                }
                continue;
            }
            else
            {
                ungetc(c, file);
                (*col)--; // Rollback the column number for the non-comment
            }
        }

        // String literal
        if (c == '"')
        {
            token_value[i++] = c;
            while ((c = fgetc(file)) != '"' && c != EOF)
            {
                token_value[i++] = c;
                (*col)++;
            }
            if (c == '"')
            {
                token_value[i++] = c;
                token_value[i] = '\0';
                token.type = STRING_LITERAL;
                strcpy(token.value, token_value);
                return token;
            }
        }

        // Identifier or keyword
        if (isalpha(c) || c == '_')
        {
            token_value[i++] = c;
            while (isalnum(c = fgetc(file)) || c == '_')
            {
                token_value[i++] = c;
                (*col)++;
            }
            ungetc(c, file); // Rollback non-alphanumeric character
            token_value[i] = '\0';

            if (is_keyword(token_value))
            {
                token.type = KEYWORD;
            }
            else
            {
                token.type = IDENTIFIER;
            }
            strcpy(token.value, token_value);
            return token;
        }

        // Numeric constant
        if (isdigit(c))
        {
            token_value[i++] = c;
            while (isdigit(c = fgetc(file)))
            {
                token_value[i++] = c;
                (*col)++;
            }
            ungetc(c, file);
            token_value[i] = '\0';
            token.type = NUMERIC;
            strcpy(token.value, token_value);
            return token;
        }

        // Special symbol or operator
        if (is_special_symbol(c))
        {
            token_value[i++] = c;
            token_value[i] = '\0';
            token.type = SPECIAL_SYMBOL;
            strcpy(token.value, token_value);
            return token;
        }

        if (is_operator(c))
        {
            token_value[i++] = c;
            token_value[i] = '\0';
            token.type = OPERATOR;
            strcpy(token.value, token_value);
            return token;
        }

        // Unknown character
        token_value[i++] = c;
        token_value[i] = '\0';
        token.type = UNKNOWN;
        strcpy(token.value, token_value);
        return token;
    }

    // EOF
    token.type = UNKNOWN;
    strcpy(token.value, "EOF");
    return token;
}

int main()
{
    FILE *file = fopen("digit.c", "r");
    if (!file)
    {
        perror("Failed to open file");
        return 1;
    }

    int row = 1, col = 0;
    Token token;

    while (1)
    {
        token = getNextToken(file, &row, &col);
        if (token.type == UNKNOWN && strcmp(token.value, "EOF") == 0)
        {
            break; // End of file
        }
        printf("Token: %-16s Type: %-5d Row: %-3d Col: %-3d\n",
               token.value, token.type, token.row, token.col);
    }

    fclose(file);
    return 0;
}
