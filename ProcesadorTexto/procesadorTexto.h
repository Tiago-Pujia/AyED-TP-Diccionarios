#ifndef PROCESADORTEXTO_H_INCLUDED
#define PROCESADORTEXTO_H_INCLUDED

#include <stdio.h>
#include <string.h>

// =======================================================
//                  CONSTANTES GENERALES
// =======================================================

#define TAM_LINEA_TXT 1000     /// Tamaño máximo de una línea de texto a leer del archivo
#define TAM_PAL 100            /// Tamaño máximo permitido para una palabra
#define TOP_PAL 5              /// Cantidad de palabras a incluir en el podio de más usadas

// =======================================================
//                  MACROS UTILITARIAS
// =======================================================

/// Determina si un carácter es un signo de puntuación común
#define ES_PUNTUACION(c) ( ((c) >= 33 && (c)<=47) || ((c) >= 58 && (c)<=63) || \
                          ((c) >= 91 && (c)<=96) || \
                          ((c) >= 123 && (c)<=126))

/// Determina si un carácter es una letra (mayúscula o minúscula)
#define ES_LETRA(c) (((c)>= 'a' && (c)<= 'z') || ((c)>= 'A' && (c) <= 'Z'))

/// Convierte una letra mayúscula a minúscula (sin usar <ctype.h>)
#define MI_TOLOWER(X) (((X)>= 'A' && (X) <= 'Z') ? ((X)+32) : (X))

// =======================================================
//                  TIPOS DE DATOS
// =======================================================

/// Estructura para almacenar estadísticas durante el procesamiento del texto
typedef struct
{
    int cantPalabras;                 /// Cantidad total de palabras encontradas
    int cantEspacios;                 /// Cantidad total de espacios y tabulaciones
    int cantPuntuacion;               /// Cantidad total de signos de puntuación
}tEstText;

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

/// Convierte una cadena de caracteres a minúsculas
void strToLower(char* str);


/// Función de acumulación: incrementa la frecuencia de una palabra
void acumularFrecuencia(void* existente, void* nuevo);

int comparaString(const void *a, const void *b);


// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Procesa un archivo de texto línea por línea, actualizando diccionario y estadísticas
int procesarArch(FILE* arch, tDic* dic, tEstText* estText);

/// Analiza una línea, separa palabras y actualiza el diccionario y estadísticas
void trozarLinea(char* linea, tDic* dic, tEstText* estText);

/// Inicializa todos los contadores de estadísticas y limpia el podio
void iniEstadisticas(tEstText* estText);


#endif // PROCESADORTEXTO_H_INCLUDED
