#ifndef INTERFAZUSUARIO_H_INCLUDED
#define INTERFAZUSUARIO_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define MAX_RUTA 256
#define TAM_LINEA 256

void mostrarInterfaz();
void imprimir_especificacion();
int validar_archivo(const char* ruta_archivo, FILE** pArchivo);

#endif // INTERFAZUSUARIO_H_INCLUDED
