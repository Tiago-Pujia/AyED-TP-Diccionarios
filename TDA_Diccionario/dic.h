#ifndef DICCIONARIO_H
#define DICCIONARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =======================================================
//                  CONSTANTES GENERALES
// =======================================================

#define TAM_DIC          10          // Tama�o de la tabla hash
#define CONST_HASH     5947          // Constante utilizada en la funci�n hash

#define EXITO            1           // Operaci�n exitosa
#define DIC_VACIO        0           // Diccionario vac�o
#define NO_ENCONTRADO    0           // Clave no encontrada
#define SIN_MEM          0           // Error por falta de memoria
#define DUPLICADO        0           // Clave duplicada
#define ERROR            0           // Error gen�rico
#define LISTA_VACIA      0
#define LISTA_LLENA      0
// =======================================================
//                  MACROS UTILITARIAS
// =======================================================

#define MIN(X,Y) ( (X) > (Y) ? (Y) : (X) )

// =======================================================
//                  TIPOS DE DATOS
// =======================================================

/// Nodo de una lista dentro del diccionario (hash con colisiones)
typedef struct sNodo {
    void* info;         // Informaci�n almacenada
    void* clave;        // Clave asociada
    size_t tamClave;
    size_t tamInfo;     // Tama�o de la informaci�n
    struct sNodo* sig;  // Siguiente nodo en la lista
} tNodo;

typedef tNodo* tLista;  // Lista enlazada usada como manejo de colisiones

/// Diccionario implementado como tabla hash de listas
typedef struct {
    tLista tabla[TAM_DIC];  // Tabla de hash con listas
} tDic;

// =======================================================
//                  PUNTEROS A FUNCIONES AUXILIARES
// =======================================================

typedef int  (*Cmp)(const void*, const void*); // Comparaci�n de claves
typedef void (*Accion)(void*, void*, void*);   // Acci�n para recorrer el diccionario

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Inicializa la tabla hash en cero
void crearDic(tDic* const dic);

/// Inserta un elemento en el diccionario o actualiza si la clave ya existe
int inserDic(tDic* const dic, void* clave, const size_t tamClave, const void* info, const size_t tamInfo, Cmp cmp);

/// Busca una clave en el diccionario y copia su informaci�n
int obtenerDic(tDic* const dic, void* clave, const size_t tamClave, void* info, const size_t tamInfo, Cmp cmp);

/// Elimina una entrada del diccionario
int sacarDic(tDic* const dic, void* clave, const size_t tamClave, void* info, const size_t tamInfo, Cmp cmp);

/// Recorre todo el diccionario aplicando una acci�n a cada elemento
void recorrerDic(const tDic* const dic, Accion accion, void* param);

/// Vac�a completamente el diccionario, liberando memoria
void vaciarDic(tDic* const dic);

/// Funci�n de hash que devuelve una posici�n en la tabla
size_t _hasheo_dic(const void* clave, const size_t len);

#endif // DICCIONARIO_H
