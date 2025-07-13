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

void limitarLista(tLista* lista, size_t lim, Cmp cmp)
{
    if (!lista || !*lista || lim <= 0)
        return;

    size_t cont = 1;
    tLista* act = lista;

    // Avanza hasta el nodo n
    while (*act && cont < lim)
    {
        act = &(*act)->sig;
        cont++;
    }

    // Vamos al final del puesto, si cambia el puesto se termina
    while(*act && (*act)->sig && !cmp((*act)->info,(*act)->sig->info))
    {
        act = &(*act)->sig;
    }

    // Si hay más nodos luego de los n, eliminarlos
    if (*act && (*act)->sig)
    {
        destruirLista(&(*act)->sig);  // Elimina desde el siguiente en adelante
        (*act)->sig = NULL;
    }
}

void _crearListaDesdeDicc(void* clave, void* info, void* ctx)
{
    ParamCrearLista* param = (ParamCrearLista*)ctx;

    tNodo nodoTemp;
    nodoTemp.clave = clave;
    nodoTemp.info = info;
    nodoTemp.tamClave = strlen((char*)clave) + 1;;
    nodoTemp.tamInfo = param->tamInfo;
    nodoTemp.sig = NULL;

    insElemOrdenado(param->lista, &nodoTemp, param->cmp);
}
void crearListaDesdeDicc(tDic* dic, Cmp cmp, tLista* listaCreada, size_t tamInfo, size_t lim)
{
    ParamCrearLista param;

    *listaCreada = NULL;

    param.lista = listaCreada;
    param.cmp = cmp;
    param.tamInfo = tamInfo;

    recorrerDic(dic, _crearListaDesdeDicc, &param);
    limitarLista(listaCreada, lim, cmp);
}

int insElemOrdenado(tLista* lista, tNodo* nodo, Cmp cmp)
{
    tNodo* nuevo;

    if (!(nuevo = (tNodo*)malloc(sizeof(tNodo))))
    {
        return SIN_MEM;
    }

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

    while (*lista && cmp(nodo->info, (*lista)->info) > 0)
    {
        lista = &(*lista)->sig;
    }

    nuevo->sig = *lista;
    *lista = nuevo;

    return EXITO;
}

int recorrerLista(tLista* lista, Accion accion, void *param)
{
    if(!*lista)
        return LISTA_VACIA;

    while(*lista)
    {
        accion((*lista)->info, (*lista)->clave, param);
        lista = &(*lista)->sig;
    }

    return EXITO;
}

int  cmpInfoDesc(const void* e1, const void* e2)
{
    int va = *(int*)e1;
    int vb = *(int*)e2;

    return vb - va;
}

void _imprimirPodioPalabrasLista(void *info, void *clave, void *param)
{
    tParamImprimirPodio *ctx = (tParamImprimirPodio*)param;

    int *frecAct = (int*)info;
    char *pal = (char*)clave;

    if(ctx->frecAnt != *frecAct)
    {
        ctx->frecAnt = *frecAct;
        ctx->puesto += ctx->palabrasEnPuesto;
        ctx->palabrasEnPuesto = 0;
        printf(">> Puesto %d:\n", ctx->puesto);
    }

    printf("\t~ %s: %d\n", pal, *frecAct);
    (ctx->palabrasEnPuesto)++;
}

void imprimirPodioPalabrasLista(tLista* lista)
{
    if (!lista || !*lista)
        return;

    tParamImprimirPodio asd = { 1, 0, -1 };

    recorrerLista(lista, _imprimirPodioPalabrasLista, &asd);
}
