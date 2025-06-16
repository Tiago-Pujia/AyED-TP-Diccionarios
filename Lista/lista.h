#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include "../TDA_Diccionario/dic.h"

void crearLista(tLista* lista);
void crearListaDesdeDicc(tDic* dic, Cmp cmp, tLista* listaCreada);
void destruirLista(tLista* lista);
int  insElemOrdenadoDesc(tLista* lista, tNodo* nodo, Cmp cmp);
int  recorrerLista(tLista* lista, Accion accion);
void imprimirPodioPalabrasLista(tLista* lista, int n, Cmp cmp, Accion accion, void*param);

int  cmpInfo(const void* e1, const void* e2);
typedef void (*Accion)(void*, void*, void*);

#endif // LISTA_H_INCLUDED
