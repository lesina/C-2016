#include <cstdlib>
#include <iostream>
#include <cmath>
#include "doubleList.h"

using namespace std;

void add( List *list, int x)
{
    Node *temp = new Node(); // Выделение памяти под новый элемент структуры
    temp->next = NULL;       // Указываем, что изначально по следующему адресу пусто
    temp->x = x;             // Записываем значение в структуру

    if ( list->head != NULL ) // Если список не пуст
    {
        temp->prev = list->tail; // Указываем адрес на предыдущий элемент в соотв. поле
        list->tail->next = temp; // Указываем адрес следующего за хвостом элемента
        list->tail = temp;       //Меняем адрес хвоста
    }
    else //Если список пустой
    {
        temp->prev = NULL; // Предыдущий элемент указывает в пустоту
        list->head = list->tail = temp;    // Голова=Хвост=тот элемент, что сейчас добавили
    }
}

void print( List * list )
{
    Node * temp = list->head;  // Временно указываем на адрес первого элемента
    while( temp != NULL )      // Пока не встретим пустое значение
    {
        cout << temp->x << endl; //Выводим значение на экран
        temp = temp->next;     //Смена адреса на адрес следующего элемента
    }
    cout<<"\n";
}

int find (List *list, int check_x){
    Node * temp = list->head;
    int i = -1; //returns -1 if element not found
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

void pop(List *list, int index){
    Node * temp = list->head;
    for(int i = 2; i <= index; i++){
        if (temp->next != NULL)
            temp = temp->next;
        else
            break;
    }
    if (temp != list->tail) {
        Node *new_tmp = temp->next->next;
        delete temp->next;
        temp->next = new_tmp;
        new_tmp->prev = temp;
    } else if (temp == list->head) {
        list->head = temp->next;
        if (temp->next != NULL)
            temp->next->prev = NULL;
        delete temp;
    } else if (temp == list->tail) {
        list->tail = temp->prev;
        temp->prev->next = NULL;
        delete temp;
    }
}