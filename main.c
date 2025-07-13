#include "Interfaz/interfaz.h"
#include "ProcesadorTexto/procesadorTexto.h"
#include "Pruebas/pruebas.h"

int main(int argc, char* argv[])
{
    int test = 0;
    int i = 0;

    while(i < argc && !test)
    {
        if(!strcmp(argv[i],"--testing"))
            test = 1;
        i++;
    }

    test ? ejecutarLoteDePruebas() : iniciarAnalisisTexto();

    return 0;
}
