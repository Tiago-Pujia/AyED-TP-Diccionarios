#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include "../TDA_Diccionario/dic.h"

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Crea una lista enlazada a partir de los nodos de un diccionario. Recorre toda la tabla hash del diccionario
/// e inserta todos sus nodos en una lista ordenada según el comparador `cmp`. La lista resultante se almacena en `listaCreada`.
void crearListaDesdeDicc(tDic* dic, Cmp cmp, tLista* listaPodio);

/// Libera toda la memoria dinámica utilizada por una lista enlazada.
void destruirLista(tLista* lista);

/// Inserta elementos de manera ordenada en una lista
int  insElemOrdenadoDesc(tLista* lista, tNodo* nodo, Cmp cmp);

/// Imprime las palabras presentes en el top n del podio
void imprimirPodioPalabrasLista(tLista* lista, int n, Cmp cmp, Accion imprimirPalabra);

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================
/// Imprime el podio de las palabras más usadas (top n), mostrando empates
int  cmpInfo(const void* e1, const void* e2);

#endif // LISTA_H_INCLUDED
