#include <stdio.h>
#include <string.h>

char stack[50];
int top=-1;

void push(char c)
{
    stack[++top]=c;
}
void pop()
{
    if(top>=0)
    top--;
}

char peek()
{
    if(top>=0)
    return stack[top];
    return '\0';
}

char precedence[6][6] = {
    // +    *    i    (    )    $
    {'>', '<', '<', '<', '>', '>'}, // +
    {'>', '>', '<', '<', '>', '>'}, // *
    {'>', '>', ' ', ' ', '>', '>'}, // i
    {'<', '<', '<', '<', '=', ' '}, // (
    {'>', '>', ' ', ' ', '>', '>'}, // )
    {'<', '<', '<', '<', ' ', '='}  // $
};

int getindex(char c)
{
    switch(c)
    {
        case '+':return 0;
        case '*':return 1;
        case 'i':return 2;
        case '(':return 3;
        case ')':return 4;
        case '$':return 5;
    }
    return -1;
}

int main()
{
    char input[50];
    printf("enter the input \n");
    scanf("%s",input);
    strcat(input,"$");
    push('$');
    int i=0;
    char a=input[i];
    printf("\nSTACK\t\tINPUT\t\tACTION\n");
    while(1)
    {
        printf("%s\t%s\t\t",stack,input+i);
        char topc=peek();
        if(topc=='$'&&a=='$')
        {
            printf("ACCEPTED\n");
            break;
        }
        if(a=='$')
        {
            printf("REDUCE\n");
            pop();
            continue;
        }
        int row=getindex(topc);
        int col=getindex(a);
        char rel=precedence[row][col];
        if(rel=='<'||rel=='=')
        {
            printf("SHIFT %c\n",a);
            push(a);
            a=input[++i];
            
        }
        else if(rel=='>')
        {
            printf("REDUCE\n");
            pop();
        }
        else
        {
            printf("invalid relation \n");
            break;
        }
    }
    return 0;
    
}
