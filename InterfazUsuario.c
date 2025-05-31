#include "InterfazUsuario.h"
#include "Diccionario.h"

void mostrarInterfaz()
{
    char ruta_archivo[MAX_RUTA];
    FILE* archivo = NULL;
    int archivo_valido;
    // tDiccionario dicc;

    imprimir_especificacion();

    printf("Ingresa a continuacion la ruta del archivo de texto que queres analizar\n");
    if (fgets(ruta_archivo, MAX_RUTA, stdin) == NULL)
    {
        printf("Ocurrio un error al leer la ruta del archivo.\n");
        return;
    }

    size_t len = strlen(ruta_archivo);
    if (len > 0 && ruta_archivo[len - 1] == '\n')
        ruta_archivo[len - 1] = '\0';

    // PENDIENTE: consultar si se deja asi o se ejecuta hasta que el usuario corte la ejecucion o ingrese un archivo valido
    archivo_valido = validar_archivo(ruta_archivo, &archivo);
    if (archivo_valido != 0)
        return;


    // crear_dic(&dicc);

    // procesarArchivo(archivo);

    // vaciar_dic(&dicc);

    fclose(archivo);

}



int validar_archivo(const char* ruta_archivo, FILE** pArchivo)
{
    FILE* archivo = fopen(ruta_archivo, "r");

    // Valido que el archivo exista en la ruta ingresada
    if(!archivo)
    {
        printf("El archivo ingresado no existe o no fue encontrado en la ruta especificada.\n");
        return 1;
    }

    // Valido que el archivo tenga la extension ".txt"
    const char* ext = strrchr(ruta_archivo, '.');
    if (!ext || strcmp(ext, ".txt") != 0)
    {
        printf("Por favor ingrese un archivo de texto.\n");
        fclose(archivo);
        return 2;
    }

    // Valido que el archivo NO este vacio
    fseek(archivo, 0, SEEK_END);
    long tam = ftell(archivo);
    rewind(archivo);

    if (tam == 0)
    {
        printf("Por favor ingrese un archivo que no este vacio.\n");
        fclose(archivo);
        return 3;
    }

    *pArchivo = archivo;

    return 0;
}

void imprimir_especificacion()
{
    printf("------------------------------------------------------------\n");
    printf("        ANALIZADOR DE TEXTO - PROCESADOR DE ARCHIVOS        \n");
    printf("------------------------------------------------------------\n");
    printf("Este programa permite analizar un archivo de texto (.txt) y obtener\n");
    printf("informacion detallada sobre su contenido.\n");

    printf("FUNCIONALIDADES:\n");
    printf("  - Cuenta la cantidad total de palabras.\n");
    printf("  - Cuenta los espacios y signos de puntuacion.\n");
    printf("  - Muestra cuantas veces aparece cada palabra.\n");
    printf("  - Genera un podio con las 5 palabras más utilizadas.\n\n");

    printf("REQUISITOS DEL ARCHIVO:\n");
    printf("  - Debe tener extension .txt (archivo de texto plano).\n");
    printf("  - No debe estar vacio.\n\n");
    printf("  NOTA: si el archivo está en la misma carpeta que este programa, coloca la ruta relativa. Caso contrario, coloca la ruta absoluta (ej: C:\\ruta\\archivo.txt) \n\n");

    printf("SALIDA ESPERADA:\n");
    printf("  - Informe detallado impreso por pantalla con todos los datos obtenidos.\n\n");
}
