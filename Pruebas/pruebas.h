#ifndef PRUEBAS_H_INCLUDED
#define PRUEBAS_H_INCLUDED

#include "stdio.h"
#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"
#include "../Interfaz/interfaz.h"

#define CANT_PRUEBAS 4
void ejecutarLoteDePruebas();
void mostrarColisiones(tDic* dic, void (*mostrar_clave)(void*));
void imprimir_clave(void* clave);

#endif
