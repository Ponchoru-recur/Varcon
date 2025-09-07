
#include <stdio.h>

// A

int main(){

    float friendHouse;
    float steps = 0;
    int perStep = 0;

    while (1){
        printf("Friend's house is at : ");
        scanf("%f", &friendHouse);
        if (steps >= 1 || steps <= 1000000){
            break;
        }
        printf("Number range not allowed!\n");
    }


    for (int i = 1; i < 6; i++){
        float friendHouseCalc = friendHouse / i;
        if (steps > friendHouseCalc || i == 1){
            steps = friendHouseCalc;
            perStep = i;
        }
    }

    printf("number of steps %d, per step %d", (int)steps, perStep);

    return 0;
}
