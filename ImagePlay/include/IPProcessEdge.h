//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

#ifndef IPProcessEdge_H
#define IPProcessEdge_H

#include <QGraphicsItem>

#include "IPProcessStep.h"

class IPProcessStep;

//-----------------------------------------------------------------------------
//!Custom QGraphicsItem rendering the process connections
/*!
 * IPProcessEdge is also used for representening the logical structure of a
 * process graph.
 */
class IPProcessEdge : public QGraphicsItem
{
public:
    explicit            IPProcessEdge       (IPProcessStep* from, IPProcessStep* to, int indexFrom, int indexTo);
                        ~IPProcessEdge      ();
    void                paint               (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF              boundingRect        () const;
    QPainterPath        shape               () const;
    void                updatePosition      ();
    IPProcessStep*      from                ()  { return _from; }
    IPProcessStep*      to                  ()  { return _to; }
    int                 indexFrom           ()  { return _indexFrom; }
    int                 indexTo             ()  { return _indexTo; }

private:
    IPProcessStep*      _from;              //!< First IPProcessStep
    IPProcessStep*      _to;                //!< Second IPProcessStep
    QPointF             _intersectionFrom;  //!< Calculated intersection point of the first square
    QPointF             _intersectionTo;    //!< Calculated intersection point of the second square
    std::vector<QColor> _colorArrow;        //!< Different colors for IPLData types
    int                 _indexFrom;         //!< First result index
    int                 _indexTo;           //!< Second result index
    int                 _typeIndex;         //!< Type used for _colorArrow
    bool                _mouseOver;         //!< Current mouseover state

    // QGraphicsItem interface
protected:
    void                mouseMoveEvent     (QGraphicsSceneMouseEvent *event);
    void                mouseReleaseEvent  (QGraphicsSceneMouseEvent *event);
    void                hoverEnterEvent    (QGraphicsSceneHoverEvent *event);
    void                hoverLeaveEvent    (QGraphicsSceneHoverEvent *event);
    void                hoverMoveEvent     (QGraphicsSceneHoverEvent *event);
    QVariant            itemChange         (GraphicsItemChange change, const QVariant &value);
};

#endif // IPProcessEdge_H
