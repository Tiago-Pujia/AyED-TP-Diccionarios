#include "lista.h"

/// Libera la memoria ocupada por toda la lista enlazada
void destruirLista(tLista* lista)
{
    if (*lista == NULL)
        return;

    tNodo* nodoElim;
    while (*lista)
    {
        nodoElim = *lista;
        *lista = nodoElim->sig;

        // Liberar memoria del nodo
        free(nodoElim->info);   // Libera contenido dinámico copiado
        free(nodoElim->clave);  // Libera clave copiada
        free(nodoElim);         // Libera el nodo en sí
    }
}

/// Crea una nueva lista enlazada a partir del contenido de un diccionario (tabla hash)
/// Copia los nodos de cada posición de la tabla, ordenándolos con la función cmp
void crearListaDesdeDicc(tDic* dic, Cmp cmp, tLista* listaPodio)
{
    tLista listaDicc;
    *listaPodio= NULL;

    for (int i = 0; i < TAM_DIC; i++)
    {
        listaDicc = dic->tabla[i];

        // Recorre cada nodo de la lista de esta posición
        while (listaDicc)
        {
            insElemOrdenadoDesc(listaPodio, listaDicc, cmp);  // Inserta ordenado
            listaDicc = listaDicc->sig;
        }
    }
}

/// Inserta un nuevo nodo en la lista en forma descendente según cmp
/// Clona el nodo original (clave e info) y lo inserta en la nueva lista
int insElemOrdenadoDesc(tLista* lista, tNodo* nodo, Cmp cmp)
{
    tNodo* nuevo;

    // Reservamos memoria para el nuevo nodo
    if (!(nuevo = (tNodo*)malloc(sizeof(tNodo))))
        return SIN_MEM;

    // Reservamos memoria para la info
    if (!(nuevo->info = malloc(nodo->tamInfo)))
    {
        free(nuevo);
        return SIN_MEM;
    }

    // Reservamos memoria para la clave
    if (!(nuevo->clave = malloc(nodo->tamClave)))
    {
        free(nuevo->info);
        free(nuevo);
        return SIN_MEM;
    }

    // Copiamos los datos desde el nodo original
    memcpy(nuevo->info, nodo->info, nodo->tamInfo);
    memcpy(nuevo->clave, nodo->clave, nodo->tamClave);
    nuevo->tamInfo = nodo->tamInfo;
    nuevo->tamClave = nodo->tamClave;

    // Buscamos la posición adecuada (orden descendente)
    while (*lista && cmp(nodo->info, (*lista)->info) < 0)
        lista = &(*lista)->sig;

    // Insertamos el nuevo nodo en la lista
    nuevo->sig = *lista;
    *lista = nuevo;

    return EXITO;
}

/// Imprime el podio de las palabras más usadas (top n), mostrando empates
void imprimirPodioPalabrasLista(tLista* lista, int n, Cmp cmp, Accion imprimirPalabra)
{
    if (!lista || !*lista)
        return;

    int totalMostradas = 0;
    int puesto = 1;

    while (*lista && totalMostradas < n)
    {
        printf(">> Puesto %d:\n", puesto);

        void* valorActual = (*lista)->info;
        int palabrasEnPuesto = 0;

        // Imprime todas las palabras empatadas en el mismo puesto
        do
        {
            imprimirPalabra(*lista, NULL, NULL);
            totalMostradas++;
            palabrasEnPuesto++;
            lista = &(*lista)->sig;
        }
        while (*lista && cmp(valorActual, (*lista)->info) == 0);

        puesto += palabrasEnPuesto;

        // Detiene si ya imprimió n o más palabras
        if (totalMostradas >= n)
            break;
    }
}


/// Compara dos elementos por su información
int cmpInfo(const void* e1, const void* e2)
{
    int elem1 = *(int*)e1;
    int elem2 = *(int*)e2;
    return elem1 - elem2;
}
