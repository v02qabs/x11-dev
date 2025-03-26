#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_LENGTH 100

int main() {
    Display *display;
    Window window;
    XEvent event;
    char text[MAX_TEXT_LENGTH] = "";
    int text_len = 0;

    // X11ディスプレイへの接続
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "XOpenDisplay failed\n");
        exit(1);
    }

    // ウィンドウの作成
    int screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 400, 200, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    // ウィンドウの表示
    XMapWindow(display, window);

    // イベントマスクの設定（キーボード入力を受け取るため）
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // グラフィックスコンテキストの作成
    GC gc = XCreateGC(display, window, 0, NULL);
    XFontStruct *font = XLoadQueryFont(display, "fixed");
    if (!font) {
        fprintf(stderr, "Font not found\n");
        exit(1);
    }
    XSetFont(display, gc, font->fid);

    // メインイベントループ
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            // ウィンドウが再描画されたときの処理
            XClearWindow(display, window);
            XDrawString(display, window, gc, 10, 50, text, text_len);
        }

        if (event.type == KeyPress) {
            // キーが押されたときの処理
            char buffer[1];
            KeySym keysym;
            XComposeStatus compose_status;

            // キー入力を取得
            int len = XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, &compose_status);
            if (len > 0 && text_len < MAX_TEXT_LENGTH - 1) {
                text[text_len++] = buffer[0];
                text[text_len] = '\0'; // null-terminate the string
                // ウィンドウを再描画して入力を表示
                XClearWindow(display, window);
                XDrawString(display, window, gc, 10, 50, text, text_len);
            }
        }
    }

    // クリーンアップ
    XFreeGC(display, gc);
    XFreeFont(display, font);
    XCloseDisplay(display);
    return 0;
}

