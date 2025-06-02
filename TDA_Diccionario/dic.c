#include "dic.h"

// Inicializa la tabla del diccionario en cero (listas vac�as)
void crearDic(tDic* const dic)
{
    memset(dic->tabla, 0, TAM_DIC * sizeof(tLista));
}

/// Inserta un nuevo elemento en el diccionario.
/// Si la clave ya existe, se actualiza la informaci�n utilizando la funci�n `actualizar`.
///
/// @param dic          Puntero al diccionario
/// @param clave        Clave asociada al nuevo dato
/// @param tamClave     Tama�o en bytes de la clave
/// @param dato         Informaci�n a almacenar
/// @param tamDato      Tama�o en bytes del dato
/// @param cmp          Funci�n de comparaci�n de claves
/// @param actualizar   Funci�n de actualizaci�n en caso de clave duplicada
///
/// @return EXITO si se insert� o actualiz�, SIN_MEM si hubo error de malloc.
int inserDic(tDic* const dic,
             void* clave, const size_t tamClave,
             const void* dato, const size_t tamDato,
             Cmp cmp, Actualizar actualizar)
{
    // Se obtiene la posici�n de la clave en la tabla hash
    size_t pos = _hasheo_dic(clave, tamClave);

    // Se accede a la lista de esa posici�n (manejo de colisiones)
    tLista* lista = &dic->tabla[pos];
    tNodo* nuevo;

    // Se recorre la lista buscando si la clave ya existe
    while (*lista)
    {
        if (cmp((*lista)->clave, clave) == 0)
        {
            // Si la clave ya existe, se actualiza el dato con la funci�n provista
            actualizar((*lista)->info, (void*)dato);
            return EXITO;
        }

        // Avanzamos al siguiente nodo
        lista = &(*lista)->sig;
    }

    // Si la clave no existe, se crea un nuevo nodo
    if (!(nuevo = (tNodo*)malloc(sizeof(tNodo))))
    {
        return SIN_MEM;
    }

    // Reservamos memoria para la informaci�n
    if (!(nuevo->info = malloc(tamDato)))
    {
        free(nuevo);
        return SIN_MEM;
    }

    // Reservamos memoria para la clave
    if (!(nuevo->clave = malloc(tamClave)))
    {
        free(nuevo->info);
        free(nuevo);
        return SIN_MEM;
    }

    // Se copian los datos al nodo
    memcpy(nuevo->info, dato, tamDato);
    memcpy(nuevo->clave, clave, tamClave);
    nuevo->tamInfo = tamDato;

    // Se inserta el nodo al final de la lista
    nuevo->sig = *lista; // Apunta a null
    *lista = nuevo;      // El ultimo nodo apunta al nuevo

    return EXITO;
}

/// Busca una clave en el diccionario y copia su informaci�n al par�metro `info`.
///
/// @param dic        Puntero al diccionario
/// @param clave      Clave que se desea buscar
/// @param tamClave   Tama�o en bytes de la clave
/// @param info       Puntero donde se copiar� la informaci�n encontrada
/// @param tamInfo    Tama�o m�ximo de bytes que se pueden copiar en `info`
/// @param cmp        Funci�n para comparar claves
///
/// @return EXITO si se encontr� la clave y se copi� la informaci�n, NO_ENCONTRADO si la clave no est� en el diccionario.
int obtenerDic(tDic* const dic,
               void* clave, const size_t tamClave,
               void* info, const size_t tamInfo,
               Cmp cmp)
{
    size_t pos = _hasheo_dic(clave, tamClave); // Se calcula la posici�n en la tabla hash correspondiente a la clave
    tLista* lista = &dic->tabla[pos]; // Se obtiene el puntero a la lista (manejo de colisiones)

    // Se recorre la lista buscando la clave
    while (*lista && cmp((*lista)->clave, clave))
    {
        lista = &(*lista)->sig;
    }

    // Si llegamos al final sin encontrarla, devolvemos error
    if (!*lista)
    {
        return NO_ENCONTRADO;
    }

    // Si se encontr�, se copia la informaci�n al puntero recibido,
    // limitando la cantidad de bytes al m�nimo entre lo que hay guardado y lo que se puede copiar
    memcpy(info, (*lista)->info, MIN((*lista)->tamInfo, tamInfo));

    return EXITO;
}

/// Elimina una entrada del diccionario asociada a una clave espec�fica.
/// Si la clave existe, copia su informaci�n al par�metro `info` antes de eliminarla.
///
/// @param dic        Puntero al diccionario
/// @param clave      Clave del elemento a eliminar
/// @param tamClave   Tama�o en bytes de la clave
/// @param info       Puntero donde se copiar� la informaci�n eliminada (opcional)
/// @param tamInfo    Tama�o m�ximo que se puede copiar en `info`
/// @param cmp        Funci�n de comparaci�n de claves
///
/// @return EXITO si se elimin� correctamente, NO_ENCONTRADO si la clave no existe.
int sacarDic(tDic* const dic,
             void* clave, const size_t tamClave,
             void* info, const size_t tamInfo,
             Cmp cmp)
{
    size_t pos = _hasheo_dic(clave, tamClave); // Se calcula la posici�n de la clave en la tabla hash correspondiente a la clave
    tLista* lista = &dic->tabla[pos]; // Obtener lista
    tNodo* aux; // Nodo auxiliar para apuntar al nodo a eliminar

    // Se recorre la lista buscando la clave
    while (*lista && cmp((*lista)->clave, clave))
    {
        lista = &(*lista)->sig;
    }

    // Si no se encontr� la clave, se informa
    if (!*lista)
    {
        return NO_ENCONTRADO;
    }

    // Se guarda la referencia del nodo que se va a eliminar
    aux = *lista;

    // Se copia la informaci�n al par�metro `info`
    memcpy(info, aux->info, MIN(aux->tamInfo, tamInfo));

    // Se desconecta el nodo de la lista
    *lista = aux->sig;

    // Se libera la memoria del nodo
    free(aux->info);
    free(aux->clave);
    free(aux);

    return EXITO;
}

/// Recorre todos los elementos del diccionario y aplica una acci�n a cada uno.
///
/// @param dic     Puntero constante al diccionario
/// @param accion  Funci�n que se ejecuta por cada elemento. Recibe clave, info y un par�metro extra.
/// @param param   Par�metro adicional que se le pasa a la funci�n `accion`
///
/// Recorre todas las listas de la tabla hash (una por cada posici�n), y para cada nodo existente, llama a la funci�n `accion`.
void recorrerDic(const tDic* const dic, Accion accion, void* param)
{
    tLista *lista;
    // Recorremos cada posici�n de la tabla
    for (int i = 0; i < TAM_DIC; i++)
    {
        // Apuntamos a la lista de la posici�n actual
        lista = (tLista*)&dic->tabla[i];

        // Recorremos la lista aplicando la acci�n a cada nodo
        while(*lista)
        {
            accion((*lista)->clave, (*lista)->info, param);
            lista = &(*lista)->sig;
        }
    }
}

/// Vac�a completamente el diccionario, liberando toda la memoria utilizada.
/// Recorre cada lista de la tabla hash y libera los nodos uno por uno.
///
/// @param dic  Puntero al diccionario que se desea vaciar
void vaciarDic(tDic* const dic)
{
    tLista *lista;
    tNodo *aux;
    size_t i;

    // Recorremos cada posici�n de la tabla
    for (i = 0; i < TAM_DIC; i++)
    {
        lista = &dic->tabla[i];  // Apuntamos al inicio de la lista en esta posici�n

        // Liberamos nodo por nodo dentro de la lista
        while (*lista)
        {
            aux = *lista;         // Guardamos el nodo actual
            *lista = aux->sig;    // Avanzamos al siguiente nodo

            free(aux->info);      // Liberamos la memoria del dato
            free(aux->clave);     // Liberamos la memoria de la clave
            free(aux);            // Liberamos el nodo en s�
        }
    }
}

/// Calcula un valor hash para una clave arbitraria, basado en una constante inicial y multiplicaci�n.
///
/// @param clave   Puntero a la clave (se trata como un arreglo de bytes)
/// @param len     Tama�o en bytes de la clave
///
/// @return        Posici�n v�lida dentro de la tabla hash
size_t _hasheo_dic(const void* clave, const size_t len)
{
    // Valor base inicial (n�mero primo grande para reducir colisiones)
    unsigned long hash = CONST_HASH;

    // Interpretamos la clave como un array de bytes
    const unsigned char* data = (const unsigned char*)clave;

    // Recorremos cada byte y actualizamos el hash con multiplicaci�n y suma
    for (size_t i = 0; i < len; i++)
    {
        // 37 es una constante multiplicadora com�n en hashing
        hash = hash * 37 + data[i];
    }

    // Finalmente, reducimos el hash al rango v�lido [0, TAM_DIC - 1]
    return hash % TAM_DIC;
}
