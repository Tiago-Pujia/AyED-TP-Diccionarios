#ifndef INTERFAZUSUARIO_H_INCLUDED
#define INTERFAZUSUARIO_H_INCLUDED

#include <stdio.h>
#include <string.h>

// =======================================================
//                  CONSTANTES GENERALES
// =======================================================

#define MAX_RUTA 256    /// Tama�o m�ximo para la ruta del archivo ingresado
#define TAM_LINEA 256   /// Tama�o m�ximo para l�neas auxiliares (uso general)

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Muestra las instrucciones y descripci�n general del programa.
void mostrarInstrucciones();

/// Solicita al usuario una ruta de archivo, valida su contenido,
/// procesa el texto y muestra los resultados.
void iniciarAnalisisTexto();

/// Se realiza una verificaci�n sobre el archivo enviado: Que exista, que no este vacio y que sea .txt
int validarArchivoTxt(const char* ruta, FILE** arch);

#endif // INTERFAZUSUARIO_H_INCLUDED
