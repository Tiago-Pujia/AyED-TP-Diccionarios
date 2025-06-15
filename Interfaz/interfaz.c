#include "interfaz.h"


// =======================================================
//                  FUNCIONES PRINCIPALES
// =======================================================

/// Función principal de interacción con el usuario.
/// Solicita la ruta del archivo, valida y procesa su contenido, y muestra resultados.
void iniciarAnalisisTexto()
{
    char ruta[TAM_RUTA];
    FILE* arch;
    tDic dicc;
    tLista listaPodio;
    tEstText estadisticas;

    int continuar = 1;
    int validacionArch;

    do
    {
        limpiarConsola();
        mostrarInstrucciones();

        printf("Ingrese la ruta del archivo de texto que desea analizar:\n");
        leerTexto(ruta, TAM_RUTA);

        if ((validacionArch = validarArchivoTxt(ruta, &arch)) != ARCHIVO_VALIDO)
        {
            printf(MSJ_CONTINUAR);
            scanf("%d", &continuar);
            continue;
        }

        crearDic(&dicc);

        iniEstadisticas(&estadisticas);

        procesarArch(arch, &dicc, &estadisticas);

        crearListaDesdeDicc(&dicc, cmpInfo, &listaPodio);

        mostrarEstadisticas(&estadisticas, &dicc, &listaPodio);

        destruirLista(&listaPodio);

        vaciarDic(&dicc);
        fclose(arch);

        printf(MSJ_CONTINUAR);
        scanf("%d", &continuar);

    } while(continuar != 0);
}

/// Valida que el archivo exista, su extension sea ".txt" y que no este vacio.
/// Si la validación es exitosa, devuelve 0 y el puntero al archivo.
int validarArchivoTxt(const char* ruta_archivo, FILE** pArchivo)
{
    FILE* archivo = fopen(ruta_archivo, "r");

    // Valido que el archivo exista en la ruta ingresada
    if(!archivo)
    {
        printf("El archivo ingresado no existe o no fue encontrado en la ruta especificada.\n");
        return ERROR_ARCHIVO_NO_ENCONTRADO;
    }

    // Valido que el archi2vo tenga la extension ".txt"
    const char* ext = strrchr(ruta_archivo, '.');
    if (!ext || strcmp(ext, ".txt") != 0)
    {
        printf("Por favor ingrese un archivo de texto.\n");
        fclose(archivo);
        return ERROR_EXTENSION_INVALIDA;
    }

    // Valido que el archivo NO este vacio
    fseek(archivo, 0, SEEK_END);
    long tam = ftell(archivo);
    rewind(archivo);

    if (tam == 0)
    {
        printf("Por favor ingrese un archivo que no este vacio.\n");
        fclose(archivo);
        return ERROR_ARCHIVO_VACIO;
    }

    *pArchivo = archivo;

    return ARCHIVO_VALIDO;
}

/// Muestra por consola una descripción detallada del programa y sus funcionalidades.
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
void mostrarEstadisticas(const tEstText* estText, const tDic* dic, tLista* listaPodio)
{
    printf("============================================================\n");
    printf("                 ESTADISTICAS DEL TEXTO                     \n");
    printf("============================================================\n");
    printf(" Palabras totales encontradas       : %d\n", estText->cantPalabras);
    printf(" Espacios y tabulaciones            : %d\n", estText->cantEspacios);
    printf(" Signos de puntuacion               : %d\n", estText->cantPuntuacion);

    printf("\n------------------------------------------------------------\n");
    printf(" TOP %d PALABRAS MAS UTILIZADAS\n", TOP_PAL);
    printf("------------------------------------------------------------\n\n");

    imprimirPodioPalabrasLista(listaPodio, TOP_PAL, cmpInfo, imprimirPalabra);

    printf("\n------------------------------------------------------------\n");
    printf(" LISTADO COMPLETO DE PALABRAS Y FRECUENCIAS\n");
    printf("------------------------------------------------------------\n");

    recorrerDic(dic, mostrarPalabra, NULL);

    printf("============================================================\n");
}

// =======================================================
//                  FUNCIONES AUXILIARES
// =======================================================


void imprimirPalabra(void* elem, void* param, void* v)
{
    tNodo* pal = (tNodo*)elem;
    int* frecuencia = (int*)pal->info;
    char* palabra = (char*)pal->clave;

    printf("\t~ %s - %d aparicion(es)\n", palabra, *frecuencia);
}

/// Lee una línea de texto desde la entrada estándar y elimina el salto de línea final.
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

/// Limpia la consola, dependiendo del sistema operativo.
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
