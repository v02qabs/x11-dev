#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    Display *d;
    Window w;
    XEvent e;
    int s;

    // Xサーバーに接続
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    s = DefaultScreen(d);

    // ウィンドウを作成
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 500, 300, 1,
                            BlackPixel(d, s), WhitePixel(d, s));

    // イベントを受け取る設定
    XSelectInput(d, w, ExposureMask | KeyPressMask);

    // ウィンドウを表示
    XMapWindow(d, w);

    // イベントループ
    while (1) {
        XNextEvent(d, &e);
        if (e.type == Expose) {
            XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 100, 50);
        }
        if (e.type == KeyPress)
            break;
    }

    // クリーンアップ
    XDestroyWindow(d, w);
    XCloseDisplay(d);
    return 0;
}

