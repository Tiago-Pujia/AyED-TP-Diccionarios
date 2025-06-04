#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

/// Incrementa en uno el valor existente asociado a una palabra ya presente en el diccionario.
///
/// @param existente  Puntero al valor actual (int) ya almacenado en el diccionario
/// @param nuevo      Puntero al valor nuevo (no se utiliza en este caso)
void acumularFrecuencia(void* existente, void* nuevo)
{
    (*(int*)existente)++;
}

/// Imprime una palabra y su cantidad de apariciones.
///
///
/// @param clave   Puntero a la clave (palabra)
/// @param dato    Puntero al valor (cantidad de apariciones)
/// @param param   Par�metro extra no utilizado (puede ser NULL)
void mostrarPalabra(void* clave, void* dato, void* param)
{
    printf("~ %s: %d\n", (char*)clave, *(int*)dato);
}

/// Convierte todos los caracteres de una cadena a min�sculas.
///
/// Esta funci�n asegura que palabras como "HolA" y "hola"
/// sean tratadas como iguales al insertarlas en el diccionario.
///
/// @param str Puntero de la cadena
void strToLower(char* str)
{
    while (*str)
    {
        *str = MI_TOLOWER(*str);  // Convierte el car�cter si es letra may�scula
        str++;                    // Avanza al siguiente car�cter
    }
}

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Procesa un archivo de texto l�nea por l�nea.
/// Cada l�nea se separa en palabras y se actualiza el diccionario y las estad�sticas generales.
///
/// @param arch    Archivo de texto abierto en modo lectura
/// @param dic     Diccionario donde se insertan/actualizan las palabras encontradas
/// @param estText Puntero a la estructura donde se acumulan estad�sticas del texto
///
/// @return EXITO si se proces� exitosamente
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
///
/// @param linea   L�nea de texto a analizar
/// @param dic     Diccionario donde se insertan/actualizan las palabras encontradas
/// @param estText Puntero a la estructura donde se acumulan estad�sticas del texto
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
                *pPalabra = '\0';  // Terminamos la palabra con null
                strToLower(palabra);  // Convertimos a min�scula y limpiamos
                inserDic(dic, palabra, strlen(palabra) + 1, &contador, sizeof(contador), strcmp, acumularFrecuencia);
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

/// Inicializa los contadores de estad�sticas en cero y
/// limpia la matriz de palabras m�s usadas.
///
/// Esta funci�n debe llamarse antes de comenzar a procesar el texto.
///
/// @param estText Puntero a la estructura donde se acumulan estad�sticas del texto
void iniEstadisticas(tEstText* estText)
{
    estText->cantPalabras = 0;
    estText->cantEspacios = 0;
    estText->cantPuntuacion = 0;

    // Inicializa cada palabra del podio con cadena vac�a
    for (size_t i = 0; i < TOP_PAL; i++)
    {
        estText->masUsadas[i][0] = '\0'; // Primer car�cter = fin de cadena
    }
}

/// Genera el podio de las palabras m�s frecuentes en el texto.
/// La palabra m�s usada quedar� en la �ltima posici�n del arreglo `masUsadas`.
///
/// @param dic        Diccionario con palabras y sus frecuencias
/// @param posicion   Posici�n actual que se est� llenando en el podio (desde TOP_PAL hasta 1)
/// @param estText    Estructura de estad�sticas donde se guarda el podio
void generarPodioPalabras(tDic* dic, int posicion, tEstText* estText)
{
    tNodo* palabraMasUsada = NULL;

    if (posicion > 0)
    {
        // Recorremos cada lista de la tabla hash
        for (size_t i = 0; i < TAM_DIC; i++)
        {
            tLista lista = dic->tabla[i];

            // Recorremos los nodos de la lista
            while (lista)
            {
                // Si a�n no tenemos una palabra m�s usada, o encontramos una con mayor frecuencia
                // y no est� ya en el podio
                if (
                    (!palabraMasUsada || strcmp(lista->info, palabraMasUsada->info) > 0) &&
                    !palabraYaEnPodio((char*)lista->clave, estText, posicion)
                )
                {
                    palabraMasUsada = lista;
                }

                lista = lista->sig;
            }
        }

        if (palabraMasUsada)
        {
            // Guardamos la clave (palabra) en la posici�n correspondiente del podio
            strncpy(estText->masUsadas[posicion - 1], (char*)palabraMasUsada->clave, TAM_PAL);
            estText->masUsadas[posicion - 1][TAM_PAL - 1] = '\0';  // Aseguramos fin de cadena

            // Llamado recursivo para completar el resto del podio
            generarPodioPalabras(dic, posicion - 1, estText);
        }
    }
}

/// Verifica si una palabra ya est� en el podio de palabras m�s utilizadas.
///
/// @param palabra   Palabra a verificar
/// @param estText   Estructura que contiene el arreglo de palabras m�s usadas
/// @param desdePos  �ndice inicial desde donde buscar en el podio
///
/// @return 1 si la palabra ya est� en el podio, 0 si no est�
int palabraYaEnPodio(const char* palabra, const tEstText* estText, int desdePos)
{
    for (size_t i = desdePos; i < TOP_PAL; i++)
    {
        // Si la palabra coincide con alguna ya en el podio, est� repetida
        if (strcmp(estText->masUsadas[i], palabra) == 0)
        {
            return 1;
        }
    }
    return 0;  // No se encontr� en el podio
}
