#include "Drawer.h"
#include "Shader.h"

// Construct.
Drawer::Drawer(Plotter_ *pltr):
    plotter(pltr),
    depth({pltr->width(),pltr->height()})
{
}

void Drawer::pixel(const ScreenPoint& point){
    plotter->plot(point.x,point.y,point.color,false);
}

// Draw line between start and end
// using Bresenham's famous algorithm
void Drawer::line(const ScreenPoint& start,
        const ScreenPoint& end) {
    int dx, dy, x, y, xc, yc;
    int D,twodydx,twodxdy,twody,twodx;

    // If they aren't cast to int then the result will
    // be a uint which will never have a negative value
    dx = Math::abs((int)end.x-(int)start.x);
    dy = Math::abs((int)end.y-(int)start.y);
    if (dx==0) xc=0;
    else xc = (start.x>end.x)?-1:1;
    if (dy==0) yc=0;
    else yc = (start.y>end.y)?-1:1;

    x = start.x;
    y = start.y;
    plotter->plot(x,y,start.color);

    if (dx>dy) {
        D = 2*dy-dx;
        twodydx = 2*(dy-dx);
        twody = 2*dy;

        while (x!=end.x) {
            x+=xc;
            if (D>0) {
                y+=yc;
                plotter->plot(x,y,start.color);
                D += twodydx;
            } else {
                plotter->plot(x,y,start.color);
                D += twody;
            }
        }

    } else {
        D = 2*dx-dy;
        twodxdy = 2*(dx-dy);
        twodx = 2*dx;

        while (y!=end.y) {
            y+=yc;
            if (D>0) {
                x+=xc;
                plotter->plot(x,y,start.color);
                D += twodxdy;
            } else {
                plotter->plot(x,y,start.color);
                D += twodx;
            }
        }
    }
}


// Draw a horizontal line between (xs,y) and (xe,y)
// This one doesn't consider the point depths.
void Drawer::hLine(int y, int xStart, int xEnd, Color cl) {
    // If y lies outside then return
    if( y >= (int)plotter->height() || y < 0)
        return;
    // xStart must be smaller than xEnd
    if (xStart>xEnd)
        swap(xStart,xEnd);
    // If x lies outside then return
    if( xStart >= (int)plotter->width() || xEnd < 0)
        return;

    // Clip the x axis
    xStart = Math::max(0,xStart);
    xEnd = Math::min(xEnd,(int)plotter->width()-1);

    while(xStart <= xEnd){
        plotter->plot(xStart,y,cl,false);
        xStart++;
    }
}

// This one considers the pixel depths while plotting. It only
// plots points closer than already there.
// The parameters are : y-coordinate, starting x-coordinate,
// starting depth value, ending x-coordinate and ending depth
// value

void Drawer::hLineD(int y, int xStart, int dStart,
        int xEnd, int dEnd, Color cl, Pair<Vector> realvs, Shader* sh,
        bool overwrite) {
    // Sort the start end end values if they are not in order

    if (xStart>xEnd) {
        swap(xStart,xEnd);
        swap(dStart,dEnd);
        swap(realvs.x,realvs.y);
    }
    // If y lies outside then return
    if( y >= (int)plotter->height() || y < 0)
        return;
    // If x lies outside then return
    if( xStart >= (int)plotter->width() || xEnd < 0)
        return;

    Linspace d(dStart,dEnd,xStart,xEnd);

    const Matrix<float>& shadow_xForm = sh->shadowMat();
    Vector sstart = {realvs.x.x,realvs.x.y,realvs.x.z,realvs.x.w};
    sstart = sstart * shadow_xForm;
    sstart.projectionNormalize();
    Vector send = {realvs.y.x,realvs.y.y,realvs.y.z,realvs.y.w};
    send = send * shadow_xForm;
    send.projectionNormalize();

    Vector delta = {0,0,0,0};
    if (xStart!=xEnd)
        delta = (send-sstart)/(double)(xEnd-xStart);

    // Clipping
    xEnd = Math::min(xEnd,(int)plotter->width()-1);
    // Clipping
    xStart = Math::max(0,xStart);

    while(xStart <= xEnd){
        // Depth clipping, checking with zero isn't necessary
        // as depth(xStart,y) is always greater than or equal
        // to 0
        // checking with far value must be done however
        // 0xffffff value because it is the maximum value it
        // should attain
        int de = d.at(xStart);
        if ((overwrite && de<=ScreenPoint::maxDepth &&
                    de>=depth(xStart,y)) ||
            (!overwrite &&  de<=ScreenPoint::maxDepth &&
             de>depth(xStart,y)) ) {
                if (sh->onShadow(sstart)) {
                    Color ncol = {cl.blue*0.5,cl.green*0.5,cl.red*0.5,
                        0xff};
                    plotter->plot(xStart,y,ncol,false);
                } else
                    plotter->plot(xStart,y,cl,false);
                depth(xStart,y)=de;
            }
        ++xStart;
        sstart += delta;
    }
}

// This one considers the pixel depths while plotting. It only
// plots points closer than already there.
// The parameters are : y-coordinate, starting x-coordinate,
// starting depth value, ending x-coordinate and ending depth
// value
void Drawer::hLineD(int y, int xStart, int dStart, int xEnd,
        int dEnd, Color cStart,Color cEnd, Pair<Vector> realvs,
        Shader* sh, bool overwrite) {
    // Sort the start end end values if they are not in order
    if (xStart>xEnd) {
        swap(xStart,xEnd);
        swap(dStart,dEnd);
        swap(cStart,cEnd);
        swap(realvs.x,realvs.y);
    }
    // If y lies outside then return
    if( y >= (int)plotter->height() || y < 0)
        return;
    // If x lies outside then return
    if( xStart >= (int)plotter->width() || xEnd < 0)
        return;

    Linspace d(dStart,dEnd,xStart,xEnd);
    Lincolor c(cStart,cEnd,xStart,xEnd);

    const Matrix<float>& shadow_xForm = sh->shadowMat();
    Vector sstart = {realvs.x.x,realvs.x.y,realvs.x.z,realvs.x.w};
    sstart = sstart * shadow_xForm;
    sstart.projectionNormalize();
    Vector send = {realvs.y.x,realvs.y.y,realvs.y.z,realvs.y.w};
    send = send * shadow_xForm;
    send.projectionNormalize();
    Vector delta = {0,0,0,0};
    if (xStart!=xEnd)
        delta = (send-sstart)/(float)(xEnd-xStart);

    // Clipping
    xEnd = Math::min(xEnd,(int)plotter->width()-1);
    xStart = Math::max(0,xStart);

    while(xStart <= xEnd){
        // Depth clipping, checking with zero isn't necessary
        // as depth(xStart,y) is always greater than or
        // equal to 0
        // checking with far value must be done however
        // 0xffffff value because it is the maximum value it
        // should attain
        int de = d.at(xStart);
        if ((overwrite && de<=ScreenPoint::maxDepth &&
                    de>=depth(xStart,y)) ||
            (!overwrite && de<=ScreenPoint::maxDepth &&
             de>depth(xStart,y)) ) {
            Color cl = c.at(xStart);
            if (sh->onShadow(sstart)) {
                Color ncol = {cl.blue*0.5,cl.green*0.5,cl.red*0.5,
                    0xff};
                plotter->plot(xStart,y,ncol,false);
            } else plotter->plot(xStart,y,cl,false);
            depth(xStart,y)=de;
        }
        ++xStart;
        sstart+=delta;
    }
}

// We need to sort the points according to their
// y-coordinates
void Drawer::initAscending(ScreenPoint& start,
        ScreenPoint& mid, ScreenPoint& end, const ScreenPoint& pt1,
        const ScreenPoint& pt2, const ScreenPoint& pt3) {
    if (pt1.y<=pt2.y && pt1.y<=pt3.y) {
        start = pt1;
        if (pt2.y<=pt3.y) {
            mid = pt2;
            end = pt3;
        } else {
            mid = pt3;
            end = pt2;
        }
    }
    else if (pt2.y<=pt1.y && pt2.y<=pt3.y) {
        start = pt2;
        if (pt1.y<=pt3.y) {
            mid = pt1;
            end = pt3;
        } else {
            mid = pt3;
            end = pt1;
        }
    }
    else {
        start = pt3;
        if (pt1.y<=pt2.y) {
            mid = pt1;
            end = pt2;
        } else {
            mid = pt2;
            end = pt1;
        }
    }
}


// Fill the triangle bounded by pt1, pt2 and pt3
// What is implemented here is a special case of
// scan-line filling which works only for triangles.
// considering depth buffer
// overwrite when true will enable overwrite to same depth
void Drawer::fillD(ScreenPoint pt1, ScreenPoint pt2,
        ScreenPoint pt3, bool interpolate,Shader* sh,bool overwrite) {

    ScreenPoint start, mid, end;
    initAscending(start,mid,end,pt1,pt2,pt3);

    if(start.y == end.y)
        return;
    if( start.y >= (int)plotter->height() || end.y < 0)
        return;
    // THe negative region is backside of the camera
    // or away from the far point
    if(start.d<=0 || end.d<=0 || mid.d<=0)
        return;

    Linspace x1(start.x,mid.x, start.y, mid.y);
    Linspace x2(start.x,end.x, start.y, end.y);
    Linspace x3(mid.x,end.x, mid.y, end.y);

    Linspace d1(start.d,mid.d, start.y , mid.y);
    Linspace d2(start.d,end.d, start.y, end.y);
    Linspace d3(mid.d,end.d, mid.y, end.y);

    LinspaceF ax(start.real.x,mid.real.x,start.y,mid.y);
    LinspaceF ay(start.real.y,mid.real.y,start.y,mid.y);
    LinspaceF az(start.real.z,mid.real.z,start.y,mid.y);

    LinspaceF bx(start.real.x,end.real.x,start.y,end.y);
    LinspaceF by(start.real.y,end.real.y,start.y,end.y);
    LinspaceF bz(start.real.z,end.real.z,start.y,end.y);

    LinspaceF cx(mid.real.x,end.real.x,mid.y,end.y);
    LinspaceF cy(mid.real.y,end.real.y,mid.y,end.y);
    LinspaceF cz(mid.real.z,end.real.z,mid.y,end.y);

    Lincolor c1(start.color,mid.color,start.y,mid.y);
    Lincolor c2(start.color,end.color,start.y,end.y);
    Lincolor c3(mid.color,end.color,mid.y,end.y);

    // Clipping
    if(interpolate){
        start.y = Math::min(mid.y,Math::max(start.y,0));
        for(int i=start.y;i<Math::min((int)plotter->height(),
                    mid.y);i++) {
            Vector rva = {ax.at(i),ay.at(i),az.at(i),1};
            Vector rvb = {bx.at(i),by.at(i),bz.at(i),1};
            Pair<Vector> realvs = {rva,rvb};
            hLineD(i,x1.at(i),d1.at(i),x2.at(i),d2.at(i),
                    c1.at(i),c2.at(i),realvs,sh,overwrite);
        }
        // Clipping
        mid.y = Math::max(mid.y,0);
        for(int i=mid.y;i<=Math::min((int)plotter->
                    height()-1,end.y);i++) {
            Vector rvb = {bx.at(i),by.at(i),bz.at(i),1};
            Vector rvc = {cx.at(i),cy.at(i),cz.at(i),1};
            Pair<Vector> realvs = {rvb,rvc};
            hLineD(i, x2.at(i), d2.at(i), x3.at(i), d3.at(i),
                    c2.at(i), c3.at(i), realvs,sh, overwrite);
        }

    } else {
        start.y = Math::min(mid.y,Math::max(start.y,0));
        for(int i=start.y;i<Math::min((int)plotter->height(),
                    mid.y);i++) {
            Vector rva = {ax.at(i),ay.at(i),az.at(i),1};
            Vector rvb = {bx.at(i),by.at(i),bz.at(i),1};
            Pair<Vector> realvs = {rva,rvb};
            hLineD(i,x1.at(i),d1.at(i),x2.at(i),d2.at(i),
                    start.color,realvs,sh,overwrite);
        }
        // Clipping
        mid.y = Math::max(mid.y,0);
        for(int i=mid.y;i<=Math::min((int)plotter->height()-1,
                    end.y);i++) {
            Vector rvb = {bx.at(i),by.at(i),bz.at(i),1};
            Vector rvc = {cx.at(i),cy.at(i),cz.at(i),1};
            Pair<Vector> realvs = {rvb,rvc};
            hLineD(i, x2.at(i), d2.at(i), x3.at(i), d3.at(i),
                    start.color, realvs,sh, overwrite);
        }
    }
}
