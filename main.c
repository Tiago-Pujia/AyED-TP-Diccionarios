#include "Interfaz/interfaz.h"
#include "ProcesadorTexto/procesadorTexto.h"
#include "Pruebas/pruebas.h"

int main(int argc, char* argv[])
{
    int test = 0;

    for(int i=1; i<argc; i++)
        test = !strcmp(argv[i], "--testing") ? 1 : 0;


    if(test)
        ejecutarLoteDePruebas();
    else
        iniciarAnalisisTexto();

    return 0;
}
