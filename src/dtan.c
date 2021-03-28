#include <math.h>

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

int DtanUpdateStatus(DtanObject* dtanObject) {
    const DtanPair* current_pair = &(dtanObject->interval[dtanObject->status.current_index]);
    double increment_alpha_size = (double)ALPHA_MAX_SIZE / (double)current_pair->time;

    DtanFade fade = current_pair->fade;
    if( fade != DtanNone) {
        int temporary_alpha = (int)round(dtanObject->status.alpha + (increment_alpha_size * (char)fade));
        if(fade == DtanIn) {
            dtanObject->status.alpha = ALPHA_MAX_SIZE < temporary_alpha ? ALPHA_MAX_SIZE : temporary_alpha;
        } else if(fade == DtanOut) {
            dtanObject->status.alpha = ALPHA_MIN_SIZE > temporary_alpha ? ALPHA_MIN_SIZE : temporary_alpha;
        }
    }

    unsigned int max_time = current_pair->time;
    if (dtanObject->status.current_time >= max_time) {
        dtanObject->status.current_index++;
        dtanObject->status.current_time = 0;
        unsigned int length = sizeof(dtanObject->interval) / sizeof(dtanObject->interval[0]);
        if( !(dtanObject->status.current_index < length) ) return 0;
    }
    dtanObject->status.current_time++;
    return 1;
}
