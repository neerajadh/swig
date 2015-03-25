#ifndef __DRAWER__
#define __DRAWER__

#include "mathematics/Matrix.h"
#include "mathematics/Linspace.h"

#include "ScreenPoint.h"
#include "Lincolor.h"

#define Plotter_ SDLPlotter
#include SSTR(Plotter_.h)

// The drawer class is an abstraction that handles the drawing
// of primitives. Drawing lines, filling polygons, etc are done
// through Drawer.
class Drawer {

    private:
    // Pointer to a plotter object
    Plotter_ *plotter;

    // A depth buffer, a matrix of uint32_t
    Matrix<uint32_t> depth;

    void initAscending(ScreenPoint& start, ScreenPoint& mid,
            ScreenPoint& end, const ScreenPoint& pt1,
            const ScreenPoint& pt2, const ScreenPoint& pt3);

    public:
    Drawer(Plotter_ *pltr);

    // Update screen.
    void update();

    // Clear the screen
    void clear(Color clearColor={255,0,255});

    void pixel(const ScreenPoint& point);

    // Draw a line from start to end
    void line(const ScreenPoint& start,const ScreenPoint& end);

    // Draw a horizontal line between (xs,y) and (xe,y)
    void hLine(int y, int xs, int xe, Color cl);

    // Draw a horizontal line considering the depth buffer
    void hLineD(int y, int xs, int hs,
            int xe, int he, Color cl, bool overwrite=true);

    // Draw a horizontal line considering the depth buffer
    // and color gradient
    void hLineD(int y, int xs, int hs, int xe, int he, Color cStart,
            Color cEnd, bool overwrite=true);

    // Fill the triangle bounded by pt1, pt2 and pt3
    // considering depth buffer and color gradient
    // overwrite when true will enable overwrite to same depth
    void fillD(ScreenPoint pt1, ScreenPoint pt2, ScreenPoint pt3,
            bool interpolate=true,bool overwrite=true);

    // Get the screen width
    int getWidth() const {
        return plotter->width();
    }

    // Get the screen height
    int getHeight() const {
        return plotter->height();
    }
};

#endif
