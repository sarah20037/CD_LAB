#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char prod[MAX][20];
char first[26][50], follow[26][50];
int n;

void add(char set[], char c) {
    if (!strchr(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

void buildFirst() {
    for (int i = n - 1; i >= 0; i--) {
        char A = prod[i][0];
        char X = prod[i][2];

        if (!isupper(X)) add(first[A - 'A'], X);
        else {
            for (int k = 0; first[X - 'A'][k]; k++)
                add(first[A - 'A'], first[X - 'A'][k]);
        }

        for (int j = 3; prod[i][j]; j++) {
            if (isupper(prod[i][j - 1]) && strchr(first[prod[i][j - 1] - 'A'], 'e')) {
                char next = prod[i][j];
                if (!isupper(next)) add(first[A - 'A'], next);
                else
                    for (int k = 0; first[next - 'A'][k]; k++)
                        add(first[A - 'A'], first[next - 'A'][k]);
            } else break;
        }
    }
}

void buildFollow() {
    add(follow[prod[0][0] - 'A'], '$'); // start symbol gets $

    for (int pass = 0; pass < 3; pass++) { // repeat to propagate
        for (int i = n - 1; i >= 0; i--) {
            char A = prod[i][0];
            int len = strlen(prod[i]);

            for (int j = 2; j < len; j++) {
                char B = prod[i][j];

                if (isupper(B)) {
                    char next = prod[i][j + 1];

                    if (next == '\0') {
                        // at end: FOLLOW(A) -> FOLLOW(B)
                        for (int k = 0; follow[A - 'A'][k]; k++)
                            add(follow[B - 'A'], follow[A - 'A'][k]);
                    } 
                    else if (!isupper(next)) {
                        add(follow[B - 'A'], next);
                    } 
                    else {
                        // FIRST(next)
                        for (int k = 0; first[next - 'A'][k]; k++) {
                            if (first[next - 'A'][k] != 'e')
                                add(follow[B - 'A'], first[next - 'A'][k]);
                        }
                        // If next can produce epsilon, also add FOLLOW(A)
                        if (strchr(first[next - 'A'], 'e'))
                            for (int k = 0; follow[A - 'A'][k]; k++)
                                add(follow[B - 'A'], follow[A - 'A'][k]);
                    }
                }
            }
        }
    }
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);
    printf("Enter productions (use e for epsilon):\n");
    for (int i = 0; i < n; i++) scanf("%s", prod[i]);

    buildFirst();
    buildFollow();

    printf("\nFIRST sets:\n");
    for (int i = 0; i < n; i++)
        if (i == 0 || prod[i][0] != prod[i - 1][0])
            printf("FIRST(%c) = { %s }\n", prod[i][0], first[prod[i][0] - 'A']);

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < n; i++)
        if (i == 0 || prod[i][0] != prod[i - 1][0])
            printf("FOLLOW(%c) = { %s }\n", prod[i][0], follow[prod[i][0] - 'A']);

    return 0;
}
