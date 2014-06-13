#include "glview.h"
#include <QtMath>
#include <QMouseEvent>

GLView::GLView(QWidget *parent) :
    QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(200,200);

    QGLFormat format;
    format.setSamples(4);

    setFormat(format);


    slot_update_lookAt_vars();

    resize(700,700);


}

void GLView::initializeGL()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1);

    // Set up lights
    GLfloat light_position[] = {    0,    0,  200,  1};
    GLfloat ambientLight[]   = {  0.5,  0.5,  0.5,  1};
    GLfloat diffuseLight[]   = {  0.8,  0.8,  0.8,  1};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Black background
    glClearColor(0.1f,0.1f,0.1f,1.0f);

}
void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(cv.lookAt.data());



    ////////// draw Here /////////


    for (int i=0;i<poly.size();i++)
    {
        drawPoly(poly[i]);
    }
    for (int i=0;i<rect.size();i++)
    {
        drawRect(rect[i]);
    }


    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0xFAFA);
    glBegin(GL_LINES);

    glVertex3f(-200,0,0);
    glVertex3f(200,0,0);

    glVertex3f(0,-200,0);
    glVertex3f(0,200,0);

    glEnd();
    glDisable(GL_LINE_STIPPLE);

}
void GLView::resizeGL(int w, int h)
{
    if (h==0)   h=1; //avoid perspective 0 division w/h == w/0

    //viewport
    glViewport(0,0,w,h);

    //perspective
    cv.perspective.setToIdentity();
    cv.perspective.perspective(cv.fov,float(w)/h,1,1000);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMultMatrixf(cv.perspective.data());
}
void GLView::mousePressEvent(QMouseEvent *event)
{
    cv.lastClickPosition=event->pos();
}
void GLView::mouseMoveEvent(QMouseEvent *event)
{
    float dx = (event->x() - cv.lastClickPosition.x());
    float dy = (event->y() - cv.lastClickPosition.y());


    if((event->buttons() & Qt::LeftButton) && (event->buttons()& Qt::RightButton))
    {
        if (dx !=0 || dy!=0)
        {
            cv.phi-=2*dx/width();
            cv.theta+=2*dy/height();
        }

    }
    else
    {
        if(event->buttons() & Qt::LeftButton)
        {
            if (dx !=0 || dy!=0)
            {
                 cv.eye+=cv.left*dx/(0.025*width()) + cv.up*dy/(0.025*height());
            }
        }
    }

    if(event->buttons() & (Qt::MiddleButton |
                           Qt::LeftButton   |
                           Qt::RightButton  ) )
    {
        cv.lastClickPosition=event->pos();
        slot_update_lookAt_vars();
    }
}
void GLView::mouseDoubleClickEvent(QMouseEvent *event)
{
    cv=cv_backup;
}

void GLView::mouseReleaseEvent(QMouseEvent *event)
{

}
void GLView::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
       {
           int delta=event->delta()/8;
           cv.eye+=delta*cv.forward/10.0;
            slot_update_lookAt_vars();
       }
}


void GLView::drawRect(const QRectF &rect)
{
    double x1,y1,x2,y2;

    rect.getCoords(&x1,&y1,&x2,&y2);

    glPushMatrix();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glRectf(x1,y1,x2,y2);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


    glPopMatrix();
}
void GLView::drawPoly(const QPolygonF &poly)
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glBegin(GL_POLYGON);
        for (int i=0;i<poly.size();i++)
        {
            glVertex3f(poly[i].x(),poly[i].y(),0);
        }
    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

}
void GLView::slot_set_polygon(std::vector<QPolygonF> &polygon)
{
    this->poly=polygon;
    updateGL();
}
void GLView::slot_set_rectVector(std::vector<QRectF> &rectVector)
{
    this->rect=rectVector;
    updateGL();
}


GLView::ControlVariables::ControlVariables()
{
    ///initialize lookAt Vars
    up=QVector3D(0,1,0);
    center=QVector3D(0,0,0);
    eye=QVector3D(0,0,50);

    r=eye.length();
    theta=acos(-eye.y()/r);
    phi=atan2(-eye.x(),-eye.z());

    fov=60;




    ///
}
void GLView::slot_update_lookAt_vars()
{
    cv.forward.setZ(cv.r*sin(cv.theta)*cos(cv.phi));
    cv.forward.setX(cv.r*sin(cv.theta)*sin(cv.phi));
    cv.forward.setY(cv.r*cos(cv.theta));
    cv.forward.normalize();

    cv.up.setZ(cv.r*sin(cv.theta-M_PI_2)*cos(cv.phi));
    cv.up.setX(cv.r*sin(cv.theta-M_PI_2)*sin(cv.phi));
    cv.up.setY(cv.r*cos(cv.theta-M_PI_2));
    cv.up.normalize();

   cv.center = cv.eye+cv.forward;

   cv.left=cv.left.crossProduct(cv.up,cv.forward);

   cv.lookAt.setToIdentity();
   cv.lookAt.lookAt(cv.eye,cv.center,cv.up);


    updateGL();
}
