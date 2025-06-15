#ifndef INTERFAZUSUARIO_H_INCLUDED
#define INTERFAZUSUARIO_H_INCLUDED

#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"
#include "../Lista/lista.h"

#include <stdio.h>
#include <string.h>

// =======================================================
//                  CONSTANTES GENERALES
// =======================================================

#define TAM_RUTA 256    /// Tama�o m�ximo para la ruta del archivo ingresado
#define TAM_LINEA 256   /// Tama�o m�ximo para l�neas auxiliares (uso general)

#define ERROR_ARCHIVO_NO_ENCONTRADO   -100
#define ERROR_EXTENSION_INVALIDA      -200
#define ERROR_ARCHIVO_VACIO           -300
#define ARCHIVO_VALIDO 0

// =======================================================
//                  MENSAJES
// =======================================================

#define MSJ_CONTINUAR "\nDesea analizar otro archivo? Ingrese 0 para salir o cualquier letra para continuar: "

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Solicita al usuario una ruta de archivo, valida su contenido,
/// procesa el texto y muestra los resultados.
void iniciarAnalisisTexto();

/// Muestra las instrucciones y descripci�n general del programa.
void mostrarInstrucciones();

void mostrarPodio(const tDic *dicPodio, const tDic *dic, Cmp cmp);

/// Muestra las estad�sticas recolectadas y todas las palabras registradas
void mostrarEstadisticas(const tEstText* estText, const tDic* dic, tLista* listaPodio);

/// Se realiza una verificaci�n y apertura sobre el archivo enviado: Que exista, que no este vacio y que sea .txt
int validarArchivoTxt(const char* ruta, FILE** arch);

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

/// Limpia la consola, dependiendo del sistema operativo.
void limpiarConsola();

/// Lee una l�nea de texto desde la entrada est�ndar y elimina el salto de l�nea final.
void leerTexto(char *texto, size_t longitud);

void imprimirPalabra(void* elem, void* param, void* v);

#endif // INTERFAZUSUARIO_H_INCLUDED
