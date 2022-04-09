#include <stdio.h>

int a[16][16];
int cnt;
FILE *fis;

void print(){

    for(int i = 0; i < 16; i++){

        for(int j = 0; j < 16; j++){
            printf("%3d ", a[i][j]);
            fprintf(fis, "%d,", a[i][j]);
        }
        
        printf("\n");
        fprintf(fis, "\n");
    }
}

int main(void){

    

    fis = fopen("data.txt", "w");

    for(int j = 15; j >= 0; j--){

        if(j % 2 == 1){
            for(int i = 15; i >= 0; i--){
                a[i][j] = cnt;
                cnt++;
            }
        }else{

            for(int i = 0; i <= 15; i++){
                a[i][j] = cnt;
                cnt++;
            }
        }

    }

    print();
    fclose(fis);

}