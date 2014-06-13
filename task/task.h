#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QPolygonF>
#include <QRectF>


#ifdef _STATIC
    #include "GeoLib.h"
#else
    #define _STATIC
    #include "GeoLib.h"
    #undef _STATIC
#endif


enum Event_Type{Top=0,Middle,Bottom,Null};//middle is an intersection point


class Event
{
public:
    Event():lines(){}
    Event(C2DPoint p_,int type_,int index_,std::vector<C2DLine>* lines_ptr)
    {
        p=p_;
        type.push_back(type_);

        index.clear();
        index.push_back(index_);
        lines=lines_ptr;
    }
    Event(C2DPoint p_,std::vector<int> type_,std::vector<int> indexes,std::vector<C2DLine>* lines_ptr)
    {
        p=p_;
        type=type_;
        index=indexes;
        lines=lines_ptr;
    }

    //boolean operators state that topLeft > bottomRight
    // primary check on Y then X if necessary
    bool operator<(const Event& rhs);
    bool operator>(const Event& rhs);
    bool operator==(const Event& rhs);
    Event& operator=(const Event& rhs);

    /// VARIABLES
    C2DPoint p;

    std::vector<int> type;
    std::vector<int> index;//which lines comprise the event,
    std::vector<C2DLine>* lines;// saved as indices to this lineSet

};


std::vector<C2DPolygon> find_perimeter_polygon(std::vector<C2DRect> rect);

C2DRect QRect_to_C2DRect(QRectF rect);
QRectF C2DRect_to_QRect(C2DRect rect);


bool operator<(const C2DPoint& p1,const C2DPoint& p2);
bool operator>(const C2DPoint& p1,const C2DPoint& p2);
bool operator==(const C2DPoint& p1,const C2DPoint& p2);


class TLine
{
public:
    TLine():lines(),y_cross(),index(-1){}
    TLine(std::vector<C2DLine>* lines_ptr ,int index_,double* y_cross_)
        :lines(lines_ptr),index(index),y_cross(y_cross_)
    {

    }


    bool operator<(const TLine&);
    bool operator>(const TLine&);
    bool operator==(const TLine&);

    TLine& operator=(const TLine&);

    /// VARIABLES
    int index;
    std::vector<C2DLine>* lines;
    double* y_cross;//where is the crossing line at;as a pointer ,it will be modiefied elsewhere


};









class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = 0);

signals:

public slots:

};

#endif // TASK_H
