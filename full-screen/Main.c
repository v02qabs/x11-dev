#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TEXT "Hello"
#define FONT "-misc-fixed-*-*-*-*-20-*-*-*-*-*-*-*"

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;
    XFontStruct *font;
    GC gc;

    // X サーバーに接続
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "X server に接続できません\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);

    // ウィンドウを作成
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, width, height, 0,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    // フルスクリーンに設定
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    XChangeProperty(display, window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&fullscreen, 1);

    // ウィンドウを表示
    XMapWindow(display, window);

    // 描画のための GC (Graphics Context) を作成
    gc = XCreateGC(display, window, 0, NULL);
    font = XLoadQueryFont(display, FONT);
    if (font) {
        XSetFont(display, gc, font->fid);
    }
    XSetForeground(display, gc, BlackPixel(display, screen));

    // イベントループ
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            int text_width = XTextWidth(font, TEXT, strlen(TEXT));
            int x = (width - text_width) / 2;
            int y = height / 2;
            XDrawString(display, window, gc, x, y, TEXT, strlen(TEXT));
        }
    }

    // クリーンアップ
    XUnloadFont(display, font->fid);
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}

