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

#ifndef IPProcessList_H
#define IPProcessList_H

#include <QListWidget>
#include <QDrag>
#include <QMimeData>
#include <QtGui>
#include <QPixmap>
#include <QPainter>

#include "IPLProcess.h"
#include "MainWindow.h"

//-----------------------------------------------------------------------------
//!Custom process list
/*!
 * Renders a process list with icon and process name.
 * Provides filtering by process name.
 */
class IPProcessList : public QListWidget
{
    Q_OBJECT
public:
    explicit            IPProcessList   (MainWindow* mainWindow, QWidget *parent = 0);
    void                filter          (QString text);
    void                addProcessItem  (QString processID, QString text, QString keywords, IPLProcess::IPLProcessCategory category);

private:
    std::vector<QColor> _categoryColors;
    MainWindow*         _mainWindow;

    // QAbstractItemView interface
protected:
    void                startDrag       (Qt::DropActions supportedActions);
};

#endif // IPProcessList_H
