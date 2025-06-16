#include "pruebas.h"

/// Ejecuta una serie de pruebas sobre archivos de texto espec�ficos. Tiene el objetivo de:
///
/// - Verificar el funcionamiento del diccionario gen�rico.
/// - Evaluar el procesamiento de estad�sticas de texto.
/// - Mostrar el podio de palabras m�s usadas.
/// - Mostrar las colisiones en la tabla hash de forma clara.
///
/// Cada archivo es procesado por separado: se inicializa el diccionario, se leen las palabras,
/// se generan estad�sticas y luego se imprimen las colisiones que ocurrieron durante la inserci�n.

void ejecutarLoteDePruebas() {
    char* archivosDePrueba[CANT_PRUEBAS] = {
        "./Pruebas/ArchivosDePrueba/largo.txt",
        "./Pruebas/ArchivosDePrueba/hola.txt",
        "./Pruebas/ArchivosDePrueba/repetidas.txt",
        "./Pruebas/ArchivosDePrueba/cuento.txt"
    };
    tDic dicc;
    tLista listaPodio;
    tEstText estadisticas;
    FILE* arch;

        for(int i = 0; i < CANT_PRUEBAS; i++)
        {
            arch = fopen(archivosDePrueba[i], "r");
            if (arch == NULL)
            {
                printf("Error al abrir el archivo: %s\n", archivosDePrueba[i]);
                continue;
            }

        crearDic(&dicc);
        iniEstadisticas(&estadisticas);

        procesarArch(arch, &dicc, &estadisticas);

        crearListaDesdeDicc(&dicc, cmpInfo, &listaPodio);

        mostrarEstadisticas(&estadisticas, &dicc, &listaPodio);
        mostrarColisiones(&dicc, imprimir_clave);

        destruirLista(&listaPodio);
        vaciarDic(&dicc);

        fclose(arch);

    }
}


// Muestra todas las claves del diccionario que generaron colisiones.
// Recorre la tabla hash e imprime las claves en listas con m�s de un nodo.
void mostrarColisiones(tDic* dic, void (*mostrar_clave)(void*))
{
    tNodo* nodo;

    for (int i = 0; i < TAM_DIC; i++)
    {
        nodo = dic->tabla[i];
        if (!nodo)
        {
            continue; // Posicion vacia
        }


        printf("Posicion %d:", i);
        while (nodo)
        {
            printf(" ");
            mostrar_clave(nodo->clave);
            nodo = nodo->sig;
            if (nodo) printf(" ->");
        }
        printf(" -> NULL\n");
    }
}

void imprimir_clave(void* clave)
{
    char* c = clave;
    printf("(%s)", c);
}
