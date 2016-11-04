#ifndef HASH_TABLE_DOUBLELIST_H
#define HASH_TABLE_DOUBLELIST_H

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

void add( List *list, int x );
void print( List * list );
void print(List * list, int number);
int find (List *list, int check_x);
void pop(List *list, int index);

#endif