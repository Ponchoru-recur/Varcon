#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// Sam M. Varela - Set B
// Guide
/*
    How many drinks : 3
    Ratio of : 50 50 100
    output : 66.666667
*/

// Defining TRUE as 1 and versa
#define TRUE 1
#define FALSE 0

// Convert string like inputted numbers into user input
short parseUserInput(int drinkAmount, int sizeOfBuffer, char* buffer, int *sum) {
    /*
        t is for index
        t_portionAmount is a temporary char string made to insert portion numbers of buffer
        which will then used to convert into a number
    */
    int t = 0;
    char t_portionAmount[16] = {0};
    int count = 0; // How many numbers were parsed to check if it's equal to the amount of drinks

    *sum = 0; // Reset before doing anything

    for (int i = 0; i < sizeOfBuffer; i++){
        // This will convert the t_portionAmount char number into an Integer using atoi function
        // This will only trigger each time it sees a space
        if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\0' || buffer[i] == '\r') {
            if (t > 0){
                t_portionAmount[t] = '\0';
                int val = atoi(t_portionAmount);
                if (val > 100){ // Checking if each string number isnt more than 100.
                    printf("Juice ratio cannot exceed more than 100 or less than 0.\n");
                    *sum = 0;
                    return FALSE;
                }
                *sum += val;
                count++;
                t = 0; // reset for next number
            }
        } else if (isdigit(buffer[i])){ // Check if it's even a number
            // This will fill [t_portionAmount] with the number of [buffer]
            if (t < (int)(sizeof(t_portionAmount) - 1)) { // avoid overflow
                t_portionAmount[t] = buffer[i];
                t++;
            }
        } else if (buffer[i] == '-') { // This is executed if something is not recognized as a number
            printf("No negative/non numbers.\n");
            return FALSE;
        }
    }

    // Guard to check whether the amount of drinks vs ratio input is the same.
    if (count != drinkAmount){
        printf("Expected %d numbers but got %d.\n", drinkAmount, count);
        *sum = 0;
        return FALSE;
    }

    return TRUE;
}

int main(){

    // Local variables
    int sum = 0;
    int drinkAmount = 0;

    // Ask user how many drinks
    while (TRUE){
        printf("How many drinks : ");
        scanf("%d", &drinkAmount);
        getchar(); // We tell compiler to eat the newline (\n) or else it will ignore fgets
        // TDLR - fgets will immediately finish if it sees \n

        // Guard to protect user from inputting invalid values
        if (drinkAmount > 0 && drinkAmount <= 100){ break;}
        printf("[Error] Does not allow negative, non-numbers and values exceeding 100.\n");
    }

    /*
        For each drink it needs 4 bytes because each input is 3 characters long (1-100) and one space.
        The space is for detection if there's another number coming allowing stream line input.
        The last +1 is for nullterminator.
    */
    int sizeOfBuffer = drinkAmount * 4 + 1;

    // Allocate memory for Juice ratio then ask for input using fgets to get spaces.
    char *juiceRatioBuffer = malloc(sizeOfBuffer);


    // This will repeat to ask question to user until corrected format.
    while (TRUE)
    {
        // Ask user for input
        printf("Ratio of : ");
        fgets(juiceRatioBuffer, sizeOfBuffer, stdin);

         // Parse or decode the string of numbers
        if (!parseUserInput(drinkAmount, sizeOfBuffer, juiceRatioBuffer, &sum)) {
            printf("Please type in correct format.\n");
        } else{
            // Print output
            printf("output : %.6f\n", (double)sum / (double)drinkAmount);
            break;
        }
    }

    // Free allocated memeory
    free(juiceRatioBuffer);

    return 0;
}

/*PS: I am going insane, I have made all possible errors I could think of. */
