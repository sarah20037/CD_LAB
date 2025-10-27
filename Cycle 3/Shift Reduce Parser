#include<stdio.h>
#include<string.h>

#define MAX_RULES 20
#define MAX_LEN 20

char stack[100];
int top=-1;

struct Rule
{
    char lhs;
    char rhs[MAX_LEN];
};

void push(char c)
{
    stack[++top]=c;
    stack[top+1]='\0';
}

void pop (int n)
{
    top-=n;
    if(top<-1)
        top=-1;
    stack[top+1]='\0';
}

int reduce(struct Rule rules[],int ruleCount)
{
    for(int i=0;i<ruleCount;i++)
    {
        int len=strlen(rules[i].rhs);
        if(top+1>=len)//check if stack has enough symbols
        {
            if(strncmp(&stack[top-len+1],rules[i].rhs,len)==0)//slices top len symbols and check if they match 
            {
                printf("%-20s %-20s REDUCE %s -> %c\n", stack, "", rules[i].rhs, rules[i].lhs);
                pop(len-1);//remove matched symbols
                stack[top]=rules[i].lhs;//push lhs symbol
                stack[top+1]='\0';
                return 1;
            }
        }
    }
    return 0;
}

int main()
{
    struct Rule rules[MAX_RULES];
    int n;
    char input[50];

    printf("Enter number of rules: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++)//Reads productions
    {
        char line[MAX_LEN];
        scanf("%s",line);
        rules[i].lhs=line[0];//stores lhs part
        strcpy(rules[i].rhs,line+2);//stores corresponding rhs
    }

    printf("Enter input:");//Reads expression
    scanf("%s",input);
    
    printf("\n%-20s %-20s ACTION\n", "STACK", "INPUT");
    printf("-------------------------------------------------------------\n");

    int i=0;
    while(input[i]!='\0')//Loop until end of input
    {
        printf("%-20s %-20s SHIFT %c\n", stack, input + i, input[i]);//
        push(input[i]);//push current input symbol to stack
        i++;//move to next input symbol 
        while(reduce(rules,n));//check if it matches any production
    }
    while(reduce(rules,n));//check one more time for any matches.
    char startSymbol = rules[0].lhs;

    if (top == 0 && stack[top] == startSymbol)//check is any symbol is left and if the symbol matches start symbol
        printf("%-20s %-20s ACCEPTED \n", stack, "");
    else
        printf("Parsing Error!\n");

    return 0;
}
