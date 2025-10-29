#include <stdio.h>
#include <string.h>

char stack[50];
int top = -1;

// Precedence table
//       +    *    i    (    )    $
char precedence[6][6] = {
    // +    *    i    (    )    $
    {'>', '<', '<', '<', '>', '>'}, // +
    {'>', '>', '<', '<', '>', '>'}, // *
    {'>', '>', ' ', ' ', '>', '>'}, // i
    {'<', '<', '<', '<', '=', ' '}, // (
    {'>', '>', ' ', ' ', '>', '>'}, // )
    {'<', '<', '<', '<', ' ', '='}  // $
};

int getIndex(char c)
{
    switch (c)
    {
    case '+': return 0;
    case '*': return 1;
    case 'i': return 2;
    case '(': return 3;
    case ')': return 4;
    case '$': return 5;
    default: return -1;
    }
}

void push(char c)
{
    stack[++top] = c;
}

void pop()
{
    if (top >= 0)
        top--;
}

char peek()
{
    if (top >= 0)
        return stack[top];
    return '\0';
}

int main()
{
    char input[50];
    printf("Enter input: ");
    scanf("%s", input);

    // Append $ at end
    int len = strlen(input);
    input[len] = '$';
    input[len + 1] = '\0';

    push('$'); // initialize stack with $

    int i = 0;
    char a = input[i];

    printf("\nSTACK\t\tINPUT\t\tACTION\n");
    printf("---------------------------------\n");

    while (1)
    {
        char topc = peek();

        // display current stack and remaining input
        for (int j = 0; j <= top; j++)
            printf("%c", stack[j]);
        printf("\t\t%s\t\t", &input[i]);

        if (topc == '$' && a == '$')
        {
            printf("String Accepted!\n");
            break;
        }

        int row = getIndex(topc);
        int col = getIndex(a);

        if (row == -1 || col == -1)
        {
            printf("Invalid symbol!\n");
            break;
        }

        char relation = precedence[row][col];

        if (relation == '<' || relation == '=')
        {
            // shift
            push(a);
            printf("Shift %c\n", a);
            a = input[++i];
        }
        else if (relation == '>')
        {
            // reduce
            printf("Reduce\n");
            pop();
        }
        else
        {
            printf("Error: Invalid relation\n");
            break;
        }
    }

    return 0;
}
