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

#ifndef IPPropertyWidget_H
#define IPPropertyWidget_H

#include <QWidget>

#include "IPL_processes.h"




class IPLColorPickHandler
{
public:
    IPLColorPickHandler()
    {

    }
    virtual void pickColor(IPLColor color) = 0;
    virtual void finishPickingColor() = 0;
};

class IPLColorPickProvider
{
public:
    IPLColorPickProvider() {}
    virtual void setColorPickHandler(IPLColorPickHandler*) = 0;
    virtual void clearColorPickHandler() = 0;

};

class IPLCoordinatePickHandler
{
public:
    IPLCoordinatePickHandler()
    {

    }
    virtual void pickCoordinates(int x, int y) = 0;
    virtual void finishPickingCoordinates() = 0;
};

class IPLCoordinatePickProvider
{
public:
    IPLCoordinatePickProvider() {}
    virtual void setCoordinatePickHandler(IPLCoordinatePickHandler*) = 0;
    virtual void clearCoordinatePickHandler() = 0;

};

class IPPropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IPPropertyWidget(IPLProcessProperty* processProperty, QWidget* parent=0) : QWidget(parent)
    {
        _processProperty = processProperty;
    }
    ~IPPropertyWidget()
    {
        _processProperty = NULL;
    }

    virtual void saveValue() = 0;
    virtual void resetValue() = 0;
    IPLProcessProperty* processProperty() { return _processProperty; }

signals:
    void changed();
private:
    IPLProcessProperty* _processProperty;

};

#endif // IPPropertyWidget_H
