#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH  300
#define HEIGHT 200
#define BUTTON_X 100
#define BUTTON_Y 80
#define BUTTON_W 100
#define BUTTON_H 40

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;
    GC gc;
    XGCValues values;
    unsigned long white, black;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    white = WhitePixel(display, screen);
    black = BlackPixel(display, screen);

    window = XCreateSimpleWindow(display, RootWindow(display, screen), 
                                 10, 10, WIDTH, HEIGHT, 1, black, white);

    XSelectInput(display, window, ExposureMask | ButtonPressMask | StructureNotifyMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, &values);
    XSetForeground(display, gc, black);

    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            // Draw button rectangle
            XDrawRectangle(display, window, gc, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H);
            // Draw button label
            XDrawString(display, window, gc, BUTTON_X + 20, BUTTON_Y + 25, "Close", 5);
        } else if (event.type == ButtonPress) {
            int x = event.xbutton.x;
            int y = event.xbutton.y;

            if (x >= BUTTON_X && x <= BUTTON_X + BUTTON_W &&
                y >= BUTTON_Y && y <= BUTTON_Y + BUTTON_H) {
                break; // Button clicked, exit loop
            }
        } else if (event.type == DestroyNotify) {
            break;
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}

