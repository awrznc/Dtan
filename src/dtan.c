#include "dtan.h"

DtanObject DtanNew(const char* displayString, DtanCharset charset) {
    DtanObject dtan_object = {
        displayString,
        charset,
        {
            { 30, DtanIn },
            { 50, DtanNone },
            { 30, DtanOut }
        },
        { ALPHA_MIN_SIZE, 0, 0 }
    };
    return dtan_object;
}
