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

#ifndef IPProcessInputOutputWidget_H
#define IPProcessInputOutputWidget_H

#include <QWidget>

#include "IPL_processes.h"
#include "MainWindow.h"

//-----------------------------------------------------------------------------
//!IPProcessInputOutputWidget displays errors and messages for IPLProcess
/*!
 *
 */
class IPProcessInputOutputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit                    IPProcessInputOutputWidget  (QWidget *parent = 0);
    void                        init                        (IPProcessStep* process);
    void                        updateInputsOutputs         ();

private:
    IPProcessStep*              _processStep;
    MainWindow*                 _mainWindow;
};

#endif // IPProcessInputOutputWidget_H

