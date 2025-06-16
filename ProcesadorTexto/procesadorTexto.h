#ifndef PROCESADORTEXTO_H_INCLUDED
#define PROCESADORTEXTO_H_INCLUDED

#include <stdio.h>
#include <string.h>

// =======================================================
//                  CONSTANTES GENERALES
// =======================================================

#define TAM_LINEA_TXT 1000     /// Tama�o m�ximo de una l�nea de texto a leer del archivo
#define TAM_PAL 100            /// Tama�o m�ximo permitido para una palabra
#define TOP_PAL 5              /// Cantidad de palabras a incluir en el podio de m�s usadas

// =======================================================
//                  MACROS UTILITARIAS
// =======================================================

/// Determina si un car�cter es un signo de puntuaci�n com�n
#define ES_PUNTUACION(c) ( ((c) >= 33 && (c)<=47) || ((c) >= 58 && (c)<=63) || \
                          ((c) >= 91 && (c)<=96) || \
                          ((c) >= 123 && (c)<=126))

/// Determina si un car�cter es una letra (may�scula o min�scula)
#define ES_LETRA(c) (((c)>= 'a' && (c)<= 'z') || ((c)>= 'A' && (c) <= 'Z'))

/// Convierte una letra may�scula a min�scula (sin usar <ctype.h>)
#define MI_TOLOWER(X) (((X)>= 'A' && (X) <= 'Z') ? ((X)+32) : (X))

// =======================================================
//                  TIPOS DE DATOS
// =======================================================

/// Estructura para almacenar estad�sticas durante el procesamiento del texto
typedef struct
{
    int cantPalabras;                 /// Cantidad total de palabras encontradas
    int cantEspacios;                 /// Cantidad total de espacios y tabulaciones
    int cantPuntuacion;               /// Cantidad total de signos de puntuaci�n
}tEstText;

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

/// Convierte una cadena de caracteres a min�sculas
void strToLower(char* str);


/// Funci�n de acumulaci�n: incrementa la frecuencia de una palabra
void acumularFrecuencia(void* existente, void* nuevo);

int comparaString(const void *a, const void *b);


// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Procesa un archivo de texto l�nea por l�nea, actualizando diccionario y estad�sticas
int procesarArch(FILE* arch, tDic* dic, tEstText* estText);

/// Analiza una l�nea, separa palabras y actualiza el diccionario y estad�sticas
void trozarLinea(char* linea, tDic* dic, tEstText* estText);

/// Inicializa todos los contadores de estad�sticas y limpia el podio
void iniEstadisticas(tEstText* estText);


#endif // PROCESADORTEXTO_H_INCLUDED
