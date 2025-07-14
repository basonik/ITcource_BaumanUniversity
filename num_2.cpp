#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int main(){
    FILE *file;
    file = fopen("infs.txt", "w+");
    if(file == nullptr){
        printf("Error\n");
        return 1;
    }
    char choice;

    char name[100]; //имя
    char sign[100]; //признак
    int price; //цена

    char max_name[100], max_sign[100];
    char min_name[100], min_sign[100];
    int max_price = 0;
    int min_price;
    bool firstEl = true;

    do{
        printf("Введите название газеты/журнала: \n");
        scanf("%s", name);
        printf("Введите признак(газета/журнал): \n");
        scanf("%s", sign);
        printf("Введите цену: \n");
        scanf("%d", &price);

        if(price > max_price){
            max_price = price;
            strcpy(max_name, name);
            strcpy(max_sign, sign);
        }

        if(firstEl){
            min_price = price;
            strcpy(min_name, name);
            strcpy(min_sign, sign);
            firstEl = false;
        } else if(price < min_price){
            min_price = price;
            strcpy(min_name, name);
            strcpy(min_sign, sign);
        }

        fprintf(file, "%s %s %d\n",name, sign, price);

        printf("Хотите продолжить запись?(y/Y - yes)\n");
        scanf(" %c", &choice);
    }while(choice == 'y' || choice == 'Y');
    fclose(file);

    FILE* pricesFile;
    pricesFile = fopen("prices.txt", "w+");
    if(pricesFile == NULL){
        printf("Error\n");
        return 1;
    }
    fprintf(pricesFile, "Максимальная цена: %s %s %d\n",max_name, max_sign, max_price);
    fprintf(pricesFile, "Минимальная цена: %s %s %d\n",min_name, min_sign, min_price);

    fclose(pricesFile);

    return 0;
}