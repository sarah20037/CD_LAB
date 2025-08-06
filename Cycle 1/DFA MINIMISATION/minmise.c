#include <stdio.h>
#include <stdlib.h>

int findalpha(char inp, int a, char alphabet[a]){
    for(int i=0; i<a; i++){
        if(alphabet[i] == inp){
            return i;
        }
    }
    return -1;
}

int reset(int n, int table[n][n]){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            table[i][j] = 0;
        }
    }
}

int isFinal(int i, int f, int final[f]){
    for(int j=0; j<f; j++){
        if(final[j] == i){
            return 1;
        }
    }
    return 0;
}

int printTable(int n, int table[n][n]){
    printf("\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%d ", table[i][j]);
        }
        printf("\n");
    }
}

int merge(int n, int empty[n][n], int i, int j){
    int flag = 0;
    for(int k=0; k<n; k++){
        if(empty[i][k] == 1 && empty[j][k]==1){
            flag = 1;
            break;
        }
    }

    if(flag){
        for(int k=0; k<n; k++){
            if(empty[j][k] == 1){
                empty[i][k] = 1;
                empty[j][k] = 0;
            }
        }
    }
}

int main(){
    int n, a, from, to, end, index;
    char state1[3], state2[3], inp[2];

    printf("Enter total no of states: ");
    scanf("%d", &n);

    printf("Enter total size of alphabet: ");
    scanf("%d", &a);
    
    char alphabet[a];
    int transition[n][a];
    int table[n][n];

    reset(n, table);

    printf("Enter alphabet without space: ");
    scanf("%s", alphabet);

    int f;
    printf("Enter no of final states: ");
    scanf("%d", &f);
    int final[f];

    for(int i=0; i<f; i++){
        scanf("%d", &final[i]);
    }

    //Build Transition Matrix
    FILE *INPUT = fopen("DFA.txt", "r");
    while((end = fscanf(INPUT, "%s %s %s", state1, inp, state2)) != EOF){
        from = state1[1] - '0';
        to = state2[1] - '0';
        index = findalpha(inp[0], a, alphabet);
        transition[from][index] = to;
    }

    //Mark initial pairs
    for(int i=0; i<n; i++){
        for(int j=0; j<i; j++){
            if( (isFinal(i, f, final) && !isFinal(j, f, final)) || (isFinal(j, f, final) && !isFinal(i, f, final)) ){
                table[i][j] = 1;
            }
        }
    }


    printf("\nAfter Marking initial pairs:\n");

    printTable(n, table);
    int out1, out2, flag = 1;

    while(flag){
        flag = 0;

        for(int i=0; i<n; i++){
            for(int j=0; j<i; j++){

                if(table[i][j] == 1)
                    continue;

                for(int k=0; k<a; k++){
                    out1 = transition[i][k];
                    out2 = transition[j][k];

                    if(table[out1][out2] == 1 || table[out2][out1] == 1){
                        table[i][j] = 1;
                        flag = 1;
                    }
                }
                
            }
        }
    }
    printf("\nFinal My-Hill Nerode theorem:\n");
    printTable(n, table);

    int empty[n][n], k=0;
    reset(n, empty);
    int flagarr[n];

    for(int i = 0; i<n; i++)
        flagarr[i] = 0;

    for(int i=0; i<n; i++){
        for(int j=0; j<i; j++){
            if(table[i][j] == 0){
                empty[k][i] = 1;
                empty[k][j] = 1;
                flagarr[i] = 1;
                flagarr[j] = 1;
                k++;
            }
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i!=j){
                merge(n, empty, i, j);
            }
        }
    }

    for(int i= 0; i<n; i++){
        if(flagarr[i] == 0){
            empty[k][i] = 1;
            k += 1;
        }
    }
    printTable(n, empty);

    for(int i=0; i<n; i++){
        int j;
        for(j=0; j<n && empty[i][j]==0; j++);

        if(j==n) continue;

        for(int b=0; b<a; b++){
            // Printing start set
            printf("{");
            for(int k=0; k<n; k++){
                if(empty[i][k] == 1)
                    printf("q%d, ", k);
            }
            printf("}\t");

            // Printing input symbol
            printf("%c \t", alphabet[b]);

            // Printing to set
            int to_state = transition[j][b];

            for(int y=0; y<n; y++){
                if(empty[y][to_state] == 1){
                    printf("{");
                    for(int k=0; k<n; k++){
                        if(empty[y][k] == 1)
                            printf("q%d, ", k);
                    }
                    printf("}\n");
                }
            }

        }
    }    
}
