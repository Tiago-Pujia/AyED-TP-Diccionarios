#include "diccionario.h"

void crear_dic(tDiccionario* pd)
{
    memset(pd->tabla, 0, TAM_DICC * sizeof(tLista)); //inicializo en 0 la tabla
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

    lista = &pd->tabla[pos]; //va hacia la lista

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

    if(!*lista)
    {
        return NO_ENCONTRADO; //directamente si esta al final, no esta la clave
    }

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

    if(!*lista)
    {
        return NO_ENCONTRADO; //directamente si esta al final, no esta la clave
    }


    aux = *lista;

    memcpy(info, aux->info, aux->tam_info > tam_info ? tam_info : aux->tam_info);

    *lista = aux->sig;

    free(aux->info);
    free(aux->clave); //liberar clave
    free(aux);

    return TODO_OK;
}
void recorrer_dic(const tDiccionario* pd, Accion accion, void* param)
{
    tLista* pl;
    //Recorro las Listas
    for(int i = 0; i < TAM_DICC ; i++)
    {
        pl = (tLista*)&pd->tabla[i];
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
        pl = (tLista*)&pd->tabla[i];
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


int procesarArchivo(const char*archivo, tDiccionario*dic)
{
    FILE *pf = fopen(archivo,"rt");

    if(!pf)
    {
        printf("No se pudo abrir el archivo");
        return ERROR;
    }
    tProcesadorTexto aux;
    aux.cantEspacios=0;
    aux.cantPalabras=0;
    aux.cantPuntuacion=0;

    char linea[TAM_LINEA];

    while(fgets(linea,TAM_LINEA, pf))
    {
        trozar(linea,dic, &aux);
    }

    fclose(pf);

    return TODO_OK;

}

void trozar(char *linea, tDiccionario*dic, tProcesadorTexto*aux)
{
    char palabra[TAM_PAL];
    char* pPal = palabra;
    int cantPal = 1; //cada vez q se agrega una palabra, arranca en 1

    while (*linea)
    {
        if (ES_LETRA(*linea))
        {
            *pPal = *linea;
            pPal++;
        }
        else
        {
            if (pPal != palabra)
            {
                *pPal = '\0';
                normalizar(palabra);

                poner_dic(dic, palabra, &cantPal, sizeof(cantPal), cmp_pal, acum);
                aux->cantPalabras++;
                pPal = palabra; //reinicio para la proxima palabra
            }

            if (*linea == ' ' || *linea == '\t')
                aux->cantEspacios++;
            else if (ES_PUNTUACION(*linea))
                aux->cantPuntuacion++;
        }

        linea++;
    }

    if (pPal != palabra)
    {
        *pPal = '\0';
        normalizar(palabra);
        poner_dic(dic, palabra, &cantPal, sizeof(cantPal), cmp_pal, acum);
        aux->cantPalabras++;
    }

}

int cmp_pal(const void* a, const void* b)
{
    return strcmp((const char*)a, (const char*)b);
}


void acum(void* existente, const void* nuevo)
{
    (*(int*)existente)++;
}


void normalizar(char * cadena)
{
    char *pLec = cadena;
    char *pEsc = cadena;

    while(*pLec)
    {
        *pEsc = mi_toLower(*pLec);
        pEsc++;
        pLec++;
    }
    *pEsc = '\0';
}
