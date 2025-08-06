#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

// Transition table: from state i to state j on symbols (as a string)
char tran[MAX][MAX][MAX];

// Epsilon closures
int eclosure[MAX][MAX];
int closurecount[MAX];

// DFA states (sets of NFA states)
int dfastate[MAX][MAX];
int dfastatecount[MAX];

// Transition symbols
char input[MAX];
int n_alpha, n_state, n_trans, start, n_final;
int final[MAX];

// DFA transition table
int dfa[MAX][MAX];
int dfa_state_total = 0;

int is_final_state(int state) {
    for (int i = 0; i < n_final; i++) {
        if (final[i] == state)
            return 1;
    }
    return 0;
}

int find_alpha_index(char ch) {
    for (int i = 0; i < n_alpha; i++) {
        if (input[i] == ch)
            return i;
    }
    return -1;
}

// Append to transition string
void add_transition(int from, int to, char symbol) {
    if (strchr(tran[from][to], symbol) == NULL) {
        int len = strlen(tran[from][to]);
        tran[from][to][len] = symbol;
        tran[from][to][len + 1] = '\0';
    }
}

void compute_eclosure() {
    for (int i = 0; i < n_state; i++) {
        int visited[MAX] = {0}, stack[MAX], top = -1;
        eclosure[i][0] = i;
        closurecount[i] = 1;
        visited[i] = 1;
        stack[++top] = i;

        while (top >= 0) {
            int curr = stack[top--];
            for (int j = 0; j < n_state; j++) {
                if (strchr(tran[curr][j], 'e') && !visited[j]) {
                    eclosure[i][closurecount[i]++] = j;
                    visited[j] = 1;
                    stack[++top] = j;
                }
            }
        }
    }
}

// Returns index if DFA state exists, else -1
int get_dfa_state_index(int state_list[], int count) {
    for (int i = 0; i < dfa_state_total; i++) {
        if (dfastatecount[i] != count) continue;
        int match = 1;
        for (int j = 0; j < count; j++) {
            if (dfastate[i][j] != state_list[j]) {
                match = 0;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

void sort_state_list(int list[], int count) {
    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (list[i] > list[j]) {
                int t = list[i];
                list[i] = list[j];
                list[j] = t;
            }
}

void subset_construction() {
    int queue[MAX][MAX], front = 0, rear = 0;
    int initial[MAX], init_count = 0;

    for (int i = 0; i < closurecount[start]; i++)
        initial[i] = eclosure[start][i];
    init_count = closurecount[start];
    sort_state_list(initial, init_count);

    memcpy(dfastate[0], initial, sizeof(int) * init_count);
    dfastatecount[0] = init_count;
    dfa_state_total = 1;

    memcpy(queue[rear++], initial, sizeof(int) * init_count);
    int queuecount[MAX]; queuecount[0] = init_count;

    while (front < rear) {
        int *curr = queue[front];
        int currcount = queuecount[front];
        int curr_index = get_dfa_state_index(curr, currcount);
        front++;

        for (int a = 0; a < n_alpha - 1; a++) {
            int move[MAX], move_count = 0;
            for (int i = 0; i < currcount; i++) {
                int state = curr[i];
                for (int j = 0; j < n_state; j++) {
                    if (strchr(tran[state][j], input[a])) {
                        for (int k = 0; k < closurecount[j]; k++) {
                            int s = eclosure[j][k];
                            int exists = 0;
                            for (int m = 0; m < move_count; m++) {
                                if (move[m] == s) {
                                    exists = 1;
                                    break;
                                }
                            }
                            if (!exists)
                                move[move_count++] = s;
                        }
                    }
                }
            }

            sort_state_list(move, move_count);
            int idx = get_dfa_state_index(move, move_count);
            if (idx == -1 && move_count > 0) {
                memcpy(dfastate[dfa_state_total], move, sizeof(int) * move_count);
                dfastatecount[dfa_state_total] = move_count;
                memcpy(queue[rear], move, sizeof(int) * move_count);
                queuecount[rear] = move_count;
                idx = dfa_state_total;
                dfa_state_total++;
                rear++;
            }

            dfa[curr_index][a] = idx;
        }
    }
}

void print_dfa() {
    printf("\nDFA Transition Table:\n");
    printf("---------------------\n\t");
    for (int i = 0; i < n_alpha - 1; i++)
        printf("%c\t", input[i]);
    printf("\n");

    for (int i = 0; i < dfa_state_total; i++) {
        printf("{");
        for (int j = 0; j < dfastatecount[i]; j++) {
            printf("q%d", dfastate[i][j]);
            if (j != dfastatecount[i] - 1)
                printf(" ");
        }
        printf("}\t");
        for (int k = 0; k < n_alpha - 1; k++) {
            int to = dfa[i][k];
            if (to == -1)
                printf("-\t");
            else {
                printf("{");
                for (int j = 0; j < dfastatecount[to]; j++) {
                    printf("q%d", dfastate[to][j]);
                    if (j != dfastatecount[to] - 1)
                        printf(" ");
                }
                printf("}\t");
            }
        }
        printf("\n");
    }
}

int main() {
    memset(tran, 0, sizeof(tran));
    memset(dfa, -1, sizeof(dfa));

    printf("Enter number of alphabets: ");
    scanf("%d", &n_alpha);
    getchar();
    printf("Enter the alphabets (e for epsilon, should be last):\n");
    for (int i = 0; i < n_alpha; i++) {
        scanf("%c", &input[i]);
        getchar();
    }

    printf("Enter number of states: ");
    scanf("%d", &n_state);
    printf("Enter number of transitions: ");
    scanf("%d", &n_trans);
    printf("Enter transitions (from symbol to):\n");
    for (int i = 0; i < n_trans; i++) {
        int from, to;
        char ch;
        scanf("%d %c %d", &from, &ch, &to);
        add_transition(from, to, ch);
    }

    printf("Enter the start state: ");
    scanf("%d", &start);
    printf("Enter number of final states: ");
    scanf("%d", &n_final);
    printf("Enter final states: ");
    for (int i = 0; i < n_final; i++)
        scanf("%d", &final[i]);

    compute_eclosure();
    subset_construction();
    print_dfa();

    return 0;
}
