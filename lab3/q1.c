#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

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

// Function to check if a string is a numerical constant
int is_numerical_constant(char *str)
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
    return 1; // It's a valid numerical constant
}

// Function to check if a string is a string literal
int is_string_literal(char *str)
{
    return (str[0] == '"' && str[strlen(str) - 1] == '"');
}

// Function to check if the character is an operator (arithmetic, relational, or logical)
int is_operator(char *str)
{
    const char *operators[] = {"+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!", "%", "++", "--"};
    int num_operators = sizeof(operators) / sizeof(operators[0]);
    for (int i = 0; i < num_operators; i++)
    {
        if (strcmp(str, operators[i]) == 0)
        {
            return 1; // It's an operator
        }
    }
    return 0; // Not an operator
}

// Function to check if the character is a special symbol
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

// Function to get the next token from the file
void get_token(FILE *file, char *token)
{
    char c;
    int i = 0;

    // Skip white spaces
    while ((c = fgetc(file)) != EOF && isspace(c))
        ;

    // If end of file is reached
    if (c == EOF)
    {
        token[0] = '\0'; // Empty string denotes end of input
        return;
    }

    // If the character is a letter or underscore, it's the start of an identifier or keyword
    if (isalpha(c) || c == '_')
    {
        do
        {
            token[i++] = c;
            c = fgetc(file);
        } while (isalnum(c) || c == '_');
        ungetc(c, file); // Put the last non-identifier character back into the stream
        token[i] = '\0';
    }
    // If the character is a digit, it's the start of a numerical constant
    else if (isdigit(c) || c == '+' || c == '-')
    {
        do
        {
            token[i++] = c;
            c = fgetc(file);
        } while (isdigit(c) || c == '.');
        ungetc(c, file);
        token[i] = '\0';
    }
    // If the character is a double quote, it's a string literal
    else if (c == '"')
    {
        token[i++] = c;
        while ((c = fgetc(file)) != EOF && c != '"')
        {
            token[i++] = c;
        }
        if (c == '"')
        {
            token[i++] = c;
        }
        token[i] = '\0';
    }
    // If the character is a special symbol or operator
    else if (is_special_symbol(c))
    {
        token[0] = c;
        token[1] = '\0';
    }
    // If it's an operator, capture two-character operators like ==, !=, <=, etc.
    else if (strchr("=<>!+-*/%&|^", c))
    {
        token[0] = c;
        token[1] = '\0';
        char next_char = fgetc(file);
        if ((c == '=' && (next_char == '=')) ||
            (c == '!' && (next_char == '=')) ||
            (c == '<' && (next_char == '=')) ||
            (c == '>' && (next_char == '=')) ||
            (c == '&' && (next_char == '&')) ||
            (c == '|' && (next_char == '|')))
        {
            token[1] = next_char;
            token[2] = '\0';
        }
        else
        {
            ungetc(next_char, file);
        }
    }
}

// Function to analyze and categorize tokens
void analyze_token(char *token)
{
    if (is_keyword(token))
    {
        printf("Keyword: %s\n", token);
    }
    else if (is_identifier(token))
    {
        printf("Identifier: %s\n", token);
    }
    else if (is_numerical_constant(token))
    {
        printf("Numerical Constant: %s\n", token);
    }
    else if (is_string_literal(token))
    {
        printf("String Literal: %s\n", token);
    }
    else if (is_operator(token))
    {
        printf("Operator: %s\n", token);
    }
    else
    {
        printf("Special Symbol: %s\n", token);
    }
}

int main()
{
    FILE *file = fopen("digit.c", "r");
    if (!file)
    {
        perror("Failed to open file");
        return 1;
    }
    char token[MAX_TOKEN_LENGTH];

    while (1)
    {
        get_token(file, token);
        if (token[0] == '\0')
        {
            break; // End of file reached
        }
        analyze_token(token);
    }

    fclose(file);
    return 0;
}
