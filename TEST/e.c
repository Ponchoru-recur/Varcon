#include <stdio.h>


// This eliminates duplicates leaving only the unique numbers.
int noneUniqueElimination(int *arr, int n){
    int i = 0;
     int writeIndex = 0;
    while(i < n){
        int j = i + 1;
         // count how many times arr[i] repeats
        while (j < n && arr[j] == arr[i]) {
            j++;
        }

        // if it appears once, keep it
        if (j == i + 1) {
            arr[writeIndex++] = arr[i];
        }

        // move to next group
        i = j;
    }

    return writeIndex; // new size

}

// Algorithm for sorting that checks it's neighbor then replaces -
// it if it's greater than the previous number
void bubbleSort(int *arr, int n){
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] < arr[j + 1]){
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

}

//Find and return its nth
char* nth(int n){
    int theLastTwo = n % 100; // You can take the last two digits of the num
    int theLast = n % 10;  // Take the last digit

    // Compare check pattern if its more than 4 and less than 13 (th)
    if (theLast >= 4 && 13 <= theLastTwo){
        return "th";
    } else if (theLast == 1) {
        return "st";
    } else if (theLast == 2) {
        return "nd";
    } else if (theLast == 3){
        return "rd";
    } else {
        return "th";
        printf("[Achievement unlock] How did i not see this?\n");
    }
}



int main() {
    int n = 0;

    // #2 Guard for undefined behavior
    while (1)
    {
        // Ask the user for array size
        printf("Array of size N : ");
        scanf("%d", &n);

        // Not allowing the user to declare a negative range
        if (n <= 0) {
            printf("Negative range is not allowed.\n");
            continue;
        }
        break;
    }

    // Declare an array for storage
    int arr[n];
    int arraySize = sizeof(arr) / sizeof(*arr);

    // Continously asks user for input to put inside array
    printf("Please enter %d integers\n", n);
    for (int i = 0; i < n; i++){
        int temp;
        printf("Insert integer : ");
        scanf("%d", &temp);
        arr[i] = temp;
    }

    //Sort the array
    bubbleSort(arr, arraySize);

    int newSize = noneUniqueElimination(arr, arraySize);

    // #2 Guard for undefined behavior
    while (1){
        printf("Nth place to look : ");
        scanf("%d", &n);

        // Check if user inserts a number greater than the range
        if (n <= 0 || n > newSize){
            printf("[Error] range is either too large or less than 0.\n");
            continue;
        }
        printf("%d%s largest : %d\n", n, nth(n), arr[n - 1]);
        break;
    }

    return 0;
}
