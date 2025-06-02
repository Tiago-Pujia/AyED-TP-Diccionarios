# Indice

- [Indice](#indice)
- [📦 TDA DICCIONARIO](#-tda-diccionario)
  - [📄 Lista de archivos](#-lista-de-archivos)
  - [🧠 ¿Qué es el TDA Diccionario?](#-qué-es-el-tda-diccionario)
  - [⚙️ Estructura y primitivas](#️-estructura-y-primitivas)
  - [🧠 Flujo de Ejecución](#-flujo-de-ejecución)
    - [📦 1. Tabla Hash](#-1-tabla-hash)
    - [🔗 2. Manejo de colisiones (encadenamiento)](#-2-manejo-de-colisiones-encadenamiento)
    - [⚙️ 3. Inserción y actualización (inserDic)](#️-3-inserción-y-actualización-inserdic)
    - [🔍 4. Búsqueda (obtenerDic)](#-4-búsqueda-obtenerdic)
    - [❌ 5. Eliminación (sacarDic)](#-5-eliminación-sacardic)
    - [🔄 6. Recorrido (recorrerDic)](#-6-recorrido-recorrerdic)
    - [🧩 7. Genérico y reutilizable](#-7-genérico-y-reutilizable)
- [📁 Procesador de Texto](#-procesador-de-texto)
  - [📄 Lista de Archivos](#-lista-de-archivos-1)
  - [🧠 ¿Qué es este Procesador de Texto?](#-qué-es-este-procesador-de-texto)
  - [⚙️ Estructura y Primitivas](#️-estructura-y-primitivas-1)
  - [🧠 Flujo de Ejecución](#-flujo-de-ejecución-1)
    - [📥 1. Lectura del archivo (procesarArch)](#-1-lectura-del-archivo-procesararch)
    - [✂️ 2. Trozado de la línea (trozarLinea)](#️-2-trozado-de-la-línea-trozarlinea)
    - [🧠 3. Almacenamiento en el diccionario (trozarLinea)](#-3-almacenamiento-en-el-diccionario-trozarlinea)
    - [🏆 4. Generación del podio (generarPodioPalabras)](#-4-generación-del-podio-generarpodiopalabras)
    - [📊 5. Visualización de resultados](#-5-visualización-de-resultados)
- [🧩 Interfaz](#-interfaz)
  - [📄 Lista de Archivos](#-lista-de-archivos-2)
  - [🧠 ¿Qué es y para qué sirve el módulo Interfaz?](#-qué-es-y-para-qué-sirve-el-módulo-interfaz)
  - [⚙️ Primitivas](#️-primitivas)
  - [Flujo de Ejecución](#flujo-de-ejecución)
    - [🪧 1. Muestra las instrucciones iniciales (mostrarInstrucciones)](#-1-muestra-las-instrucciones-iniciales-mostrarinstrucciones)
    - [🧾 2. Solicita la ruta del archivo (iniciarAnalisisTexto)](#-2-solicita-la-ruta-del-archivo-iniciaranalisistexto)
    - [🛡️ 3. Valida el archivo (validarArchivoTxt)](#️-3-valida-el-archivo-validararchivotxt)
    - [🔁 4. Coordina el procesamiento (iniciarAnalisisTexto)](#-4-coordina-el-procesamiento-iniciaranalisistexto)
    - [🧹 5. Libera recursos](#-5-libera-recursos)
  - [📚 Créditos y Referencias](#-créditos-y-referencias)
    - [👨‍💻 Autores del proyecto](#-autores-del-proyecto)
    - [👩‍🏫 Docentes responsables](#-docentes-responsables)
    - [🔗 Bibliografía y fuentes consultadas](#-bibliografía-y-fuentes-consultadas)

---

# 📦 TDA DICCIONARIO

## 📄 Lista de archivos

Los archivos del TDA Diccionario se encuentran en la carpeta:

~~~
TDA_Diccionario/
├── diccionario.c       // Implementación de primitivas del TDA
└── diccionario.h       // Declaraciones, estructuras, macros y firmas de funciones
~~~

## 🧠 ¿Qué es el TDA Diccionario?

![](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2f/Hash_table-es.svg/1920px-Hash_table-es.svg.png)

Un **diccionario** (también conocido como *hashtable*) permite asociar claves con valores, como una especie de "mapa". Es una estructura de datos que permite:

- Insertar un elemento asociado a una clave.
- Obtener el valor a partir de una clave.
- Eliminar un elemento dada su clave.
- Recorrer todos los elementos existentes.

Internamente, el diccionario está implementado como una **tabla de punteros a listas enlazadas**. La ubicación de cada clave se determina a partir de una función de hash, que convierte una clave arbitraria en un índice de la tabla o vector.

- Cada elemento se inserta en una posición determinada por una **función de hash** que transforma la clave en un índice dentro de la tabla.
- Si dos claves diferentes generan el mismo índice (colisión), se encadenan en una lista dentro de esa posición.
- De esta manera, la búsqueda, inserción y eliminación pueden mantenerse eficientes incluso con múltiples datos.

## ⚙️ Estructura y primitivas

Por cada nodo de la lista, se tiene la siguiente estructura:

~~~c
/// Nodo de una lista dentro del diccionario (hash con colisiones)
typedef struct sNodo {
    void* clave;        // Clave asociada
    void* info;         // Información almacenada
    size_t tamInfo;     // Tamaño de la información
    struct sNodo* sig;  // Siguiente nodo en la lista
} tNodo;

typedef tNodo* tLista;  // Lista enlazada usada como manejo de colisiones
~~~

Luego, se crea la 'hash table', que es un vector donde cada posición apunta a una lista:

~~~c
/// Diccionario implementado como tabla hash de listas
typedef struct {
    tLista tabla[TAM_TABLA_HASH];  // Tabla de hash con listas
} tDic;
~~~

Se tienen las siguientes primitivas

| Función       | Descripción                                                              |
| ------------- | ------------------------------------------------------------------------ |
| `crearDic`    | Inicializa la tabla hash en NULL.                                        |
| `inserDic`    | Inserta un elemento nuevo o actualiza uno existente si la clave ya está. |
| `obtenerDic`  | Busca una clave y copia su información asociada.                         |
| `sacarDic`    | Elimina una entrada por clave y devuelve su información.                 |
| `vaciarDic`   | Libera toda la memoria ocupada por el diccionario.                       |
| `recorrerDic` | Recorre todos los elementos aplicando una función de acción (`Accion`).  |
| `_hasheo_dic` | Función hash interna para mapear claves a índices.                       |

## 🧠 Flujo de Ejecución

### 📦 1. Tabla Hash

~~~c
tLista tabla[TAM_TABLA_HASH];
~~~

El diccionario está compuesto por un arreglo llamado tabla, de tamaño fijo (TAM_DIC). Cada posición de este arreglo apunta a una lista enlazada.

~~~C
size_t _hasheo_dic(const void* clave, size_t len);
~~~

Cada elemento que se quiera insertar se transforma en una posición dentro de esa tabla mediante una función hash. Esta función toma los bytes de la clave y genera un número entre `0` y `TAM_DIC - 1`.

### 🔗 2. Manejo de colisiones (encadenamiento)

Dos claves diferentes pueden terminar en la misma posición de la tabla (colisión). Para resolverlo, se utiliza una lista enlazada en cada posición de la tabla. Cada `tNodo` cumple la siguiente estructura:

~~~c
typedef struct sNodo {
    void* clave;        // Clave asociada
    void* info;         // Información almacenada
    size_t tamInfo;     // Tamaño de la información
    struct sNodo* sig;  // Siguiente nodo en la lista
} tNodo;
~~~

### ⚙️ 3. Inserción y actualización (inserDic)

~~~c
int inserDic(tDic* const dic, void* clave, const size_t tamClave, const void* info, const size_t tamInfo, Cmp cmp, Actualizar actualizar);
~~~

Cuando se inserta una clave:
1. Se calcula su hash
2. Se accede a la lista en esa posición
3. Si ya existe esa clave → se llama a una función de actualización.
4. Si no existe → se crea un nuevo nodo y se inserta al final de la lista.

### 🔍 4. Búsqueda (obtenerDic)

~~~c
int obtenerDic(tDic* const dic, void* clave, const size_t tamClave, void* info, const size_t tamInfo, Cmp cmp);
~~~

Para obtener el valor asociado a una clave:

1. Se calcula el hash de la clave.
2. Se recorre la lista correspondiente comparando cada nodo con una función de comparación (Cmp).
3. Si se encuentra la clave, se copia el valor al espacio del usuario.

### ❌ 5. Eliminación (sacarDic)

~~~c
int sacarDic(tDic* const dic, void* clave, const size_t tamClave, void* info, const size_t tamInfo, Cmp cmp);
~~~

Para eliminar una clave:

1. Se busca su posición con el hash
2. Se recorre la lista hasta encontrarla.
3. Se desconecta el nodo y se libera la memoria de clave, dato y nodo.

### 🔄 6. Recorrido (recorrerDic)

~~~c
void recorrerDic(const tDic* const dic, Accion accion, void* param);
~~~

La función `recorrerDic` permite recorrer todos los nodos de la tabla y aplicar una función a cada uno.

### 🧩 7. Genérico y reutilizable

El TDA no depende del tipo de clave o valor porque:
- Usa `void*` para clave e info.
- Requiere que el usuario proporcione funciones:
  - **Cmp** → compara claves.
  - **Actualizar** → actualiza un valor existente.
  - **Accion** → ejecuta algo sobre (clave, info).

---

# 📁 Procesador de Texto

## 📄 Lista de Archivos

Los archivos del procesador de texto se encuentra en la siguiente carpeta:

~~~
ProcesadorTexto/
├── procesadorTexto.c // Implementación de funciones de análisis de texto
└── procesadorTexto.h // Definición de constantes, estructuras y prototipos
~~~

## 🧠 ¿Qué es este Procesador de Texto?

El **Procesador de Texto** es un módulo diseñado para analizar el contenido de un archivo `.txt` y extraer de él información estadística relevante de forma automática. Su objetivo es recorrer el texto línea por línea y detectar:

- ✅ Cantidad de **palabras** que hay en total.
- ✅ Cuántos **espacios y tabulaciones** contiene.
- ✅ Cuántos **signos de puntuación** aparecen.
- ✅ La **frecuencia exacta** de cada palabra utilizada.
- ✅ Cuáles son las **palabras más repetidas** (top 5).

## ⚙️ Estructura y Primitivas

El módulo utiliza una estructura llamada `tEstText` (abreviatura de "Estadísticas de Texto"), que actúa como acumulador de datos globales mientras se procesa el archivo.

Esta estructura se pasa por referencia a lo largo del procesamiento, y se actualiza automáticamente conforme se analiza el texto.

~~~c
typedef struct
{
    int cantPalabras;                 /// Cantidad total de palabras encontradas
    int cantEspacios;                 /// Cantidad total de espacios y tabulaciones
    int cantPuntuacion;               /// Cantidad total de signos de puntuación
    char masUsadas[TOP_PAL][TAM_PAL]; /// Podio: palabras más frecuentes en el texto
} tEstText;
~~~

A continuación se listan las funciones principales expuestas por el procesador de texto:

| Función                | Descripción                                                                  |
| ---------------------- | ---------------------------------------------------------------------------- |
| `iniEstadisticas`      | Inicializa todos los contadores de `tEstText` en cero y limpia el podio.     |
| `procesarArch`         | Procesa todo el archivo `.txt`, actualizando el diccionario y estadísticas.  |
| `trozarLinea`          | Separa una línea en palabras, las normaliza y las inserta en el diccionario. |
| `strToLower`           | Convierte una cadena a minúsculas (sin usar `<ctype.h>`).                    |
| `acumularFrecuencia`   | Función de actualización: incrementa el contador de una palabra repetida.    |
| `compararPalabras`     | Función de comparación alfabética de claves tipo `char*`.                    |
| `mostrarPalabra`       | Imprime una palabra y su frecuencia (usada en `recorrerDic`).                |
| `generarPodioPalabras` | Calcula las 5 palabras más frecuentes usando el contenido del diccionario.   |
| `palabraYaEnPodio`     | Verifica si una palabra ya fue incluida en el podio para evitar duplicados.  |
| `mostrarEstadisticas`  | Muestra por consola los resultados finales del análisis textual.             |

Cada una de estas funciones cumple un rol específico y colabora con el TDA Diccionario para completar el análisis de forma modular y reutilizable.

## 🧠 Flujo de Ejecución

### 📥 1. Lectura del archivo (procesarArch)

~~~c
int procesarArch(FILE* arch, tDic* dic, tEstText* estText);
~~~

El procesamiento comienza en la función `procesarArch`, que recibe un `FILE*` ya abierto. Esta función recorre el archivo utilizando `fgets`, leyendo una línea por vez.

Cada línea leída es enviada a `trozarLinea`, la cual realiza el análisis real del contenido textual.

### ✂️ 2. Trozado de la línea (trozarLinea)

~~~c
void trozarLinea(char* linea, tDic* dic, tEstText* estText);
~~~

La función `trozarLinea` recorre la línea carácter por carácter:

1. Detecta letras válidas (`ES_LETRA`) y arma palabras.
2. Al encontrar un separador (espacio, puntuación, fin de línea), termina la palabra.
3. Antes de insertar la palabra, la **normaliza** (convierte a minúsculas con `strToLower`).
4. Luego la **inserta en el diccionario**:
   1. Si ya existe → se incrementa su contador (`acumularFrecuencia`).
   2. Si no existe → se agrega con valor 1.

Además:

- Si detecta un espacio o tabulación → incrementa `cantEspacios`.
- Si detecta un signo de puntuación → incrementa `cantPuntuacion`.
- Cada palabra encontrada incrementa `cantPalabras`.

### 🧠 3. Almacenamiento en el diccionario (trozarLinea)

Cada palabra se almacena como **clave** en el TDA Diccionario. El valor asociado es un entero que representa cuántas veces fue encontrada en el texto.

Gracias a la estructura hash con listas, el acceso es rápido y eficiente incluso con muchas palabras.

### 🏆 4. Generación del podio (generarPodioPalabras)

~~~c
void generarPodioPalabras(tDic* dic, int posicion, tEstText* estText);
~~~

Luego del análisis completo, se selecciona las 5 (`TOP_PAL`) palabras más utilizadas:

1. Recorre todas las listas de la tabla hash.
2. Compara los contadores y selecciona la palabra con mayor frecuencia.
3. Se asegura de no repetir palabras ya elegidas (`palabraYaEnPodio`).
4. Repite el proceso recursivamente hasta llenar el podio.

Las palabras del podio se almacenan en el arreglo `masUsadas` dentro de `tEstText`.

### 📊 5. Visualización de resultados

~~~c
void mostrarEstadisticas(const tEstText* estText, const tDic* dic);
~~~

Finalmente, `mostrarEstadisticas` imprime por consola:

- Cantidad total de palabras.
- Cantidad de espacios y signos de puntuación.
- Las 5 palabras más utilizadas.
- Todas las palabras encontradas junto con su frecuencia (`mostrarPalabra`).

# 🧩 Interfaz

## 📄 Lista de Archivos

El módulo de Interfaz está compuesto por los siguientes archivos, ubicados en la carpeta:

~~~
Interfaz/
├── interfaz.c      // Implementación de funciones de entrada/salida e interacción con el usuario
└── interfaz.h      // Prototipos, constantes de entrada y documentación
~~~

## 🧠 ¿Qué es y para qué sirve el módulo Interfaz?

Este módulo representa el **punto de contacto entre el usuario y el sistema**. Es el encargado de solicitar la ruta del archivo a analizar, validar que el archivo sea correcto, y coordinar la ejecución de los distintos módulos: diccionario, procesador de texto y visualización de resultados. Este módulo se encarga de toda la **interacción con el usuario**, guiándolo para que el uso del programa sea claro y sin errores. Actúa como un "puente" entre la lógica interna del sistema y el entorno externo.

Es responsable de:

- Mostrar un mensaje introductorio con las funcionalidades.
- Solicitar correctamente la ruta del archivo a analizar.
- Validar que se trate de un archivo `.txt` válido y no vacío.
- Encadenar las funciones del TDA Diccionario y del Procesador de Texto para ejecutar el análisis.
- Mostrar la salida de forma clara al usuario por consola.

## ⚙️ Primitivas

| Función                   | Descripción                                                                                                                  |
|---------------------------|------------------------------------------------------------------------------------------------------------------------------|
| `mostrarInstrucciones()`  | Muestra por consola una explicación del programa, sus funcionalidades y requisitos.                                          |
| `iniciarAnalisisTexto()`  | Función principal que guía al usuario en el uso del programa y coordina el análisis del archivo.                             |
| `validarArchivoTxt()`     | Verifica que el archivo exista, tenga extensión `.txt` y no esté vacío. Devuelve un puntero al archivo abierto si es válido. |

## Flujo de Ejecución

1. **Presentación inicial**: `mostrarInstrucciones()` imprime una introducción clara para el usuario.
2. **Ingreso de archivo**: `iniciarAnalisisTexto()` solicita al usuario que ingrese la ruta del archivo a analizar.
3. **Validación**: Se invoca `validarArchivoTxt()` para asegurarse de que el archivo es correcto.
4. **Inicialización de estructuras**: Se crean el diccionario y la estructura de estadísticas.
5. **Procesamiento y visualización**:
   - Se llama a `procesarArch()` del módulo de texto.
   - Se genera el podio.
   - Se muestran estadísticas con `mostrarEstadisticas()`.
6. **Limpieza**: Se libera memoria utilizada y se cierra el archivo.

### 🪧 1. Muestra las instrucciones iniciales (mostrarInstrucciones)

~~~c
void mostrarInstrucciones();
~~~

La ejecución comienza llamando a la función `mostrarInstrucciones()`, que imprime un mensaje en consola con:

- Una descripción general del programa.
- Las funcionalidades disponibles.
- Los requisitos del archivo de entrada.
- El tipo de salida esperada.

### 🧾 2. Solicita la ruta del archivo (iniciarAnalisisTexto)

~~~c
void iniciarAnalisisTexto();
~~~

A través de la función `iniciarAnalisisTexto()`:

- Se solicita al usuario que ingrese la ruta del archivo `.txt`.
- Se lee la entrada desde `stdin` usando `fgets`.
- Se elimina el salto de línea final para trabajar con la ruta limpia.

### 🛡️ 3. Valida el archivo (validarArchivoTxt)

~~~c
int validarArchivoTxt(const char* ruta, FILE** arch);
~~~

Antes de continuar, se llama a `validarArchivoTxt()` para verificar tres condiciones:

1. Que el archivo **exista** y se pueda abrir.
2. Que tenga **extensión `.txt`**.
3. Que **no esté vacío**.

Si cualquiera de esas condiciones falla, se imprime un mensaje de error y la ejecución se detiene.

### 🔁 4. Coordina el procesamiento (iniciarAnalisisTexto)

~~~c
void iniciarAnalisisTexto();
~~~

Si el archivo es válido:

1. Se inicializa el TDA Diccionario (`dicc`).
2. Se inicializa la estructura de estadísticas (`tEstText`).
3. Se llama a `procesarArch()` para analizar todo el archivo.
4. Se genera el podio de palabras con `generarPodioPalabras()`.
5. Se muestra el resultado en pantalla con `mostrarEstadisticas()`.

### 🧹 5. Libera recursos

Una vez finalizado el análisis:

- Se libera la memoria del diccionario (`vaciarDic()`).
- Se cierra el archivo con `fclose()`.

---

## 📚 Créditos y Referencias

### 👨‍💻 Autores del proyecto

Este trabajo práctico fue realizado por alumnos de la carrera **Ingeniería en Informática** en la **Universidad Nacional de La Matanza (UNLaM)**, en el marco de la materia **Algoritmos y Estructuras de Datos**.

**Grupo de alumnos:**

- ✏️ Tiago Pujia
- ✏️ Luana Dominguez
- ✏️ Bautista Rios de Gaeta
- ✏️ Ornella Bonachera

📆 **Fecha de entrega:** Junio 2025

🧑‍🏫 **Comisión:** 01-1900 | Cuatrimestre 1C

### 👩‍🏫 Docentes responsables

- **Prof. Adjunto/a:** Federico Pezzola
- **Prof. Adjunto/a:** Brian Jordi
- **Prof. Adjunto/a:** Guatelli Renata

### 🔗 Bibliografía y fuentes consultadas

- [Wikipedia](https://es.wikipedia.org/wiki/Tabla_hash)
- [ChatGpt](https://chatgpt.com/)
- [Contenido de Miel](https://drive.google.com/drive/folders/1oXcMnT-veOcgrTkaG9UGqrK3YF5J0hvY?usp=drive_link)
- [Stack Overflow](https://stackoverflow.com/)