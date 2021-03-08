#include "dtan.h"

int main( int argc, char *argv[] ) {
    if(argc < 2) return 0;
#ifdef _WINDOWS
    DtanObject dtan = DtanNew(argv[1], DtanSJIS);
#elif __APPLE__
    DtanObject dtan = DtanNew(argv[1], DtanUTF8);
#endif
    DtanRun(&dtan);
    return 0;
}
