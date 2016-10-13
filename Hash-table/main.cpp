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

struct hashTable{
    List *values = new List[N_MAX];
};

void add( hashTable *table, int x)
{
    Node *temp = new Node(); // Выделение памяти под новый элемент структуры
    temp->next = NULL;       // Указываем, что изначально по следующему адресу пусто
    temp->x = x;             // Записываем значение в структуру
    int key = myHash(x);
    List &current = table->values[key];
    if ( current.head != NULL ) // Если список не пуст
    {
        temp->prev = current.tail; // Указываем адрес на предыдущий элемент в соотв. поле
        current.tail->next = temp; // Указываем адрес следующего за хвостом элемента
        current.tail = temp;       //Меняем адрес хвоста
    }
    else //Если список пустой
    {
        temp->prev = NULL; // Предыдущий элемент указывает в пустоту
        current.head = current.tail = temp;    // Голова=Хвост=тот элемент, что сейчас добавили
    }
}

int find(hashTable *table, int check_x){

    int key = myHash(check_x);
    Node * temp = (table->values[key]).head;
    int i = -1;
    if (temp != NULL)
        i = 0;
    while (temp != NULL){
        if ((temp->x == check_x)) {
            break;
        }
        temp = temp->next;
        i++;
    }
    return i;
}

void pop(hashTable *table, int pop_x){
    int key = myHash(pop_x);
    Node * temp = (table->values[key]).head;
    int index = find(table, pop_x);
    if (index == 0) {
        (table->values[key]).head = temp->next;
        temp->next->prev = NULL;
        delete temp;
    } else {
        for (int i = 1; i < index; i++) {
            temp = temp->next;
        }
        Node *new_tmp = temp->next->next;
        temp->next = new_tmp;
        new_tmp->prev = temp;
        delete temp;
    }
}

int main(void){

    hashTable *table = new hashTable;
    for (int i = 0; i < 20; i++) {
        int x = rand() / 1000000;
        add(table, x);
    }
    cout << find(table, 1804) << endl;
    pop(table, 1804);
    cout << find(table, 1804) << endl;
    return 0;
}