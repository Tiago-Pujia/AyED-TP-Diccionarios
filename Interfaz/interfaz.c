#include "interfaz.h"
#include "../TDA_Diccionario/dic.h"
#include "../ProcesadorTexto/procesadorTexto.h"

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================

/// Limpia la consola, dependiendo del sistema operativo.
///
/// Compatible con:
/// - Windows (cls)
/// - Linux/macOS (clear)
void limpiarConsola()
{
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");  // Comando para Windows
    #elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
        system("clear");  // Comando para Linux/macOS
    #else
        puts("Sistema operativo no soportado para limpieza de consola.");
    #endif
}

/// Lee una línea de texto desde la entrada estándar y elimina el salto de línea final.
///
/// @param texto     Buffer donde se guarda el texto leído.
/// @param longitud  Tamaño máximo del buffer.
void leerTexto(char *texto, size_t longitud)
{
    size_t i = 0;
    fflush(stdin);
    fgets(texto, longitud, stdin);
    while(texto[i] != '\0')
        texto[i] == '\n' ? texto[i] = '\0' : i++;
}

// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

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
    char ruta[TAM_RUTA];           // Ruta ingresada por el usuario
    FILE* arch;                    // Puntero al archivo
    tDic dicc;                     // Diccionario de palabras
    tEstText estadisticas;         // Estructura para estadísticas del texto

    char continuar = 1;
    char validacionArch;

    do
    {
        limpiarConsola();

        // Inicializamos el diccionario y contadores
        crearDic(&dicc);
        iniEstadisticas(&estadisticas);

        mostrarInstrucciones();  // Instrucciones y presentación

        printf("Ingrese la ruta del archivo de texto que desea analizar:\n");

        // Leemos la ruta ingresada por el usuario
        leerTexto(ruta, TAM_RUTA);

        // Validamos el archivo (existe, es .txt, no está vacío)
        if ((validacionArch = validarAbrirArchTxt(ruta, &arch)) != EXITO)
        {
            mostrarValidacionArch(validacionArch);
            printf(MSJ_CONTINUAR);
            scanf("%d", &continuar);
            continue;
        }

        // Procesamos el contenido del archivo y generamos estadísticas
        procesarArch(arch, &dicc, &estadisticas);
        generarPodioPalabras(&dicc, TOP_PAL, &estadisticas);
        limpiarConsola();
        mostrarEstadisticas(&estadisticas, &dicc);

        vaciarDic(&dicc); // Liberamos la memoria ocupada por el diccionario
        fclose(arch);  // Cerramos el archivo abierto

        printf(MSJ_CONTINUAR);
        scanf("%d", &continuar);

    } while(continuar);
}

/// Muestra un mensaje descriptivo según el tipo de error al validar un archivo.
/// @param tipoError Código del error:
///        1 = No se pudo abrir,
///        2 = Extensión inválida,
///        3 = Archivo vacío
void mostrarValidacionArch(int tipoError)
{
    printf("\n[!] ERROR: ");

    switch(tipoError)
    {
        case ERROR_ARCHIVO_NO_ENCONTRADO:
            printf("El archivo no existe o no se pudo abrir.\n");
            break;

        case ERROR_EXTENSION_INVALIDA:
            printf("El archivo debe tener extensión .txt\n");
            break;

        case ERROR_ARCHIVO_VACIO:
            printf("El archivo está vacío. Por favor, seleccioná uno con contenido.\n");
            break;

        default:
            printf("Error desconocido al validar el archivo.\n");
            break;
    }
}

/// Muestra por consola una descripción detallada del programa y sus funcionalidades.
///
/// Esta función presenta al usuario qué hace el programa, cómo debe proporcionar
/// el archivo de entrada, y qué tipo de salida puede esperar.
void mostrarInstrucciones()
{
    printf("============================================================\n");
    printf("        ANALIZADOR DE TEXTO - PROCESADOR DE ARCHIVOS .TXT   \n");
    printf("============================================================\n\n");

    printf("DESCRIPCION:\n");
    printf("Este programa permite analizar un archivo de texto plano (.txt)\n");
    printf("y generar un informe estadistico completo sobre su contenido.\n\n");

    printf("FUNCIONALIDADES:\n");
    printf("  - Cuenta la cantidad total de palabras.\n");
    printf("  - Cuenta los espacios y signos de puntuacion.\n");
    printf("  - Muestra cuantas veces aparece cada palabra.\n");
    printf("  - Genera un podio con las 5 palabras mas utilizadas.\n\n");

    printf("REQUISITOS DEL ARCHIVO:\n");
    printf("  - Debe ser un archivo de texto plano con extension .txt\n");
    printf("  - No debe estar vacio\n\n");

    printf("NOTA:\n");
    printf("  - Si el archivo esta en la misma carpeta que el programa,\n");
    printf("    puedes usar una ruta relativa (ej: archivo.txt)\n");
    printf("  - Si esta en otra ubicacion, usa la ruta completa\n");
    printf("    (ej: C:\\\\Usuarios\\\\TuNombre\\\\Documentos\\\\archivo.txt)\n\n");

    printf("SALIDA ESPERADA:\n");
    printf("  - Informe detallado impreso por pantalla con:\n");
    printf("      * Estadisticas generales del texto\n");
    printf("      * Palabras mas frecuentes\n");
    printf("      * Frecuencia de todas las palabras encontradas\n");

    printf("============================================================\n\n");
}

/// Muestra las estadísticas recolectadas del archivo de texto procesado.
///
/// Incluye:
/// - Cantidad total de palabras
/// - Cantidad de espacios y signos de puntuación
/// - Podio con las palabras más frecuentes
/// - Listado completo de palabras con sus frecuencias
///
/// @param estText  Estructura con estadísticas acumuladas
/// @param dic      Diccionario con las palabras y sus repeticiones
void mostrarEstadisticas(const tEstText* estText, const tDic* dic)
{
    printf("============================================================\n");
    printf("                 ESTADISTICAS DEL TEXTO                     \n");
    printf("============================================================\n");
    printf(" Palabras totales encontradas       : %d\n", estText->cantPalabras);
    printf(" Espacios y tabulaciones            : %d\n", estText->cantEspacios);
    printf(" Signos de puntuacion               : %d\n", estText->cantPuntuacion);

    printf("\n------------------------------------------------------------\n");
    printf(" TOP %d PALABRAS MAS UTILIZADAS\n", TOP_PAL);
    printf("------------------------------------------------------------\n");

    for (int i = TOP_PAL - 1; i >= 0; i--)
        if (estText->masUsadas[i][0] != '\0')
            printf(" %d. %s\n", TOP_PAL - i, estText->masUsadas[i]);

    printf("\n------------------------------------------------------------\n");
    printf(" LISTADO COMPLETO DE PALABRAS Y FRECUENCIAS\n");
    printf("------------------------------------------------------------\n");

    recorrerDic(dic, mostrarPalabra, NULL);

    printf("============================================================\n");
}
