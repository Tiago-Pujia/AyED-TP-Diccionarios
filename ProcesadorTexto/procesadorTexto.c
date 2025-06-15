#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

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
        *str = MI_TOLOWER(*str);
        str++;
    }
}

int comparaString(const void *a, const void *b)
{
    return strcmp((char*)a, (char*)b);
}

int comparaEntero(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

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
