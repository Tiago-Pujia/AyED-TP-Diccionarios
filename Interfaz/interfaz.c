#include "interfaz.h"
#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

/// Función principal de interacción con el usuario.
/// Solicita la ruta del archivo, valida y procesa su contenido, y muestra resultados.
///
/// Flujo general:
/// 1. Muestra las instrucciones.
/// 2. Solicita la ruta del archivo.
/// 3. Valida existencia, extensión y contenido.
/// 4. Procesa el texto y genera estadísticas.
/// 5. Muestra los resultados.
/// 6. Libera memoria utilizada.
void iniciarAnalisisTexto()
{
    char ruta[MAX_RUTA];           // Ruta ingresada por el usuario
    FILE* arch = NULL;             // Puntero al archivo
    int estadoValidacion;         // Resultado de validarArchivoTxt

    tDic dicc;                     // Diccionario de palabras
    tEstText estadisticas;        // Estructura para estadísticas del texto

    iniEstadisticas(&estadisticas);  // Inicializamos contadores y podio

    mostrarInstrucciones();  // Instrucciones y presentación

    printf("Ingrese la ruta del archivo de texto que desea analizar:\n");

    // Leemos la ruta ingresada por el usuario
    if (fgets(ruta, MAX_RUTA, stdin) == NULL)
    {
        printf("❌ Ocurrió un error al leer la ruta del archivo.\n");
        return;
    }

    // Eliminamos el salto de línea final si está presente
    size_t len = strlen(ruta);
    if (len > 0 && ruta[len - 1] == '\n')
        ruta[len - 1] = '\0';

    // Validamos el archivo (existe, es .txt, no está vacío)
    estadoValidacion = validarArchivoTxt(ruta, &arch);
    if (estadoValidacion != 0)
        return;  // Finaliza si el archivo no es válido

    // Inicializamos el diccionario
    crearDic(&dicc);

    // Procesamos el contenido del archivo y generamos estadísticas
    procesarArch(arch, &dicc, &estadisticas);
    generarPodioPalabras(&dicc, TOP_PAL, &estadisticas);

    // Mostramos las estadísticas recolectadas
    mostrarEstadisticas(&estadisticas, &dicc);

    // Liberamos la memoria ocupada por el diccionario
    vaciarDic(&dicc);

    fclose(arch);  // Cerramos el archivo abierto
}


/// Valida que el archivo especificado sea accesible, tenga extensión .txt y no esté vacío.
///
/// @param ruta   Ruta str del archivo a validar
/// @param arch   Puntero al puntero de archivo donde se almacenará el FILE* abierto
///
/// @return 0 si es válido.
///         1 si el archivo no existe o no se pudo abrir.
///         2 si la extensión no es .txt.
///         3 si el archivo está vacío.
int validarArchivoTxt(const char* ruta, FILE** arch)
{
    FILE* archivo = fopen(ruta, "r");

    // Verifica que el archivo exista y se pueda abrir
    if (!archivo)
    {
        printf("❌ El archivo no existe o no se pudo abrir.\n");
        return 1;
    }

    // Verifica que tenga extensión .txt
    const char* extension = strrchr(ruta, '.');
    if (!extension || strcmp(extension, ".txt") != 0)
    {
        printf("❌ El archivo debe tener extensión .txt\n");
        fclose(archivo);
        return 2;
    }

    // Verifica que el archivo no esté vacío
    fseek(archivo, 0, SEEK_END);
    long tamanio = ftell(archivo);
    rewind(archivo);

    if (tamanio == 0)
    {
        printf("❌ El archivo está vacío. Por favor, seleccioná uno con contenido.\n");
        fclose(archivo);
        return 3;
    }

    // Si todo está correcto, devolvemos el archivo abierto
    *arch = archivo;
    return 0;
}

/// Muestra por consola una descripción detallada del programa y sus funcionalidades.
///
/// Esta función presenta al usuario qué hace el programa, cómo debe proporcionar
/// el archivo de entrada, y qué tipo de salida puede esperar.
void mostrarInstrucciones()
{
    printf("------------------------------------------------------------\n");
    printf("        ANALIZADOR DE TEXTO - PROCESADOR DE ARCHIVOS        \n");
    printf("------------------------------------------------------------\n");
    printf("Este programa permite analizar un archivo de texto (.txt) y obtener\n");
    printf("información detallada sobre su contenido.\n");

    printf("FUNCIONALIDADES:\n");
    printf("  - Cuenta la cantidad total de palabras.\n");
    printf("  - Cuenta los espacios y signos de puntuación.\n");
    printf("  - Muestra cuántas veces aparece cada palabra.\n");
    printf("  - Genera un podio con las 5 palabras más utilizadas.\n\n");

    printf("REQUISITOS DEL ARCHIVO:\n");
    printf("  - Debe ser un archivo de texto plano (.txt).\n");
    printf("  - No debe estar vacío.\n\n");
    printf("  NOTA: Si el archivo está en la misma carpeta que este programa, usá la ruta relativa.\n");
    printf("        Caso contrario, colocá la ruta absoluta (ej: C:\\\\ruta\\\\archivo.txt)\n\n");

    printf("SALIDA ESPERADA:\n");
    printf("  - Informe detallado impreso por pantalla con todos los datos obtenidos.\n\n");
}

