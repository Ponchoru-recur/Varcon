#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char sentence[1000];
    printf("Insert sentence ");
    fgets(sentence, sizeof(sentence), stdin);

    char output[1000];
    int outIndex = 0;

    sentence[strlen(sentence) - 1] = '\0';

    for (int i = 0; i < strlen(sentence); i++){
        int found = 0;
        for (int j = 0; j < outIndex; i++)
        {
            if (output[j] == sentence[i]){
                found = 1;
                break;
            }
        }

        if (!found){
            output[outIndex++] = sentence[i];
        }
    }

    output[outIndex] = '\0';

    printf("Output : %s", output);




    return 0;
}
