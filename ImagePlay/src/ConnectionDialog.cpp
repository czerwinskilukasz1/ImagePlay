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

#include "ConnectionDialog.h"

#include "IPLData.h"

/*! ConnectionDialog Constructor:
If command is cmdMenu the button is supposed to have sub menu buttons.
If isCheckable is true the button gets a checkbox
If isChecked is true the checkbox gets a little cross
If radioParent is a parent button all of it checkable children act as radio buttons.
If closeOnClick is true the entire menu gets closed after command execution.
*/
ConnectionDialog::ConnectionDialog(IPProcessStep *from, IPProcessStep *to, IPProcessGridScene* scene, QWidget *parent) : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    setMinimumWidth(350);
    setMaximumWidth(350);

    _mainWindow = (MainWindow*) parent;

    _layout = new QGridLayout;
    setLayout(_layout);

    _from = from;
    _to = to;

    _scene = scene;

    // set up GUI
    _layout->addWidget(new QLabel("<b>Source</b>", this), 0, 0);
    _layout->addWidget(new QLabel("<b>Destination</b>", this), 0, 2);

    setStyleSheet("QDialog {border: 1px solid rgb(53,53,53);}");

    _groupFrom = new QButtonGroup;
    _groupTo = new QButtonGroup;

    int rowL = 1;
    bool isDefaultSet = false;
    for(int i=0; i < (int)_from->process()->outputs()->size(); i++)
    {
        QRadioButton* btn = new QRadioButton(QString::fromStdString(_from->process()->outputs()->at(i).name), this);

        const IPLProcessIO& output = _from->process()->outputs()->at(i);
        // check if output is free
        if(output.occupied)
        {
            btn->setEnabled(false);
        }
        else
        {
            bool shouldBeEnabled = false;

            std::vector<IPLProcessIO>* inputs = _to->process()->inputs();
            // set it as enabled iff there is a destination compatible with this output
            for(int j=0; j < (int)inputs->size() && !shouldBeEnabled; j++)
            {
                const IPLProcessIO& input = inputs->at(j);
                if (IPLData::isConvertibleTo(output.type, input.type))
                {
                    shouldBeEnabled = true;

                    // set as checked a default option; please note we might have no default checked if none of the options are valid
                    if(!isDefaultSet)
                    {
                        btn->setChecked(true);
                        isDefaultSet = true;
                    }
                }
            }

            if (!shouldBeEnabled)
            {
                btn->setEnabled(false);
            }
        }

        _layout->addWidget(btn, rowL++, 0);
        _groupFrom->addButton(btn, i);

        // click event
        connect(btn, &QPushButton::clicked, this, &ConnectionDialog::redrawConnection);

        // special styling
        btn->setStyleSheet(QString("QRadioButton {width: 10px; spacing: -115px; text-align:right;} QRadioButton::indicator { subcontrol-origin: content; subcontrol-position: right top; }"));
    }

    int rowR = 1;
    isDefaultSet = false;
    for(int i=0; i < (int)_to->process()->inputs()->size(); i++)
    {
        QRadioButton* btn = new QRadioButton(QString::fromStdString(_to->process()->inputs()->at(i).name), this);

        const IPLProcessIO& input = _to->process()->inputs()->at(i);
        // check if output is free
        if(input.occupied)
        {
            btn->setEnabled(false);
        }
        else
        {
            bool shouldBeEnabled = false;

            std::vector<IPLProcessIO>* outputs = _from->process()->outputs();
            // set it as enabled iff there is a source compatible with this destination
            for(int j=0; j < (int)outputs->size() && !shouldBeEnabled; j++)
            {
                const IPLProcessIO& output = outputs->at(j);
                if (IPLData::isConvertibleTo(output.type, input.type))
                {
                    shouldBeEnabled = true;

                    // set as checked a default option; please note we might have no default checked if none of the options are valid
                    if(!isDefaultSet)
                    {
                        btn->setChecked(true);
                        isDefaultSet = true;
                    }
                }
            }

            if (!shouldBeEnabled)
            {
                btn->setEnabled(false);
            }
        }

        _layout->addWidget(btn, rowR++, 2);
        _groupTo->addButton(btn, i);

        // click event
        connect(btn, &QPushButton::clicked, this, &ConnectionDialog::redrawConnection);
    }


    _btnAccept = new QPushButton("Connect", this);
    _btnCancel = new QPushButton("Cancel", this);

    int rowB = std::max(rowL, rowR) + 1;
    _layout->addWidget(_btnAccept, rowB, 0);
    _layout->addWidget(_btnCancel, rowB, 2);

    // spacer
    QLabel* spacer = new QLabel("", this);
    spacer->setMinimumWidth(50);
    spacer->setMaximumWidth(50);
    _layout->addWidget(spacer, 0, 1, rowB+1, 1);

    // set default


    // connect events
    connect(_btnAccept, &QPushButton::pressed, this, &ConnectionDialog::on_accept);
    connect(_btnCancel, &QPushButton::pressed, this, &ConnectionDialog::on_cancel);
}

void ConnectionDialog::on_accept()
{
    int indexFrom = _groupFrom->checkedId();
    int indexTo = _groupTo->checkedId();

    IPProcessEdge* newEdge = new IPProcessEdge(_from, _to, indexFrom, indexTo);

    _mainWindow->addEdge(newEdge);

    accept();
}

void ConnectionDialog::on_cancel()
{
    reject();
}

void ConnectionDialog::redrawConnection()
{
    update();
}


void ConnectionDialog::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int indexFrom = _groupFrom->checkedId();
    int indexTo = _groupTo->checkedId();

    if(indexFrom > -1 && indexTo > -1)
    {
        QPointF pointFrom = _groupFrom->button(indexFrom)->pos() + QPointF(_groupFrom->button(indexFrom)->width()-8,6);
        QPointF pointTo = _groupTo->button(indexTo)->pos() + QPointF(8,8);

        QPainterPath path(pointFrom);
        path.cubicTo(pointFrom+QPointF(50,0), pointTo+QPointF(-50,0), pointTo);

        QPen pen;
        pen.setColor(Qt::darkGray);
        pen.setWidth(2);

        painter.setPen(pen);
        painter.drawPath(path);

        _btnAccept->setEnabled(true);
    }
    else
    {
        _btnAccept->setEnabled(false);
    }

    QDialog::paintEvent(e);
}
