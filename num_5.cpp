#include <iostream>
#include <cmath>
using namespace std;

struct Polynomial{
    double coefficient; //коэффициент
    int exponent; //показатель
    Polynomial* next;
};

class List{
    private:
    Polynomial* head;

    public:
    List(){
        head = NULL;
    }

    //добавление элементов списка
    void AddList(double coeff, int exp){
        if(coeff == 0){
            return;
        }

        Polynomial* newel = new Polynomial;
        newel->coefficient = coeff;
        newel->exponent = exp;
        newel->next = NULL;

        if(head == NULL){
            head = newel;
        }
        else{
            Polynomial* current = head;
            while(current->next != NULL){
                current = current->next;
            }
            current->next = newel;
        }
    }

    //вычисления в заданной точке
    double GivenPoint(double point){
        double result = 0.0;
        Polynomial* current = head;

        while(current != NULL){
            result += current->coefficient*pow(point, current->exponent);
            current = current->next;
        }

        return result;
    }

    //формирование списка с интегралом
    List* Integral(){
        List* IntegralList = new List;
        Polynomial* current = head;

        while(current != NULL){
            if(current->exponent != -1){
                double newCoeff = current->coefficient/(current->exponent + 1);
                int newExp = current->exponent + 1;
                IntegralList->AddList(newCoeff, newExp);
            }
            else{
                cout << "Деление на нуль";
            }
            current = current->next;
        }
        return IntegralList;
    }

    void Clear(){
        while(head != NULL){
            Polynomial* temp = head;
            head = head->next;
            delete temp;
        }
    }

    double DefiniteIntegral(double a, double b){
        List* integralList = this->Integral();
        double result = integralList->GivenPoint(b) - integralList->GivenPoint(a);
        integralList->Clear();
        delete integralList;
        return result;
    }

    //отображение многочлена
    void Display(){
        Polynomial* current = head;
        while (current!=NULL){
            cout << current->coefficient << "x^" << current->exponent;
            if(current->next != NULL){
                cout << "+";
            }
            current = current->next;
        }
        cout << endl;
    }
};

int main(){
    List polinomList;
    double coeff; 
    int exp;
    char choice;

    do{
        cout << "Введите коэффициент: " << endl;
        cin >> coeff;

        cout << "Введите показатель степени: " << endl;
        cin >> exp;

        polinomList.AddList(coeff, exp);

        cout << "Хотите еще ввести один член?(Y or y - yes)"<<endl;
        cin >> choice;
    } while(choice == 'Y' || choice == 'y');

    cout << "Исходный многочлен: " << endl;
    polinomList.Display();

    double a, b;
    cout << "Нижний предел интегрирования: " << endl;
    cin >> a;
    cout << "Верхний предел интегрирования" << endl;
    cin >> b;

    double definiteIntegral = polinomList.DefiniteIntegral(a, b);
    cout << "Определеный интеграл от " << a << " до " << b << ": " << definiteIntegral << endl;
    
    return 0;
}