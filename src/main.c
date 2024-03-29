#include "dtan.h"

int main( int argc, char *argv[] ) {
    if(argc < 2) return 0;
#ifdef _WIN32
    DtanObject dtan = DtanNew(argv[1], DtanSJIS);
#elif __linux__ || __APPLE__
    DtanObject dtan = DtanNew(argv[1], DtanUTF8);
#endif
    DtanRun(&dtan);
    return 0;
}
