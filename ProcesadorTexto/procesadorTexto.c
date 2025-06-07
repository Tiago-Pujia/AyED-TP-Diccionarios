#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

void acumularFrecuencia(void* existente, void* nuevo)
{
    (*(int*)existente)++;
}

void mostrarPalabra(void* clave, void* dato, void* param)
{
    printf("~ %s: %d\n", (char*)clave, *(int*)dato);
}

void strToLower(char* str)
{
    while (*str)
    {
        *str = MI_TOLOWER(*str);
        str++;
    }
}

int comparaString(const void *a, const void *b)
{
    return strcmp((char*)a, (char*)b);
}

int comparaEntero(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Procesa un archivo de texto línea por línea.
int procesarArch(FILE* arch, tDic* dic, tEstText* estText)
{
    rewind(arch);  // Nos aseguramos de empezar desde el inicio del archivo

    char lineaLeida[TAM_LINEA_TXT];

    // Leemos el archivo línea por línea
    while (fgets(lineaLeida, TAM_LINEA_TXT, arch))
    {
        // Procesamos la línea para contar palabras, espacios, signos y actualizar el diccionario
        trozarLinea(lineaLeida, dic, estText);
    }

    return EXITO;
}

/// Procesa una línea de texto, separa las palabras, y actualiza el diccionario y las estadísticas.
void trozarLinea(char* linea, tDic* dic, tEstText* estText)
{
    char palabra[TAM_PAL];      // Buffer para armar una palabra
    char *pPalabra = palabra;   // Puntero móvil dentro del buffer
    int contador = 1;           // Aparece 1 vez cuando se inserta

    do
    {
        if (*linea && ES_LETRA(*linea))  // Si es una letra válida, la agregamos al buffer
        {
            *pPalabra = *linea;
            pPalabra++;
        }
        else
        {
            if (pPalabra != palabra)  // Si hay contenido en el buffer, se formó una palabra
            {
                *pPalabra = '\0';  // Terminamos la palabra con null
                strToLower(palabra);  // Convertimos a minúscula y limpiamos
                inserDic(dic, palabra, strlen(palabra) + 1, &contador, sizeof(contador), comparaString, acumularFrecuencia);
                pPalabra = palabra;  // Reiniciamos el buffer para la siguiente palabra
                estText->cantPalabras++;
            }

            // Contamos espacios o tabulaciones
            if (*linea == ' ' || *linea == '\t')
                estText->cantEspacios++;
            else if (ES_PUNTUACION(*linea))
                estText->cantPuntuacion++;
        }
    }
    while (*linea++);
}

/// Inicializa los contadores de estadísticas en cero
void iniEstadisticas(tEstText* estText)
{
    estText->cantPalabras = 0;
    estText->cantEspacios = 0;
    estText->cantPuntuacion = 0;
}

/// Genera el podio dinámico de las palabras más frecuentes del diccionario principal.
/// - Inserta palabras en el diccionario `podioDic`, asignándoles un puesto numérico.
/// - Las palabras con la misma frecuencia comparten el mismo puesto (empates).
void generarPodioPalabras(tDic* dic, size_t cantPuestos, Cmp cmpClave, tDic *dicPodio, Cmp cmpEnt)
{
    tNodo* palabraMasUsada = NULL, *iguales;
    int posicionYaUsada = 1, cantEmpate;

    while(posicionYaUsada <= cantPuestos)
    {
        palabraMasUsada = buscarMasUsada(dic, cmpClave, dicPodio);

        if(palabraMasUsada)
        {
            cantEmpate = 0;

            inserDic(dicPodio, palabraMasUsada->clave, strlen(palabraMasUsada->clave) + 1, &posicionYaUsada, sizeof(posicionYaUsada), cmpClave, NULL);
            iguales = buscarIgual(dic, palabraMasUsada, cmpEnt, dicPodio, cmpClave);

            while(iguales)
            {
                inserDic(dicPodio, iguales->clave, strlen(iguales->clave) + 1, &posicionYaUsada, sizeof(posicionYaUsada), cmpClave, NULL);
                cantEmpate++;
                iguales = buscarIgual(dic, iguales, cmpEnt,dicPodio, cmpClave);


            }

            if(cantEmpate > 0)
                posicionYaUsada += cantEmpate;
        }
        posicionYaUsada++;

    }
}
/*
void generarPodioPalabras(tDic* dic, int cantPuestos, Cmp cmpClaves, tDic* podioDic, Cmp cmpFrecuencias)
{
    size_t puestoActual = 1;
    size_t cantInsertado = 0;

    while (puestoActual <= cantPuestos)
    {
        cantInsertado = 0;

        // Buscar la palabra con mayor frecuencia que aún no está en el podio
        tNodo* nodoMasFrecuente = buscarMasUsada(dic, cmpClaves, podioDic);
        if (!nodoMasFrecuente)
            break;

        // Insertar palabra al podio con el puesto actual
        inserDic(podioDic, nodoMasFrecuente->clave, strlen((char*)nodoMasFrecuente->clave) + 1, &puestoActual, sizeof(int), comparaString, NULL);

        cantInsertado++;

        // Buscar y agregar otras palabras con la misma frecuencia (empates)
        tNodo* nodoEmpatado = buscarIgual(dic, nodoMasFrecuente, cmpFrecuencias, podioDic, cmpClaves);
        while (nodoEmpatado)
        {
            inserDic(podioDic, nodoEmpatado->clave, strlen((char*)nodoEmpatado->clave) + 1, &puestoActual, sizeof(int), comparaString, NULL);

            nodoEmpatado = buscarIgual(dic, nodoEmpatado, cmpFrecuencias, podioDic, cmpClaves);
            cantInsertado++;
        }

        // Avanzar al siguiente número de puesto
        puestoActual += cantInsertado;
    }
}
*/

/*
void generarPodioPalabras(tDic* dic, int posicion, Cmp cmp, tDic*dicPodio, Cmp cmpEnt)
{
    tNodo* palabraMasUsada = NULL, *iguales;
    int posicionYaUsada = 1, cantEmpate;

    while(posicionYaUsada <= posicion)
    {
        palabraMasUsada = buscarMasUsada(dic, cmp, dicPodio);

        if(palabraMasUsada)
        {
            cantEmpate=0;

            inserDic(dicPodio, palabraMasUsada->clave, strlen(palabraMasUsada->clave) + 1, &posicionYaUsada, sizeof(posicionYaUsada), comparaString, NULL);
            iguales = buscarIgual(dic, palabraMasUsada, cmpEnt, dicPodio, cmp);

            while(iguales)
            {
                inserDic(dicPodio, iguales->clave, strlen(iguales->clave) + 1, &posicionYaUsada, sizeof(posicionYaUsada), comparaString, NULL);
                cantEmpate++;
                iguales = buscarIgual(dic, iguales, cmpEnt,dicPodio, cmp);
            }

            if(cantEmpate > 0)
                posicionYaUsada += cantEmpate;
            else
                posicionYaUsada++;
        }
        posicionYaUsada++;

    }
}
*/

/// Busca otra palabra en el diccionario que tenga la misma frecuencia que `dato`,
/// y que aún no haya sido insertada en el diccionario del podio (`dicPodio`).
tNodo* buscarIgual(tDic* dic, tNodo* dato, Cmp cmp, tDic* dicPodio, Cmp cmpStr)
{
    // Recorre toda la tabla hash
    for(size_t i = 0; i < TAM_DIC; i++)
    {
        tLista lista = dic->tabla[i];  // Lista en la posición i

        // Recorre todos los nodos de la lista
        while(lista)
        {
            // Compara si el valor (frecuencia) del nodo actual es igual al del nodo de referencia,
            // y verifica que aún no haya sido insertado en el podio (dicPodio)
            if((cmp(lista->info, dato->info) == 0) &&
               !estaEnDic(dicPodio, lista->clave, cmpStr))
            {
                return lista;  // Devuelve el nodo empatado (misma frecuencia y no repetido)
            }

            lista = lista->sig;  // Avanza al siguiente nodo
        }
    }

    return NULL; // No se encontró otro nodo con la misma frecuencia no repetido
}

/// Busca la palabra más frecuente en el diccionario (`dic`) que aún no haya sido insertada en el podio.
tNodo* buscarMasUsada(tDic* dic, Cmp cmp, tDic* dicPodio)
{
    tNodo* palabraMasUsada = NULL;  // Puntero al nodo con mayor frecuencia hallado hasta ahora

    // Recorre todas las posiciones de la tabla hash
    for (size_t i = 0; i < TAM_DIC; i++)
    {
        tLista lista = dic->tabla[i];  // Lista de colisiones en la posición i

        // Recorre todos los nodos de la lista
        while (lista)
        {
            // Verifica que la palabra aún no haya sido usada en el podio
            // Si es la primera palabra encontrada o su frecuencia es mayor a la actual
            if (
                ( !estaEnDic(dicPodio, lista->clave, cmp) ) &&
                ( !palabraMasUsada || (*(int*)lista->info > *(int*)palabraMasUsada->info) )
               )
            {
                    palabraMasUsada = lista;  // Actualiza el puntero con la nueva más usada
            }

            lista = lista->sig;  // Avanza al siguiente nodo
        }
    }

    return palabraMasUsada;  // Devuelve el nodo con la frecuencia más alta no repetido
}

/// Verifica si una clave (palabra) ya está presente en el diccionario.
int estaEnDic(tDic* dic, const void* clave, Cmp cmp)
{
    // Recorre cada posición de la tabla hash
    for (size_t i = 0; i < TAM_DIC; i++)
    {
        tLista lista = dic->tabla[i];  // Apunta a la lista en esa posición

        // Recorre cada nodo en la lista
        while (lista)
        {
            // Si la clave coincide con la buscada (usando la función de comparación)
            if (cmp(lista->clave, clave) == 0)
            {
                return 1;  // La clave ya está en el diccionario
            }

            lista = lista->sig;  // Avanza al siguiente nodo
        }
    }

    return 0; // La clave no fue encontrada
}
