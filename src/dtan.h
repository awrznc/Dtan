#ifndef __FUNCTION_H_INCLUDED_
#define __FUNCTION_H_INCLUDED_

#define FONT_SIZE 64

// period for increment. (N miri second)
#define UPDATE_PERIOD 10
#define ALPHA_MIN_SIZE 0
#define ALPHA_MAX_SIZE 255

enum dtan_charset {
    DtanSJIS = 0,
    DtanUTF8 = 1
} typedef DtanCharset;

enum dtan_fade {
    DtanIn = 1,
    DtanNone = 0,
    DtanOut = -1
} typedef DtanFade;

struct dtan_pair {
    unsigned int time;
    DtanFade fade;
} typedef DtanPair;

typedef const DtanPair DtanInterval[3];

struct dtan_status {
    unsigned char alpha;
    unsigned int current_index;
    unsigned int current_time;
} typedef DtanStatus;

struct dtan_object {
    const char* display_string;
    const DtanCharset charset;
    const DtanInterval interval;
    DtanStatus status;
} typedef DtanObject;

DtanObject DtanNew(const char* displayString, DtanCharset charset);

int DtanRun(DtanObject* dtanObject);

#endif
