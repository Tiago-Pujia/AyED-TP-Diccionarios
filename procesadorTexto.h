#ifndef PROCESADORTEXTO_H_INCLUDED
#define PROCESADORTEXTO_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define TAM_LINEA_TXT 1000
#define ES_PUNTUACION(c) ((c)=='.'||(c)==','||(c)==';'||(c)=='!'||(c)=='?'||(c)==':'||(c)=='"'||(c)=='('||(c)==')' || (c) == '-')
#define ES_LETRA(c) (((c)>= 'a' && (c)<= 'z') || ((c)>= 'A' && (c) <= 'Z'))
#define mi_toLower(X) (((X)>= 'A' && (X) <= 'Z') ? ((X)+32) : (X))
#define TAM_PAL 100
#define TOP_PAL 5

typedef struct
{
    int cantPalabras;
    int cantEspacios;
    int cantPuntuacion;
    char masUsadas[TOP_PAL][TAM_PAL];
}tProcesadorTexto;

void iniciarEnCero(tProcesadorTexto*aux);
int procesarArchivo(FILE*archivo, tDiccionario*dic,tProcesadorTexto* aux);
void trozar(char *linea, tDiccionario*dic, tProcesadorTexto*aux);
void acum(void* existente, void* nuevo);
int cmp_pal(const void* a, const void* b);
void normalizar(char * cadena);

void mostrar_estadisticas(const tProcesadorTexto* aux, const tDiccionario* dic);
void mostrar_palabra(void* clave, void* dato, void* param);

int ya_en_podio(const char *palabra, tProcesadorTexto* aux, int hasta);
void podioPalabras(tDiccionario*dic, int n, tProcesadorTexto*aux);
#endif // PROCESADORTEXTO_H_INCLUDED
