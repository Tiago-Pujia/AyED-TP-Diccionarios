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

#define ERROR_ARCHIVO_NO_ENCONTRADO   -100
#define ERROR_EXTENSION_INVALIDA      -200
#define ERROR_ARCHIVO_VACIO           -300

// =======================================================
//                  MACROS UTILITARIAS
// =======================================================

/// Determina si un car�cter es un signo de puntuaci�n com�n
#define ES_PUNTUACION(c) ((c)=='.'||(c)==','||(c)==';'||(c)=='!'||(c)=='?'||(c)==':'||(c)=='\"'||(c)=='('||(c)==')'||(c)=='-')

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
    char masUsadas[TOP_PAL][TAM_PAL]; /// Podio: palabras m�s frecuentes en el texto
} tEstText;

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

/// Convierte una cadena de caracteres a min�sculas
void strToLower(char* str);

/// Muestra una palabra y su frecuencia (usado por recorrerDic)
void mostrarPalabra(void* clave, void* dato, void* param);

/// Funci�n de acumulaci�n: incrementa la frecuencia de una palabra
void acumularFrecuencia(void* existente, void* nuevo);

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Procesa un archivo de texto l�nea por l�nea, actualizando diccionario y estad�sticas
int procesarArch(FILE* arch, tDic* dic, tEstText* estText);

/// Analiza una l�nea, separa palabras y actualiza el diccionario y estad�sticas
void trozarLinea(char* linea, tDic* dic, tEstText* estText);

/// Inicializa todos los contadores de estad�sticas y limpia el podio
void iniEstadisticas(tEstText* estText);

/// Verifica si una palabra ya est� presente en el podio
int palabraYaEnPodio(const char* palabra, const tEstText* estText, int desdePos);

/// Genera el podio con las palabras m�s frecuentes del texto
void generarPodioPalabras(tDic* dic, int posicion, tEstText* estText);

/// Se realiza una verificaci�n y apertura sobre el archivo enviado: Que exista, que no este vacio y que sea .txt
int validarAbrirArchTxt(const char* ruta, FILE** arch);

#endif // PROCESADORTEXTO_H_INCLUDED
