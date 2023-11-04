#include "window.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <unistd.h>

#include <cstdlib>
#include <string>

#include "graphicsException.h"

using namespace std;

Xwindow::Xwindow(int width, int height) {
    d = XOpenDisplay(NULL);
    if (d == NULL) {
        throw GraphicsException("Cannot open display");
        exit(1);
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1, BlackPixel(d, s),
                            WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapRaised(d, w);

    Pixmap pix = XCreatePixmap(d, w, width, height, DefaultDepth(d, DefaultScreen(d)));
    gc = XCreateGC(d, pix, 0, (XGCValues *)0);

    XFlush(d);
    XFlush(d);

    // Set up colours.
    XColor xcolour;
    Colormap cmap;
    char color_vals[7][10] = {"white", "black", "red", "green", "blue"};

    cmap = DefaultColormap(d, DefaultScreen(d));
    for (int i = 0; i < 5; ++i) {
        XParseColor(d, cmap, color_vals[i], &xcolour);
        XAllocColor(d, cmap, &xcolour);
        colours[i] = xcolour.pixel;
    }

    XSetForeground(d, gc, colours[Black]);

    // Make window non-resizeable.
    XSizeHints hints;
    hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
    hints.height = hints.base_height = hints.min_height = hints.max_height = height;
    hints.width = hints.base_width = hints.min_width = hints.max_width = width;
    XSetNormalHints(d, w, &hints);

    XSynchronize(d, True);

    usleep(1);
}

Xwindow::~Xwindow() {
    XFreeGC(d, gc);
    XCloseDisplay(d);
}

void Xwindow::clearScreen() { XClearWindow(d, w); }

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
    XSetForeground(d, gc, colours[colour]);
    XFillRectangle(d, w, gc, x, y, width, height);
    XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, string msg) {
    XDrawString(d, w, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
}

void Xwindow::drawXpmImage(int x, int y, const std::string &imgPath, bool transparent) {
    XImage *img;

    if (!transparent) {
        if (XpmReadFileToImage(d, imgPath.c_str(), &img, NULL, NULL)) {
            throw GraphicsException("Cannot open " + imgPath);
        }
        XPutImage(d, w, gc, img, 0, 0, x, y, img->width, img->height);
        return;
    }

    XImage *clp;  // used to check transparancy parts of image

    // read the image
    if (XpmReadFileToImage(d, imgPath.c_str(), &img, &clp, NULL)) {
        throw GraphicsException("Cannot open " + imgPath);
    }

    // copy the transparent image into to pixmap
    Pixmap pix = XCreatePixmap(d, w, clp->width, clp->height, clp->depth);
    XPutImage(d, pix, XCreateGC(d, pix, 0, NULL), clp, 0, 0, 0, 0, clp->width, clp->height);

    // set clip origin and copy
    XSetClipMask(d, gc, pix);
    XSetClipOrigin(d, gc, x, y);
    XPutImage(d, w, gc, img, 0, 0, x, y, img->width, img->height);

    /**
     * @Note: We XSetClipMask(d, gc, None); at the end after displaying image
     * This tells that next drawing operation does not involve transparent points.
     * this command has to be executed before calling functions like XDrawRectangle, etc.
     * However, since for pieces we do not overlap images and use these commands
     * We avoid that for efficiency
     */
   
}
