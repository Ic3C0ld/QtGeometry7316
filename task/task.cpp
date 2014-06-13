#include "task.h"
#include "AVLTree.h"



C2DRect QRect_to_C2DRect(QRectF rect)
{
    QPointF topLeft=rect.topLeft();
    QPointF bottomRight=rect.bottomRight();

    C2DRect c2drect=C2DRect(C2DPoint(topLeft.x(),topLeft.y()),
                         C2DPoint(bottomRight.x(),bottomRight.y()) );
    return c2drect;
}


QRectF C2DRect_to_QRect(C2DRect rect)
{
    C2DPoint topLeft=rect.GetTopLeft();
    C2DPoint bottomRight=rect.GetBottomRight();


    QRectF qrect=QRectF(QPointF(topLeft.x,topLeft.y),
                        QPointF(bottomRight.x,bottomRight.y));
    return qrect;
}


///
///         Event
///

bool Event::operator <(const Event& rhs)
{
    if(this->p.y  < rhs.p.y)
        return true;
    if (this->p.y > rhs.p.y)
        return false;

// y1=y2

    if (this->p.x < rhs.p.x )
        return false;
    if( this->p.x > rhs.p.x )
        return true;

//if  p1==p2
    return false;
}
bool Event::operator>(const Event& rhs)
{
    if(this->p.y  > rhs.p.y)
        return true;
    if (this->p.y < rhs.p.y)
        return false;

// y1=y2
    if (this->p.x < rhs.p.x )
        return true;
    if( this->p.x > rhs.p.x )
        return false;

//if  p1=p2
    return false;
}
bool Event::operator==(const Event& rhs)
{
    if (this->p.y == rhs.p.y &&
        this->p.x == rhs.p.x)
        return true;


    return false;
}
Event& Event::operator=(const Event& rhs)
{
    this->index=rhs.index;
    this->lines=rhs.lines;
    this->p=rhs.p;
    this->type=rhs.type;

    return (*this);
}

///
///
///

///     C2DPoint Comparisons
bool operator<(const C2DPoint& p1,const C2DPoint& p2)
{
    if (p1.y<p2.y)
        return true;
    if (p1.y>p2.y)
        return false;

/// else ys are equal
    if(p1.x<p2.x)
        return false;
    if(p1.x>p2.x)
        return true;
/// else identical points
    return false;
}
bool operator>(const C2DPoint& p1,const C2DPoint& p2)
{
    if (p1.y>p2.y)
        return true;
    if (p1.y<p2.y)
        return false;

/// else ys are equal
    if(p1.x<p2.x)
        return true;
    if(p1.x>p2.x)
        return false;
/// else identical points
    return false;
}
bool operator==(const C2DPoint& p1,const C2DPoint& p2)
{
    if (p1.y==p2.y   &&
        p1.x==p2.x)
        return true;

    return false;
}

///

///
///  TLine
///

bool TLine::operator<(const TLine& rhs)
{




    return false;
}
bool TLine::operator>(const TLine&)
{
    return false;

}
bool TLine::operator==(const TLine&)
{
    return false;

}

TLine& TLine::operator=(const TLine&)
{
    return *this;
}





std::vector<C2DPolygon> find_perimeter_polygon(std::vector<C2DRect> crect)
{
/// -------------------------------------------------------------
    /// make QRects to C2DRects
//    std::vector<C2DRect> crect;
//    for (int i=0;i<rect.size();i++)
//    {
//        crect.push_back(QRect_to_C2DRect(rect[i]));
//    }

/// -------------------------------------------------------------
/// extract all the lines from the C2DRects
    std::vector<C2DLine> lines;

    C2DLine tmp_line;
    C2DPoint bot_left,
             top_left,
             bot_right,
             top_right;
    for (int i=0;i<crect.size();i++)
    {
        bot_left=C2DPoint(crect[i].GetLeft(),crect[i].GetBottom());
        bot_right=C2DPoint(crect[i].GetRight(),crect[i].GetBottom());
        top_left=C2DPoint(crect[i].GetLeft(),crect[i].GetTop());
        top_right=C2DPoint(crect[i].GetRight(),crect[i].GetTop());

        lines.push_back(C2DLine(bot_right,top_right));
        lines.push_back(C2DLine(top_right,top_left));
        lines.push_back(C2DLine(top_left,bot_left));
        lines.push_back(C2DLine(bot_left,bot_right));

    }

/// -------------------------------------------------------------
/// extract all the EVENTS from the C2DLines' points
/// create the initial Q-Tree,
/// -------------------------------------------------------------
    AVLTree<Event> Q;

    for (int i=0;i<lines.size();i++)
    {
        ///create the 2 events -----------------------------
        Event evt1,evt2;

        evt1=Event(lines[i].GetPointFrom(),
                     Top,
                     i,&lines);

        evt2=Event(lines[i].GetPointTo(),
                     Bottom,
                     i,&lines);

        if (lines[i].GetPointFrom()<lines[i].GetPointTo())
        {
            //swap types in case it was assumed wrong
            evt1.type[0]=Bottom;
            evt2.type[0]=Top;
        }

        /// insert the 2 events -----------------------------
        Node<Event>* node=Q.search(evt1);
        if(node==NULL)
        {
            Q.insert(evt1);
        }
        else
        {
            //if this event exists ,it means
            //there is another point from another line
            //ending or starting here
            node->data->type.push_back(evt1.type[0]);
            node->data->index.push_back(i);
        }

        node=Q.search(evt2);

        if(node==NULL)
        {
            Q.insert(evt2);
        }
        else
        {
            node->data->type.push_back(evt2.type[0]);
            node->data->index.push_back(i);
        }

    }//end for: lines

/// -----------------------------------------------------------------------
/// Find the intersections
/// -----------------------------------------------------------------------

    while (Q.root != NULL)//while Q-Tree is not empty
    {


    }

    std::vector<C2DPolygon> polys;
    return polys;

}










Task::Task(QObject *parent) :
    QObject(parent)
{
}
