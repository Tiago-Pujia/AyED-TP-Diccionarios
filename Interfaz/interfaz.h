#ifndef INTERFAZUSUARIO_H_INCLUDED
#define INTERFAZUSUARIO_H_INCLUDED

#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

#include <stdio.h>
#include <string.h>

// =======================================================
//                  CONSTANTES GENERALES
// =======================================================

#define TAM_RUTA 256    /// Tama�o m�ximo para la ruta del archivo ingresado
#define TAM_LINEA 256   /// Tama�o m�ximo para l�neas auxiliares (uso general)

// =======================================================
//                  MENSAJES
// =======================================================

#define MSJ_CONTINUAR "\n�Desea analizar otro archivo? (1 = SI / 0 = NO): "

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

/// Limpia la consola, dependiendo del sistema operativo.
void limpiarConsola();

/// Lee una l�nea de texto desde la entrada est�ndar y elimina el salto de l�nea final.
void leerTexto(char *texto, size_t longitud);

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Solicita al usuario una ruta de archivo, valida su contenido,
/// procesa el texto y muestra los resultados.
void iniciarAnalisisTexto();

/// Muestra las instrucciones y descripci�n general del programa.
void mostrarInstrucciones();

/// Muestra un mensaje descriptivo seg�n el tipo de error al validar un archivo.
void mostrarValidacionArch(int tipoError);

/// Muestra las estad�sticas recolectadas y todas las palabras registradas
void mostrarEstadisticas(const tEstText* estText, const tDic* dic);

#endif // INTERFAZUSUARIO_H_INCLUDED
