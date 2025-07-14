#include <stdio.h>
#include <stdlib.h>

struct Info{
    char lastname[100];
    double obl_res;
    double arb_res;
    double total;
};

double Result(int k1, int k2, double res1, double res2){
    return k1*res1 + k2*res2;
}

void Input(FILE* file, int n, int k1, int k2){
    struct Info sport;
    for(int i = 0; i < n; i++){
        printf("Введите фамилию спортсмена: \n");
        scanf("%99s", sport.lastname);
        printf("Введите первую оценку: \n");
        scanf("%lf", &sport.obl_res);
        printf("Введите вторую оценку: \n");
        scanf("%lf", &sport.arb_res);
        sport.total = Result(k1, k2, sport.obl_res, sport.arb_res);
        fwrite(&sport, sizeof(struct Info), 1, file);
    }
}

void View(FILE* file){
    struct Info sport;
    fseek(file, 0, SEEK_SET);
    printf("Содержание: \n");
    while(fread(&sport, sizeof(Info), 1, file) == 1){
        printf("Фамилия: %s\n", sport.lastname);
        printf("Результат обязательной программы: %.2f\n", sport.obl_res);
        printf("Результат произвольной программы: %.2f\n", sport.arb_res);
        printf("Итого: %.2f\n", sport.total);
    }
}

int main(){
    FILE* file;
    file = fopen("num_3.bin", "wb+");
    if(file == NULL){
        printf("Error\n");
        return 1;
    }
    int num;
    int k1, k2;

    printf("Введите весовой коэффициент К1: \n");
    scanf("%d", &k1);
    printf("Введите весовой коэффициент К2: \n");
    scanf("%d", &k2);
    printf("Введите количество спортсменов: \n");
    scanf("%d", &num);

    Input(file, num, k1, k2);
    View(file);

    fclose(file);
    return 0;
}

