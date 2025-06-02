#include "dic.h"

// Inicializa la tabla del diccionario en cero (listas vacías)
void crearDic(tDic* const dic)
{
    memset(dic->tabla, 0, TAM_DIC * sizeof(tLista));
}

/// Inserta un nuevo elemento en el diccionario.
/// Si la clave ya existe, se actualiza la información utilizando la función `actualizar`.
///
/// @param dic          Puntero al diccionario
/// @param clave        Clave asociada al nuevo dato
/// @param tamClave     Tamaño en bytes de la clave
/// @param dato         Información a almacenar
/// @param tamDato      Tamaño en bytes del dato
/// @param cmp          Función de comparación de claves
/// @param actualizar   Función de actualización en caso de clave duplicada
///
/// @return EXITO si se insertó o actualizó, SIN_MEM si hubo error de malloc.
int inserDic(tDic* const dic,
             void* clave, const size_t tamClave,
             const void* dato, const size_t tamDato,
             Cmp cmp, Actualizar actualizar)
{
    // Se obtiene la posición de la clave en la tabla hash
    size_t pos = _hasheo_dic(clave, tamClave);

    // Se accede a la lista de esa posición (manejo de colisiones)
    tLista* lista = &dic->tabla[pos];
    tNodo* nuevo;

    // Se recorre la lista buscando si la clave ya existe
    while (*lista)
    {
        if (cmp((*lista)->clave, clave) == 0)
        {
            // Si la clave ya existe, se actualiza el dato con la función provista
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

    // Reservamos memoria para la información
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

/// Busca una clave en el diccionario y copia su información al parámetro `info`.
///
/// @param dic        Puntero al diccionario
/// @param clave      Clave que se desea buscar
/// @param tamClave   Tamaño en bytes de la clave
/// @param info       Puntero donde se copiará la información encontrada
/// @param tamInfo    Tamaño máximo de bytes que se pueden copiar en `info`
/// @param cmp        Función para comparar claves
///
/// @return EXITO si se encontró la clave y se copió la información, NO_ENCONTRADO si la clave no está en el diccionario.
int obtenerDic(tDic* const dic,
               void* clave, const size_t tamClave,
               void* info, const size_t tamInfo,
               Cmp cmp)
{
    size_t pos = _hasheo_dic(clave, tamClave); // Se calcula la posición en la tabla hash correspondiente a la clave
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

    // Si se encontró, se copia la información al puntero recibido,
    // limitando la cantidad de bytes al mínimo entre lo que hay guardado y lo que se puede copiar
    memcpy(info, (*lista)->info, MIN((*lista)->tamInfo, tamInfo));

    return EXITO;
}

/// Elimina una entrada del diccionario asociada a una clave específica.
/// Si la clave existe, copia su información al parámetro `info` antes de eliminarla.
///
/// @param dic        Puntero al diccionario
/// @param clave      Clave del elemento a eliminar
/// @param tamClave   Tamaño en bytes de la clave
/// @param info       Puntero donde se copiará la información eliminada (opcional)
/// @param tamInfo    Tamaño máximo que se puede copiar en `info`
/// @param cmp        Función de comparación de claves
///
/// @return EXITO si se eliminó correctamente, NO_ENCONTRADO si la clave no existe.
int sacarDic(tDic* const dic,
             void* clave, const size_t tamClave,
             void* info, const size_t tamInfo,
             Cmp cmp)
{
    size_t pos = _hasheo_dic(clave, tamClave); // Se calcula la posición de la clave en la tabla hash correspondiente a la clave
    tLista* lista = &dic->tabla[pos]; // Obtener lista
    tNodo* aux; // Nodo auxiliar para apuntar al nodo a eliminar

    // Se recorre la lista buscando la clave
    while (*lista && cmp((*lista)->clave, clave))
    {
        lista = &(*lista)->sig;
    }

    // Si no se encontró la clave, se informa
    if (!*lista)
    {
        return NO_ENCONTRADO;
    }

    // Se guarda la referencia del nodo que se va a eliminar
    aux = *lista;

    // Se copia la información al parámetro `info`
    memcpy(info, aux->info, MIN(aux->tamInfo, tamInfo));

    // Se desconecta el nodo de la lista
    *lista = aux->sig;

    // Se libera la memoria del nodo
    free(aux->info);
    free(aux->clave);
    free(aux);

    return EXITO;
}

/// Recorre todos los elementos del diccionario y aplica una acción a cada uno.
///
/// @param dic     Puntero constante al diccionario
/// @param accion  Función que se ejecuta por cada elemento. Recibe clave, info y un parámetro extra.
/// @param param   Parámetro adicional que se le pasa a la función `accion`
///
/// Recorre todas las listas de la tabla hash (una por cada posición), y para cada nodo existente, llama a la función `accion`.
void recorrerDic(const tDic* const dic, Accion accion, void* param)
{
    tLista *lista;
    // Recorremos cada posición de la tabla
    for (int i = 0; i < TAM_DIC; i++)
    {
        // Apuntamos a la lista de la posición actual
        lista = (tLista*)&dic->tabla[i];

        // Recorremos la lista aplicando la acción a cada nodo
        while(*lista)
        {
            accion((*lista)->clave, (*lista)->info, param);
            lista = &(*lista)->sig;
        }
    }
}

/// Vacía completamente el diccionario, liberando toda la memoria utilizada.
/// Recorre cada lista de la tabla hash y libera los nodos uno por uno.
///
/// @param dic  Puntero al diccionario que se desea vaciar
void vaciarDic(tDic* const dic)
{
    tLista *lista;
    tNodo *aux;
    size_t i;

    // Recorremos cada posición de la tabla
    for (i = 0; i < TAM_DIC; i++)
    {
        lista = &dic->tabla[i];  // Apuntamos al inicio de la lista en esta posición

        // Liberamos nodo por nodo dentro de la lista
        while (*lista)
        {
            aux = *lista;         // Guardamos el nodo actual
            *lista = aux->sig;    // Avanzamos al siguiente nodo

            free(aux->info);      // Liberamos la memoria del dato
            free(aux->clave);     // Liberamos la memoria de la clave
            free(aux);            // Liberamos el nodo en sí
        }
    }
}

/// Calcula un valor hash para una clave arbitraria, basado en una constante inicial y multiplicación.
///
/// @param clave   Puntero a la clave (se trata como un arreglo de bytes)
/// @param len     Tamaño en bytes de la clave
///
/// @return        Posición válida dentro de la tabla hash
size_t _hasheo_dic(const void* clave, const size_t len)
{
    // Valor base inicial (número primo grande para reducir colisiones)
    unsigned long hash = CONST_HASH;

    // Interpretamos la clave como un array de bytes
    const unsigned char* data = (const unsigned char*)clave;

    // Recorremos cada byte y actualizamos el hash con multiplicación y suma
    for (size_t i = 0; i < len; i++)
    {
        // 37 es una constante multiplicadora común en hashing
        hash = hash * 37 + data[i];
    }

    // Finalmente, reducimos el hash al rango válido [0, TAM_DIC - 1]
    return hash % TAM_DIC;
}
