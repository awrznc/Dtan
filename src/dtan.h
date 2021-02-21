#include <windows.h>

#define APP_NAME "dtan"
#define APP_WIDTH 1000
#define APP_HIGHT 72

#define FONT_SIZE 64
#define FONT_TYPE TEXT("Yu Mincho")

// period for increment. (N miri second)
#define UPDATE_PERIOD 10
#define ALPHA_MIN_SIZE 0
#define ALPHA_MAX_SIZE 255
#define PROP_KEY TEXT("dtan_object")

#include<stdio.h>
static void char_to_tchar(TCHAR* tch, const char* ch) {
    #ifdef UNICODE
    MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, ch, strlen(ch), tch, (sizeof tch)/2);
    #else
    strcpy(tch,ch);
    #endif
}

static int utf8_to_sjis(char* sjis, const char* utf8) {
    int unicode_lenght = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, utf8, -1, NULL, 0);
    wchar_t* unicord_char = (wchar_t *) calloc(unicode_lenght, sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, utf8, -1, unicord_char, unicode_lenght);

    int sjis_length = WideCharToMultiByte(932, 0, unicord_char, -1, NULL, 0, NULL, NULL);
    if(sjis == NULL) {
        free(unicord_char);
        return sjis_length;
    }
    char* sjis_temporary = (char *) calloc(sjis_length, sizeof(char));
    WideCharToMultiByte(932, 0, unicord_char, -1, sjis_temporary, sjis_length, NULL, NULL);
    strcpy(sjis, sjis_temporary);

    free(unicord_char);
    free(sjis_temporary);
    return sjis_length;
}

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

#include <memory.h>
#include <locale.h>
#include <math.h>

static LRESULT CALLBACK window_procedure(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
) {
    DtanObject* dtan_object = (DtanObject*)GetProp(hwnd, PROP_KEY);

    if (uMsg == WM_CREATE) {
        SetTimer(hwnd, 1, UPDATE_PERIOD, NULL);
        return 0;

    } else if(uMsg == WM_PAINT) {

        // font
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HFONT font_handler = CreateFont(
            FONT_SIZE, 0, 0, 0,
            FW_BOLD, FALSE, TRUE, FALSE,
            SHIFTJIS_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            FIXED_PITCH,
            FONT_TYPE );
        SelectObject(hdc, font_handler);
        SetTextAlign(hdc , TA_CENTER | TA_TOP | TA_NOUPDATECP);
        SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
        SetBkMode(hdc, TRANSPARENT); // SetBkColor(hdc, RGB(0,0,0));

        // string
        TCHAR* display_string = NULL;
        if (dtan_object->charset == DtanUTF8) {
            int sjis_size = utf8_to_sjis(NULL, dtan_object->display_string);
            char* sjis = (char *) calloc(sjis_size, sizeof(char));
            utf8_to_sjis(sjis, dtan_object->display_string);
            display_string = (TCHAR *) calloc(strlen(sjis), sizeof(TCHAR));
            char_to_tchar(display_string, sjis);
            free(sjis);
        } else if (dtan_object->charset == DtanSJIS) {
            display_string = (TCHAR *) calloc(strlen(dtan_object->display_string), sizeof(TCHAR));
            char_to_tchar(display_string, dtan_object->display_string);
        }

        TextOut(hdc, GetSystemMetrics(SM_CXSCREEN)/2, 0, (LPCTSTR)display_string, (int)strlen(display_string));

        if (dtan_object->charset == DtanUTF8 || dtan_object->charset == DtanUTF8) {
            free(display_string);
        }

        DeleteObject(font_handler);
        EndPaint(hwnd, &ps);

        return 0;

    } else if(uMsg == WM_TIMER) {

        const DtanPair* current_pair = &(dtan_object->interval[dtan_object->status.current_index]);
        double increment_alpha_size = (double)ALPHA_MAX_SIZE / (double)current_pair->time;

        DtanFade fade = current_pair->fade;
        if( fade != DtanNone) {
            int temporary_alpha = round(dtan_object->status.alpha + (increment_alpha_size * (char)fade));
            if(fade == DtanIn) {
                dtan_object->status.alpha = ALPHA_MAX_SIZE < temporary_alpha ? ALPHA_MAX_SIZE : temporary_alpha;
            } else if(fade == DtanOut) {
                dtan_object->status.alpha = ALPHA_MIN_SIZE > temporary_alpha ? ALPHA_MIN_SIZE : temporary_alpha;
            }
        }

        unsigned int length = sizeof(dtan_object->interval) / sizeof(dtan_object->interval[0]);
        unsigned int max_time = current_pair->time;
        if (dtan_object->status.current_time >= max_time) {
            dtan_object->status.current_index++;
            dtan_object->status.current_time = 0;
            if( !(dtan_object->status.current_index < length) ) SendMessage( hwnd, WM_CLOSE, 0, 0 );
        }
        dtan_object->status.current_time++;

        SetLayeredWindowAttributes(hwnd, RGB(0xFE, 0xFE, 0xFE), (BYTE)dtan_object->status.alpha, LWA_COLORKEY | LWA_ALPHA);
        return 0;

    } else if(uMsg == WM_DESTROY) {
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int DtanRun(DtanObject* dtanObject) {
    HINSTANCE instance_handle = (HINSTANCE)GetModuleHandle(0);

    WNDCLASSEX window_class_ex = { 0 };
    window_class_ex.cbSize        = sizeof(WNDCLASSEX);
    window_class_ex.style         = CS_HREDRAW | CS_VREDRAW;
    window_class_ex.lpfnWndProc   = window_procedure;
    window_class_ex.cbClsExtra    = 0;
    window_class_ex.cbWndExtra    = 0;
    window_class_ex.hInstance     = instance_handle;
    window_class_ex.hIcon         = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
    window_class_ex.hCursor       = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    window_class_ex.hbrBackground = CreateSolidBrush( RGB( 0xFE, 0xFE, 0xFE ) );
    window_class_ex.lpszMenuName  = NULL;
    window_class_ex.lpszClassName = APP_NAME;
    window_class_ex.hIconSm       = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);

    if (RegisterClassEx(&window_class_ex) == 0) return 0;

    unsigned int app_width = GetSystemMetrics(SM_CXSCREEN);
    unsigned int app_position_x = 0;
    unsigned int app_position_y = GetSystemMetrics(SM_CYSCREEN)/2-FONT_SIZE;
    HWND window_handler = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT,
        APP_NAME, APP_NAME, WS_POPUP,
        app_position_x, app_position_y,
        app_width, APP_HIGHT,
        NULL, NULL, instance_handle, NULL
    );

    SetProp(window_handler, PROP_KEY, (HANDLE)dtanObject);

    if (window_handler == NULL) return 0;

    int cmd_show = SW_RESTORE;
    ShowWindow(window_handler, cmd_show);
    UpdateWindow(window_handler);

    MSG message = { 0 };
    while (GetMessage(&message, NULL, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    RemoveProp(window_handler, PROP_KEY);

    return (int)message.wParam;
}
