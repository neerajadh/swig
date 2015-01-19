#ifndef __DRAWER__
#define __DRAWER__

#include "Point.h"
#include "setPlotter.h"

#include SSTR(Plotter_.h)

// The drawer class is an abstraction that handles the drawing of
// primitives. Drawing lines, filling polygons, etc are done
// through Drawer.
class Drawer {

    private:
    Plotter_ *plotter;

    public:
    Drawer(Plotter_ *pltr);

    // Update screen.
    void update();
    // Clear the screen
    void clear();
    // Draw a line from start to end
    void line(const ScreenPoint& start,const ScreenPoint& end);
    // Fill the triangle bounded by pt1, pt2 and pt3
    void fill(ScreenPoint pt1, ScreenPoint pt2,
            ScreenPoint pt3, Color fillcolor);
    // Draw a horizontal line between (xs,y) and (xe,y)
    void hLine(unsigned y, unsigned xs, unsigned xe, Color cl);
};

#endif