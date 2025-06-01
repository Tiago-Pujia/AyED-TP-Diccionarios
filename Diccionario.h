#ifndef DICCIONARIO_H
#define DICCIONARIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_DICC 10
#define CONST_HASHEO 5947

#define TODO_OK 1

#define DICC_VACIO 0
#define NO_ENCONTRADO 0
#define SIN_MEM 0
#define DUPLICADO 0
#define ERROR 0



#define MAX_ALU 15

typedef struct sNodo
{
    void* info;
    void* clave;
    size_t tam_info;
    struct sNodo* sig;
}
tNodo;

typedef tNodo* tLista;

typedef struct
{
    tLista tabla[TAM_DICC]; //tabla hash
}
tDiccionario;




typedef int(*Cmp)(const void* a, const void* b);
typedef void (*Accion)(void*, void*, void*);
typedef void (*Actualizar)(void*, void*);


void crear_dic(tDiccionario* pd);
int poner_dic(tDiccionario* pd, void* clave, size_t tam_clave, const void* info, size_t tam_info, Cmp cmp, Actualizar actualizar);
int obtener_dic(tDiccionario* pd, void* clave, size_t tam_clave, void* info, size_t tam_info, Cmp cmp);
int sacar_dic(tDiccionario* pd, void* clave, size_t tam_clave, void* info, size_t tam_info, Cmp cmp);
void recorrer_dic(const tDiccionario* pd, Accion accion, void* param);
void vaciar_dic(const tDiccionario* pd);

size_t _hasheo_dic(void* clave, size_t len);

#endif // DICCIONARIO_H
