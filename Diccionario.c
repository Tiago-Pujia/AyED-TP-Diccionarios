#include "diccionario.h"

void crear_dic(tDiccionario* pd)
{
    memset(pd->tabla, 0, TAM_DICC * sizeof(tLista));
}

int poner_dic(tDiccionario* pd, void* clave, size_t tam_clave, const void* info, size_t tam_info, Cmp cmp, Actualizar actualizar)
{
    size_t pos = _hasheo_dic(clave, tam_clave);
    tNodo* nue;
    tLista* lista;

    //Reservo memoria Nodo (SI SE CAMBIO GUARDA QUE HAY UN FREE ABAJO)
    nue = (tNodo*) malloc(sizeof(tNodo));
    if(!nue)
        return SIN_MEM;
    nue->info = malloc(tam_info);
    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }
    nue->clave = malloc(tam_clave);
    if(!nue->clave)
    {
        free(nue->info);
        free(nue);
        return SIN_MEM;
    }

    lista = &pd->tabla[pos];

    while(*lista)
    {
        //IMPLEMENTAR DUPLICADOS
        if(cmp((*lista)->clave, clave) == 0)
        {
            free(nue->clave);
            free(nue->info);
            free(nue);
            actualizar((*lista)->info, (void*)info);
            return TODO_OK;
        }
        lista = &(*lista)->sig;
    }

    memcpy(nue->info, info, tam_info);
    memcpy(nue->clave, clave, tam_clave);
    nue->tam_info = tam_info;
    nue->sig = *lista;
    *lista = nue;

    return TODO_OK;
}
int obtener_dic(tDiccionario* pd, void* clave, size_t tam_clave, void* info, size_t tam_info, Cmp cmp)
{
    size_t pos = _hasheo_dic(clave, tam_clave);
    tLista* lista;
    tNodo* aux;

    lista = &pd->tabla[pos];

    if(!*lista)
        return DICC_VACIO;

    while(*lista && cmp((*lista)->clave, clave) != 0)
    {
        lista = &(*lista)->sig;
    }

    if(cmp((*lista)->clave, clave) != 0)
        return NO_ENCONTRADO;

    aux = *lista;
    memcpy(info, aux->info, aux->tam_info > tam_info ? tam_info : aux->tam_info);
    return TODO_OK;


}
int sacar_dic(tDiccionario* pd, void* clave, size_t tam_clave, void* info, size_t tam_info, Cmp cmp)
{
        size_t pos = _hasheo_dic(clave, tam_clave);
    tLista* lista;
    tNodo* aux;

    lista = &pd->tabla[pos];

    if(!*lista)
        return DICC_VACIO;

    while(*lista && cmp((*lista)->clave, clave) != 0)
    {
        lista = &(*lista)->sig;
    }

    if(cmp((*lista)->clave, clave) != 0)
        return NO_ENCONTRADO;

    aux = *lista;

    memcpy(info, aux->info, aux->tam_info > tam_info ? tam_info : aux->tam_info);

    *lista = aux->sig;

    free(aux->info);
    free(aux);

    return TODO_OK;
}
void recorrer_dic(const tDiccionario* pd, Accion accion, void* param)
{
    tLista* pl;
    //Recorro las Listas
    for(int i = 0; i < TAM_DICC ; i++)
    {
        pl = (tLista*) &pd->tabla[i];
        //Recorro una de las listas
        while(*pl)
        {
            accion((*pl)->info, param);
            pl = &(*pl)->sig;
        }
    }
}
void vaciar_dic(const tDiccionario* pd)
{
    tLista* pl;
    tNodo* aux;
    //Recorro las Listas
    for(int i = 0; i < TAM_DICC ; i++)
    {
        pl = (tLista*) &pd->tabla[i];
        //Recorro una de las listas
        while(*pl)
        {
            //Borro el nodo
            aux = *pl;
            *pl = aux->sig;
            free(aux->info);
            free(aux);
        }
    }
}

size_t _hasheo_dic(void* clave, size_t len)
{
    unsigned long hash = CONST_HASHEO;
    char* data = (char*) clave;
    int i;
    for(i = 0; i < len ; i++)
        hash = hash * 37 + *(data+i);
    return hash % TAM_DICC;
}
