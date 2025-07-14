#include <iostream>
#include <cctype>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//класс элементов списка
class Node{
    public:
    char data;
    Node* prev, *next;
    public:
    Node(char data){
        this ->data = data;
        this ->prev = this -> next = NULL;
    }
};

//класс для работы со всем списком целиком
class LinkedList{
    public:
    Node* head, *tail;
    public:
    LinkedList(){
        this->head = this->tail = NULL;
    }

    //метод для добавление элементов в начало списка
    Node* push_front(char data){
        Node* ptr = new Node(data);

        ptr->next = head;
        if(head != NULL){
            head -> prev = ptr;
        }
        if(tail == NULL){
            tail  = ptr;
        }
        head = ptr;

        return ptr;
    }

    //метод для добавления элементов в конец списка
    Node* push_back(char data){
        Node* ptr = new Node(data);

        ptr->prev = tail;
        if(tail!=NULL){
            tail->next = ptr;
        }
        if(tail == NULL){
            head = ptr;
        }
        tail = ptr;

        return ptr;
    }

    //удаление элементов в начале списка
    void pop_front(){
        if(head == NULL){
            return;
        }
        Node* ptr = head->next;
        if(ptr != NULL){
            ptr->prev = NULL;
        }
        else{
            tail = NULL;
        }
        delete head;
        head = ptr;
    }

    //удаление элементов в конце списка
    void pop_back(){
        if(tail == NULL){
            return;
        }
        Node* ptr = tail->prev;
        if(ptr != NULL){
            ptr->next = NULL;
        }
        else{
            head = NULL;
        }
        delete tail;
        tail = ptr;
    }

    //метод получения произвольного элемента по индексу
    Node* get_at(int index){
        Node* ptr = head;
        int n = 0;

        while(n!=index){
            if(ptr == NULL){
                return ptr;
            }
            ptr = ptr->next;
            n++;
        }
        return ptr;
    }

    //вызов элемента как в массиве
    Node* operator [] (int index){
        return get_at(index);
    }

    //вставка нового элемента в позицию с индексом
    Node* insert(int index, char data){
        Node* right = get_at(index);
        if(right == NULL){
            return push_back(data);
        }
        Node* left = right ->prev;
        if(left == NULL){
            return push_front(data);
        }
        Node* ptr = new Node(data);

        ptr->prev = left;
        ptr->next = right;
        left->next = ptr;
        right->prev = ptr;

        return ptr;
    }

    //удаление элемента по индексу
    void erase(int index){
        Node* ptr = get_at(index);
        if(ptr == NULL){
            return;
        }
        if(ptr->prev == NULL){
            pop_front();
            return;
        }
        if(ptr->next == NULL){
            pop_back();
            return;
        }

        Node* left = ptr->prev;
        Node* right = ptr->next;
        left->next = right;
        right->prev = left;

        delete ptr;
    }

    void remove_digits(char letter){
        Node* ptr = head;
        while(ptr != NULL){
            if((*ptr).data == letter){
                Node* current = ptr->prev;
                while(current!=NULL && isdigit(static_cast<int>(current->data))){
                    Node* del = current;
                    current = current->prev;
                    if(current != NULL){
                        current->next = ptr;
                    }
                    ptr->prev = current;
                    if(del == head){
                        head = ptr;
                    }
                    delete del;
                }
            }
            ptr = ptr->next;
        }
    }
    void print(){
        Node* ptr = head;
        while(ptr != NULL){
            cout << ptr->data << " ";
            ptr = ptr->next;
        }
        cout << endl;
    }

    ~LinkedList(){
        while(head!=NULL){
            pop_front();
        }
    }
};

int main(){
    ofstream outfile("num_6.txt");
    if(!outfile.is_open()){
        cerr << "Error!!!" << endl;
        return 1;
    }
    vector<char> newsymbols;
    char input;
    cout << "Введите символы(конец - '0')"<<endl;
    while(cin >> input && input != '0'){
        newsymbols.push_back(input);
    }
    for(char ch : newsymbols){
        outfile << ch << " ";
    }
    outfile.close();

    ifstream file("num_6.txt");
    if(!file.is_open()){
        cerr << "Error!!!" << endl;
        return 1;
    }
    LinkedList list;
    char symbol;
    while(file>>symbol){
        list.push_back(symbol);
    }
    file.close();
    cout << "Исходный список: " << endl;
    list.print();
    char letter;
    cout << "Введите букву: " << endl;
    cin >> letter;
    list.remove_digits(letter);
    cout << "Полученный список: " << endl;
    list.print();
    return 0;
}
    