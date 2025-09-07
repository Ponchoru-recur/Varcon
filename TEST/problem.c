#include <stdio.h>

// Problem set III
// Sam M. Varela BSCS - 2nd year

int findTheNth(int n){
    if (n < 0) {
        printf("Error no negative numbers allowed!");
        return 0;
    }
    int initialInput = 1;
    int secondLevel = 2;
    const int thirdLevel = 1;

    // printf("%d ", initialInput);
    for (int i = 0; i < n - 1; i++){
        initialInput = initialInput + secondLevel;

        // printf("%d ", initialInput);
        secondLevel += thirdLevel;
    }

    return initialInput;
}

int recurFindNth(int n){
    if (n < 0) {
        printf("Error no negative numbers allowed!");
        return 0;
    }
    if (n == 1){
        return 1;
    }
    return recurFindNth(n - 1) + n;
}

int main() {

    int ask1 = 0;
    int ask2 = 0;

    printf("Ask iteratively : ");
    scanf("%d", &ask1);
    if (findTheNth(ask1) != 0){
        printf("Iteratively : %d", findTheNth(ask1));
    }



    printf("\n");

    printf("Ask recusively : ");

    scanf("%d", &ask2);
    if (recurFindNth(ask2) != 0){
        printf("Recursively : %d",recurFindNth(ask2));
    }

    return 0;
}
