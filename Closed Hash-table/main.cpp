#include <cstdlib>
#include <iostream>
#include <cmath>

const int N_MAX = 10000;

using namespace std;

struct Node       //Структура являющаяся звеном списка
{
    int x;//Значение x будет передаваться в список
    Node *next, *prev; //Указатели на адреса следующего и предыдущего элементов списка
};


struct List   //Создаем тип данных Список
{
    Node *head;
    Node *tail;  //Указатели на адреса начала списка и его конца
};

int myHash(int x){
    return hash<int>()(x) % N_MAX;
}

struct ClosedHashTable{
    List *values = new List[N_MAX];
};

void add( ClosedHashTable *table, int x)
{
    Node *temp = new Node(); // Выделение памяти под новый элемент структуры
    temp->next = NULL;       // Указываем, что изначально по следующему адресу пусто
    temp->x = x;             // Записываем значение в структуру
    int key = myHash(x);
    List &current = table->values[key];
    while (current.head != NULL && current.head->x != 0){
        key++;
        if (key == N_MAX) {
            key = 0;
        }
        current = table->values[key];
    }
    temp->prev = NULL; // Предыдущий элемент указывает в пустоту
    current.head = current.tail = temp;    // Голова=Хвост=тот элемент, что сейчас добавили
}

void find(ClosedHashTable *table, int check_x){

    int key = myHash(check_x);
    List &current = table->values[key];
    //Сделаем индикатор нахождения элементов
    int notFound = 1;
    while (current.head != NULL){
        if (current.head->x == check_x){
            cout << "FOUND!" << endl;
            notFound = 0;
            break;
        }
        key++;
        if (key == N_MAX) {
            key = 0;
        }
        current = table->values[key];
    }
    if (notFound){
        cout << "NOT FOUND =(" << endl;
    }
}

void pop(ClosedHashTable *table, int pop_x){
    /*
     * Нулями заполняем элементы, которые были удалены
     */
    int key = myHash(pop_x);
    List &current = table->values[key];
    while (current.head != NULL){
        if (current.head->x == pop_x){
            current.head->x = 0;
            break;
        }
        key++;
        if (key == N_MAX) {
            key = 0;
        }
        current = table->values[key];
    }
}

int main(void){

    ClosedHashTable *table = new ClosedHashTable;
    for (int i = 0; i < 20; i++) {
        int x = rand() / 1000000;
        add(table, x);
    }
    find(table, 1804);
    pop(table, 1804);
    find(table, 1804);
    return 0;
}