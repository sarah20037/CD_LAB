%{
#include <stdio.h>
#include <stdlib.h>

// AST Node definition
typedef struct AST {
    char op;
    int val;
    struct AST* left;
    struct AST* right;
} AST;

// Function prototypes
AST* createNumNode(int val);
AST* createOpNode(char op, AST* left, AST* right);
void printAST(AST* root, int level);
void yyerror(const char *s);
int yylex();
%}

%union {
    int num;
    AST* node;
}

%token NUM
%type <node> expr

%%

stmt: expr {
    printf("Abstract Syntax Tree:\n");
    printAST($1, 0);
};

expr: NUM {
    $$ = createNumNode($1);
}
| expr '+' expr {
    $$ = createOpNode('+', $1, $3);
}
| expr '-' expr {
    $$ = createOpNode('-', $1, $3);
}
| expr '*' expr {
    $$ = createOpNode('*', $1, $3);
}
| expr '/' expr {
    $$ = createOpNode('/', $1, $3);
}
| '(' expr ')' {
    $$ = $2;
};

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

AST* createNumNode(int val) {
    AST* node = (AST*) malloc(sizeof(AST));
    node->op = '\0';
    node->val = val;
    node->left = node->right = NULL;
    return node;
}

AST* createOpNode(char op, AST* left, AST* right) {
    AST* node = (AST*) malloc(sizeof(AST));
    node->op = op;
    node->val = 0;
    node->left = left;
    node->right = right;
    return node;
}

void printAST(AST* root, int level) {
    if (root == NULL) return;
    for (int i = 0; i < level; i++) printf("  ");
    if (root->op == '\0')
        printf("%d\n", root->val);
    else
        printf("%c\n", root->op);
    printAST(root->left, level + 1);
    printAST(root->right, level + 1);
}

int main() {
    printf("Enter an arithmetic expression:\n");
    yyparse();
    return 0;
}
