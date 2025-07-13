#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================


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


// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Procesa un archivo de texto l�nea por l�nea.
int procesarArch(FILE* arch, tDic* dic, tEstText* estText)
{
    rewind(arch);  // Nos aseguramos de empezar desde el inicio del archivo

    char lineaLeida[TAM_LINEA_TXT];

    // Leemos el archivo l�nea por l�nea
    while (fgets(lineaLeida, TAM_LINEA_TXT, arch))
    {
        // Procesamos la l�nea para contar palabras, espacios, signos y actualizar el diccionario
        trozarLinea(lineaLeida, dic, estText);
    }

    return EXITO;
}

/// Procesa una l�nea de texto, separa las palabras, y actualiza el diccionario y las estad�sticas.
void trozarLinea(char* linea, tDic* dic, tEstText* estText)
{
    char palabra[TAM_PAL];      // Buffer para armar una palabra
    char *pPalabra = palabra;   // Puntero m�vil dentro del buffer
    int contador = 1;           // Aparece 1 vez cuando se inserta

    do
    {
        if (*linea && ES_LETRA(*linea))  // Si es una letra v�lida, la agregamos al buffer
        {
            *pPalabra = *linea;
            pPalabra++;
        }
        else
        {
            if (pPalabra != palabra)  // Si hay contenido en el buffer, se form� una palabra
            {
                contador = 1;
                *pPalabra = '\0';  // Terminamos la palabra con null
                strToLower(palabra);  // Convertimos a min�scula y limpiamos
                if(obtenerDic(dic, palabra, strlen(palabra) + 1, &contador, sizeof(contador), comparaString) == EXITO)
                    contador++;
                inserDic(dic, palabra, strlen(palabra) + 1, &contador, sizeof(contador), comparaString);
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

/// Inicializa los contadores de estad�sticas en cero
void iniEstadisticas(tEstText* estText)
{
    estText->cantPalabras = 0;
    estText->cantEspacios = 0;
    estText->cantPuntuacion = 0;
}



