#include "lista.h"

void crearLista(tLista* lista)
{
    *lista = NULL;
}

void destruirLista(tLista* lista)
{
    if(*lista == NULL)
        return;

    tNodo* nodoElim;
    while(*lista)
    {
        nodoElim = *lista;
        *lista = nodoElim->sig;
        free(nodoElim->info);
        free(nodoElim->clave);
        free(nodoElim);
    }
}

void crearListaDesdeDicc(tDic* dic, Cmp cmp, tLista* listaCreada)
{
    tLista listaDicc;
    *listaCreada = NULL;

    for (int i = 0; i < TAM_DIC; i++)
    {
        listaDicc = dic->tabla[i];

        while (listaDicc)
        {
            insElemOrdenadoDesc(listaCreada, listaDicc, cmp);
            listaDicc = listaDicc->sig;
        }
    }
}

int insElemOrdenadoDesc(tLista* lista, tNodo* nodo, Cmp cmp)
{
    tNodo* nuevo;

    if (!(nuevo = (tNodo*)malloc(sizeof(tNodo))))
        return SIN_MEM;

    if (!(nuevo->info = malloc(nodo->tamInfo)))
    {
        free(nuevo);
        return SIN_MEM;
    }

    if (!(nuevo->clave = malloc(nodo->tamClave)))
    {
        free(nuevo->info);
        free(nuevo);
        return SIN_MEM;
    }

    memcpy(nuevo->info, nodo->info, nodo->tamInfo);
    memcpy(nuevo->clave, nodo->clave, nodo->tamClave);
    nuevo->tamInfo = nodo->tamInfo;
    nuevo->tamClave = nodo->tamClave;

    while (*lista && cmp(nodo->info, (*lista)->info) < 0)
        lista = &(*lista)->sig;

    nuevo->sig = *lista;
    *lista = nuevo;

    return EXITO;
}

int recorrerLista(tLista* lista, Accion accion)
{
    if(!*lista)
        return LISTA_VACIA;

    while(*lista)
    {
        accion(*lista, NULL, NULL);
        lista = &(*lista)->sig;
    }

    return EXITO;
}

int  cmpInfo(const void* e1, const void* e2)
{
    int va = *(int*)e1;
    int vb = *(int*)e2;

    return va - vb;
}

void imprimirPodioPalabrasLista(tLista* lista, int n, Cmp cmp, Accion accion, void*param)
{
    if (!lista || !*lista)
        return;

    int totalMostradas = 0;
    int puesto = 1;

    while(*lista && totalMostradas < n)
    {
        printf(">> Puesto %d:\n", puesto);

        void* valorActual = (*lista)->info;
        int palabrasEnPuesto = 0;

        // Imprimir todas las palabras empatadas en este puesto
        do
        {
            accion((*lista)->clave, (*lista)->info, param); //se imprime la palabra
            totalMostradas++;
            palabrasEnPuesto++;
            lista = &(*lista)->sig;
        }
        while(*lista && cmp(valorActual, (*lista)->info) == 0);

        puesto += palabrasEnPuesto; //para avanzar x cantidad de palabras que ocuparon un mismo puesto

        // Si ya tenemos n o más palabras, cortamos
        if(totalMostradas >= n)
            break;
    }
}

