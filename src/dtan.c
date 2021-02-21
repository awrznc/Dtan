#include "dtan.h"

int main( int argc, char *argv[] ) {
    if(argc < 2) return 0;
    DtanObject dtan = DtanNew(argv[1], DtanSJIS);
    DtanRun(&dtan);
    return 0;
}
