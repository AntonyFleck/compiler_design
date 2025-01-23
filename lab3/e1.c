
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Define token types
typedef enum
{
    NUMBER, // for integers and floating-point numbers
    PLUS,   // for "+"
    MINUS,  // for "-"
    TIMES,  // for "*"
    DIVIDE, // for "/"
    LPAREN, // for "("
    RPAREN, // for ")"
    ERROR,  // for invalid tokens
    END     // End of input
} TokenType;

// Token structure to store token information
typedef struct
{
    TokenType type;
    char value[100]; // The value of the token (e.g., the number or operator)
} Token;

// Function to create a token with a given type and value
Token create_token(TokenType type, const char *value)
{
    Token token;
    token.type = type;
    strcpy(token.value, value);
    return token;
}

// Function to get the next token from the input string
Token get_next_token(const char **input)
{
    while (**input != '\0' && isspace(**input))
    {
        (*input)++; // Skip whitespace
    }

    if (**input == '\0')
    {
        return create_token(END, ""); // End of input
    }

    // Handle numbers (integers or floating-point)
    if (isdigit(**input))
    {
        char num[100];
        int i = 0;
        while (isdigit(**input))
        {
            num[i++] = **input;
            (*input)++;
        }
        num[i] = '\0';

        // Handle decimal part for floating-point numbers
        if (**input == '.')
        {
            num[i++] = **input;
            (*input)++;
            while (isdigit(**input))
            {
                num[i++] = **input;
                (*input)++;
            }
            num[i] = '\0';
        }

        return create_token(NUMBER, num); // Return number token
    }

    // Handle operators and parentheses
    if (**input == '+')
    {
        (*input)++;
        return create_token(PLUS, "+");
    }
    if (**input == '-')
    {
        (*input)++;
        return create_token(MINUS, "-");
    }
    if (**input == '*')
    {
        (*input)++;
        return create_token(TIMES, "*");
    }
    if (**input == '/')
    {
        (*input)++;
        return create_token(DIVIDE, "/");
    }
    if (**input == '(')
    {
        (*input)++;
        return create_token(LPAREN, "(");
    }
    if (**input == ')')
    {
        (*input)++;
        return create_token(RPAREN, ")");
    }

    // If we get here, the character is an invalid token
    char invalid[2] = {*(*input), '\0'};
    (*input)++;
    return create_token(ERROR, invalid);
}

// Function to print the token information
void print_token(Token token)
{
    const char *type_str;
    switch (token.type)
    {
    case NUMBER:
        type_str = "NUMBER";
        break;
    case PLUS:
        type_str = "PLUS";
        break;
    case MINUS:
        type_str = "MINUS";
        break;
    case TIMES:
        type_str = "TIMES";
        break;
    case DIVIDE:
        type_str = "DIVIDE";
        break;
    case LPAREN:
        type_str = "LPAREN";
        break;
    case RPAREN:
        type_str = "RPAREN";
        break;
    case ERROR:
        type_str = "ERROR";
        break;
    case END:
        type_str = "END";
        break;
    default:
        type_str = "UNKNOWN";
        break;
    }

    printf("Token: %-10s Value: %s\n", type_str, token.value);
}

int main()
{
    // Example input (can be replaced with any arithmetic expression)
    const char *input = "3 + 5 * (10 - 2) / 4.5";

    // Get tokens one by one
    Token token;
    while (1)
    {
        token = get_next_token(&input); // Get the next token
        print_token(token);             // Print the token

        if (token.type == END)
        {
            break; // End of input
        }
    }

    return 0;
}
