#ifndef PRUEBAS_H_INCLUDED
#define PRUEBAS_H_INCLUDED

#include "stdio.h"

#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"
#include "../Interfaz/interfaz.h"

// =======================================================
//                  CONSTANTES GENERALES
// =======================================================

#define CANT_PRUEBAS 4 // define la cantidad total de pruebas que se van a ejecutar.

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Ejecuta un conjunto predefinido de pruebas utilizando archivos de prueba.
/// Cada prueba analiza un archivo de texto y evalúa la funcionalidad del diccionario, el podio y las estadísticas de texto.
void ejecutarLoteDePruebas();

/// Muestra todas las claves del diccionario que generaron colisiones en la tabla hash. Recorre cada posición de la tabla.
void mostrarColisiones(tDic* dic, void mostrar(void*));

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

// Imprime una clave char
void imprimirClave(void* clave);

#endif
