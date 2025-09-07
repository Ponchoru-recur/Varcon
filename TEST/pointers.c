#include <stdio.h>

int lv_power(int base, int power){
    if (power == 0){
        return 1;
    }

    return base * lv_power(base, power - 1);
}

int lv_gcf(int a, int b){
    int aFactors[a];
    int bFactors[b];
    int aFactor_count = 0;
    int bFactor_count = 0;
    int common = 0;

    for (int i = 1; i - 1 < a; i++){
        if (a % i == 0){
            aFactors[aFactor_count++] = i;
        }
    }

    for (int i = 1, j = 0; i - 1 < b; i++){
       if (b % i == 0){
            bFactors[bFactor_count++] = i;
        }
    }


    for (int i = 0; i < aFactor_count; i++)
    {
        printf("%d ", aFactors[i]);
    }
    printf("\n");

    for (int i = 0; i < bFactor_count; i++)
    {
        printf("%d ", bFactors[i]);
    }

    int length = aFactor_count > bFactor_count ? aFactor_count : bFactor_count;

    for (int i = 0; i < aFactor_count; i++)
    {

        for (int j = 0; j < bFactor_count; j++)
        {
            if (aFactors[i] == bFactors[j]){
                common = aFactors[i];
            }
        }

    }

    printf("Common : %d", common);

}

int main() {

    lv_gcf(5, 10);

        return 0;
}
