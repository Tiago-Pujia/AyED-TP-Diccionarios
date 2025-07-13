#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include "../TDA_Diccionario/dic.h"
typedef struct
{
    tLista* lista;
    Cmp cmp;
    size_t tamInfo;
}ParamCrearLista;

typedef struct
{
    int puesto;
    int palabrasEnPuesto;
    int frecAnt;
} tParamImprimirPodio;

void crearLista(tLista* lista);
void limitarLista(tLista* lista, size_t lim, Cmp cmp);
void _crearListaDesdeDicc(void* clave, void* info, void* ctx);
void crearListaDesdeDicc(tDic* dic, Cmp cmp, tLista* listaCreada, size_t tamInfo, size_t lim);
void destruirLista(tLista* lista);
int  insElemOrdenado(tLista* lista, tNodo* nodo, Cmp cmp);
int  recorrerLista(tLista* lista, Accion accion, void *param);
void _imprimirPodioPalabrasLista(void *info, void *clave, void *param);
void imprimirPodioPalabrasLista(tLista* lista);

int  cmpInfoDesc(const void* e1, const void* e2);

#endif // LISTA_H_INCLUDED
