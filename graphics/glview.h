#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QRectF>
#include <QMatrix4x4>
#include <QVector3D>

class GLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLView(QWidget *parent = 0);
protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    //controls // QObject overrides
    virtual void  mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    void drawRect(const QRectF& rect);
    void drawPoly(const QPolygonF& poly);
signals:

public slots:
    void slot_set_rectVector(std::vector<QRectF>&);
    void slot_set_polygon(std::vector<QPolygonF>&);
private slots:
    void slot_update_lookAt_vars();

public:
protected:
private:
std::vector<QRectF> rect;
std::vector<QPolygonF> poly;

    struct ControlVariables
    {
        ControlVariables();

        float r,phi,theta,fov;

        QMatrix4x4 perspective,lookAt;
        QVector3D eye,center,up;

        QVector3D forward,left;
        QPoint lastClickPosition;

    }cv,cv_backup;
};

#endif // GLVIEW_H
