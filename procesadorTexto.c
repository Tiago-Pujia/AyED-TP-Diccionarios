#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

void acumularFrecuencia(void* existente, void* nuevo)
{
    (*(int*)existente)++;
}


void mostrarPalabra(void* clave, void* dato, void* param)
{
    printf("~ %s: %d\n", (char*)clave, *(int*)dato);
}


void strToLower(char* str)
{
    while (*str)
    {
        *str = MI_TOLOWER(*str);  // Convierte el carácter si es letra mayúscula
        str++;                    // Avanza al siguiente carácter
    }
}


/// Procesa un archivo de texto línea por línea.

int procesarArch(FILE* arch, tDic* dic, tEstText* estText)
{
    rewind(arch);  // Nos aseguramos de empezar desde el inicio del archivo

    char lineaLeida[TAM_LINEA_TXT];

    // Leemos el archivo línea por línea
    while (fgets(lineaLeida, TAM_LINEA_TXT, arch))
    {
        // Procesamos la línea para contar palabras, espacios, signos y actualizar el diccionario
        trozarLinea(lineaLeida, dic, estText);
    }

    return EXITO;
}

/// Procesa una línea de texto, separa las palabras, y actualiza el diccionario y las estadísticas.

void trozarLinea(char* linea, tDic* dic, tEstText* estText)
{
    char palabra[TAM_PAL];      // Buffer para armar una palabra
    char *pPalabra = palabra;   // Puntero móvil dentro del buffer
    int contador = 1;           // Aparece 1 vez cuando se inserta

    do
    {
        if (*linea && ES_LETRA(*linea))  // Si es una letra válida, la agregamos al buffer
        {
            *pPalabra = *linea;
            pPalabra++;
        }
        else
        {
            if (pPalabra != palabra)  // Si hay contenido en el buffer, se formó una palabra
            {
                *pPalabra = '\0';  // Terminamos la palabra con null
                strToLower(palabra);  // Convertimos a minúscula y limpiamos
                inserDic(dic, palabra, strlen(palabra) + 1, &contador, sizeof(contador), comparaString, acumularFrecuencia);
                pPalabra = palabra;  // Reiniciamos el buffer para la siguiente palabra
                estText->cantPalabras++;
            }

            // Contamos espacios o tabulaciones
            if (*linea == ' ' || *linea == '\t')
                estText->cantEspacios++;
            else if (ES_PUNTUACION(*linea))
                estText->cantPuntuacion++;
        }
    }
    while (*linea++);
}

/// Inicializa los contadores de estadísticas en cero
void iniEstadisticas(tEstText* estText)
{
    estText->cantPalabras = 0;
    estText->cantEspacios = 0;
    estText->cantPuntuacion = 0;
}

void generarPodioPalabras(tDic* dic, int posicion, tEstText* estText, Cmp cmp, tDic*dicPodio, Cmp cmpEnt)
{
    tNodo* palabraMasUsada = NULL, *iguales;
    int posicionYaUsada = 1, cantEmpate;

    while(posicionYaUsada <= posicion)
    {
        palabraMasUsada = buscarMasUsada(dic, cmp, dicPodio);

        if(palabraMasUsada)
        {
            cantEmpate=0;

            inserDic(dicPodio, palabraMasUsada->clave, strlen(palabraMasUsada->clave) + 1, &posicionYaUsada, sizeof(posicionYaUsada), comparaString, NULL);
            iguales = buscarIgual(dic, palabraMasUsada, cmpEnt, dicPodio, cmp);

            while(iguales)
            {
                printf("HOLA\n");
                inserDic(dicPodio, iguales->clave, strlen(iguales->clave) + 1, &posicionYaUsada, sizeof(posicionYaUsada), comparaString, NULL);
                cantEmpate++;
                iguales = buscarIgual(dic, iguales, cmpEnt,dicPodio, cmp);

            }


            if(cantEmpate > 0)
                posicionYaUsada += cantEmpate;
            else
                posicionYaUsada++;
        }
        posicionYaUsada++;

    }
}


tNodo* buscarIgual(tDic*dic, tNodo*dato,Cmp cmp, tDic*dicPodio, Cmp cmpStr)
{

    for(size_t i = 0; i < TAM_DIC; i++)
    {
        tLista lista = dic->tabla[i];

        while(lista)
        {
            if((cmp((lista)->info,dato->info)==0) && !(estaEnDic(dicPodio, lista->clave, cmpStr)))
            {
                return lista;
            }

            lista = lista->sig;
        }
    }

    return NULL;
}

tNodo* buscarMasUsada(tDic* dic, Cmp cmp, tDic* dicPodio)
{
    tNodo* palabraMasUsada = NULL;
    for (size_t i = 0; i < TAM_DIC; i++)
    {
        tLista lista = dic->tabla[i];
        while (lista)
        {
            // Verificamos que no esté en el podio
            if (!estaEnDic(dicPodio, lista->clave, cmp))
            {
                // Buscamos la palabra con mayor frecuencia (mayor info)
                if(!palabraMasUsada ||
                        (*(int*)lista->info > *(int*)palabraMasUsada->info))
                {
                    palabraMasUsada = lista;
                }
            }
            lista = lista->sig;
        }
    }
    return palabraMasUsada;
}
/// Verifica si una palabra ya está en el podio de palabras más utilizadas.

int estaEnDic(tDic* dic, const void* clave, Cmp cmp)
{
    for(size_t i = 0; i < TAM_DIC; i++)
    {
        tLista lista = dic->tabla[i];

        while(lista)
        {
            if((cmp((lista)->clave,clave)==0))
            {
                return 1;
            }

            lista = lista->sig;
        }
    }
    return 0;
}

int comparaString(const void *a, const void *b)
{
    return strcmp((char*)a, (char*)b);
}
int comparaEntero(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}




//no muy eficiente
void mostrarPodioPorPosicion(tDic* dicPodio)
{
    int puestoActual = 1;

    while(puestoActual <= TOP_PAL)
    {
        for (int i = 0; i < TAM_DIC; i++)
        {
            tLista lista = dicPodio->tabla[i];

            while (lista)
            {
                int puestoPalabra = *(int*)lista->info;

                if (puestoPalabra == puestoActual)
                {
                    printf("%d. %s\n", puestoActual, (char*)lista->clave);
                }

                lista = lista->sig;
            }
        }

        puestoActual++;
    }
}


