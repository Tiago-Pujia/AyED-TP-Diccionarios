#include "Diccionario.h"
#include "procesadorTexto.h"

int procesarArchivo(FILE*pf, tDiccionario*dic,tProcesadorTexto* aux)
{
    rewind(pf);

    char linea[TAM_LINEA_TXT];

    while(fgets(linea,TAM_LINEA_TXT, pf))
    {
        trozar(linea,dic, aux);
    }
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
                poner_dic(dic, palabra, (strlen(palabra)+1), &cantPal, sizeof(cantPal), cmp_pal, acum);
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

    if(pPal != palabra)
    {
        *pPal = '\0';
        normalizar(palabra);
        poner_dic(dic, palabra, (strlen(palabra)+1), &cantPal, sizeof(cantPal), cmp_pal, acum);
        aux->cantPalabras++;
    }

}

int cmp_pal(const void* a, const void* b)
{
    return strcmp((const char*)a, (const char*)b);
}


void acum(void* existente, void* nuevo)
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

void iniciarEnCero(tProcesadorTexto*aux)
{
    aux->cantEspacios=0;
    aux->cantPalabras=0;
    aux->cantPuntuacion=0;
}


void mostrar_estadisticas(const tProcesadorTexto* aux, const tDiccionario* dic)
{
    printf("\n===== ESTADISTICAS DEL TEXTO =====\n");
    printf("Cantidad total de palabras: %d\n", aux->cantPalabras);
    printf("Cantidad de espacios y tabulaciones: %d\n", aux->cantEspacios);
    printf("Cantidad de signos de puntuacion: %d\n", aux->cantPuntuacion);

    printf("\n===== TOP 5 PALABRAS MAS USADAS =====\n");
    for(int i = TOP_PAL - 1; i >= 0; i--)
    {
        if(aux->masUsadas[i][0] != '\0')
            printf("%d. %s\n", TOP_PAL - i, aux->masUsadas[i]);
    }
    printf("\n===== PALABRAS Y FRECUENCIAS =====\n");
    recorrer_dic(dic, mostrar_palabra, NULL);

}


void mostrar_palabra(void* clave, void* dato, void* param)
{
    printf("%s: %d\n", (char*)clave, *(int*)dato);
}


void podioPalabras(tDiccionario*dic, int n, tProcesadorTexto*aux)
{
    tLista masUsada = NULL;

    if(n>0)
    {
        for(int i=0; i<TAM_DICC; i++)
        {
            tLista pl = dic->tabla[i];
            while(pl)
            {
                if((!masUsada || cmp_pal(pl->info, masUsada->info) > 0) &&
                        !ya_en_podio((char*)pl->clave,aux,n))
                {
                    masUsada = pl;
                }
                pl = pl->sig;
            }
        }

        if(masUsada)
        {
            strncpy(aux->masUsadas[n-1], (char*)masUsada->clave,TAM_PAL);
            aux->masUsadas[n-1][TAM_PAL-1] = '\0';

            podioPalabras(dic,n-1,aux);
        }
    }
}


int ya_en_podio(const char *palabra, tProcesadorTexto* aux, int hasta)
{
    for(int i = hasta; i < TOP_PAL; i++)
    {
        if(strcmp(aux->masUsadas[i], palabra) == 0)
            return 1; // ya esta
    }
    return 0; //no esta
}
