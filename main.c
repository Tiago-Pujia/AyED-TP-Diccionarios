#include "Diccionario.h"


//EJEMPLO INTS
int comparar_ints(const void* a, const void* b);
void Imprimir_ints(void* a, void* params);
void actualizar_ints(void* a, void* b);
//EJEMPLO INTS



//EJEMPLO REALISTA (en progreso...)

typedef struct
{
    int dni;
    char nyAp[50];
    int notas[3];
    char estado;
}
Alumno;

int comparar_alumnos(const void* a, const void* b);
void Imprimir_alumnos(void* a, void* params);

//EJEMPLO REALISTA (en progreso...)


int main()
{
    tDiccionario diccionario;
    //EJEMPLO INTS

    int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,11};
    int b;
    crear_dic(&diccionario);

    for(int i = 0 ; i < (sizeof(a) / sizeof(int)) ; i++)
    poner_dic(&diccionario, &a[i], sizeof(int), &a[i], sizeof(int), comparar_ints, actualizar_ints);


    printf("Diccionario:\n");
    recorrer_dic(&diccionario, Imprimir_ints,NULL);
    printf("Fin\n\n");

    sacar_dic(&diccionario, &a[2], sizeof(int), &b, sizeof(int), comparar_ints);
    printf("%d\n", b);

    printf("\nDiccionario:\n");
    recorrer_dic(&diccionario, Imprimir_ints,NULL);
    printf("Fin\n\n");

    vaciar_dic(&diccionario);

    printf("\nDiccionario:\n");
    recorrer_dic(&diccionario, Imprimir_ints,NULL);
    printf("Fin\n\n");

    return 0;
}

int comparar_ints(const void* a, const void* b)
{
    int* a1 = (int*) a;
    int* b1 = (int*) b;
    return *a1 - *b1;
}

void Imprimir_ints(void* a, void* params)
{
    int* b = (int*) a;
    printf("%d\n", *b);
}

void actualizar_ints(void* a, void* b)
{
    int* a1 = (int*) a;
    int* b1 = (int*) b;
    *a1 = *a1 + *b1;
}


//EJEMPLO REALISTA (en progreso...)
int comparar_alumnos(const void* a, const void* b)
{
    Alumno* a1 = (Alumno*) a;
    Alumno* a2 = (Alumno*) b;

    return a1->dni - a2->dni;
}
void Imprimir_alumnos(void* a, void* params)
{
    Alumno* a1 = (Alumno*) a;
    printf("\nAlumno: %s\n", a1->nyAp);
    printf("Dni: %d\n", a1->dni);
    printf("notas: %d,%d,%d\n", a1->notas[0], a1->notas[1], a1->notas[2]);
    printf("Estado: %c\n", a1->estado);
}
//EJEMPLO REALISTA (en progreso...)
