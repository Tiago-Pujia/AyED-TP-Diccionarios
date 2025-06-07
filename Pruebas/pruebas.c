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
    tDic dic, dicPodio;
    tEstText estadisticas;
    FILE* arch;

    for(int i = 0; i < CANT_PRUEBAS; i++) {
        arch = fopen(archivosDePrueba[i], "r");
        if (arch == NULL) {
            printf("Error al abrir el archivo: %s\n", archivosDePrueba[i]);
            continue;
        }

        crearDic(&dic);
        crearDic(&dicPodio);
        iniEstadisticas(&estadisticas);

        procesarArch(arch, &dic, &estadisticas);
        generarPodioPalabras(&dic, TOP_PAL, comparaString, &dicPodio, comparaEntero);
        mostrarEstadisticas(&estadisticas, &dic, &dicPodio);
        mostrarColisiones(&dic, imprimir_clave);

        vaciarDic(&dic);
        vaciarDic(&dicPodio);
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
