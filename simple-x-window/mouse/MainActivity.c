#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    Display *display;
    Window window;
    XEvent event;
    
    // ディスプレイをオープン
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        exit(1);
    }

    // ウィンドウを作成
    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 10, 10, 400, 300, 1, BlackPixel(display, 0), WhitePixel(display, 0));
    XSelectInput(display, window, ButtonPressMask | KeyPressMask);  // ボタンのクリックイベントをキャッチ
    XMapWindow(display, window);

    // イベントループ
    while (1) {
        XNextEvent(display, &event);
        
        if (event.type == ButtonPress) {
            // 左クリック（ボタン1）
            if (event.xbutton.button == Button1) {
                printf("Left click detected, closing window...\n");
                XDestroyWindow(display, window);  // ウィンドウを閉!じる
                break;  // イベントループを終了
            }
        }

        // 他のイベントも必要に応じて処理
    }

    // ディスプレイを閉じる
    XCloseDisplay(display);
    return 0;
}

