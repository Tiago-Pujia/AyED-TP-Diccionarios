#ifndef INTERFAZUSUARIO_H_INCLUDED
#define INTERFAZUSUARIO_H_INCLUDED

#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

#include <stdio.h>
#include <string.h>

// =======================================================
//                  CONSTANTES GENERALES
// =======================================================

#define TAM_RUTA 256    /// Tamaño máximo para la ruta del archivo ingresado
#define TAM_LINEA 256   /// Tamaño máximo para líneas auxiliares (uso general)

// =======================================================
//                  MENSAJES
// =======================================================

#define MSJ_CONTINUAR "\n¿Desea analizar otro archivo? (1 = SI / 0 = NO): "

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

/// Limpia la consola, dependiendo del sistema operativo.
void limpiarConsola();

/// Lee una línea de texto desde la entrada estándar y elimina el salto de línea final.
void leerTexto(char *texto, size_t longitud);

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Solicita al usuario una ruta de archivo, valida su contenido,
/// procesa el texto y muestra los resultados.
void iniciarAnalisisTexto();

/// Muestra las instrucciones y descripción general del programa.
void mostrarInstrucciones();

/// Muestra un mensaje descriptivo según el tipo de error al validar un archivo.
void mostrarValidacionArch(int tipoError);

/// Muestra las estadísticas recolectadas y todas las palabras registradas
void mostrarEstadisticas(const tEstText* estText, const tDic* dic);

#endif // INTERFAZUSUARIO_H_INCLUDED
