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

#ifndef IPPropertyCombobox_H
#define IPPropertyCombobox_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QComboBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyCombobox : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyCombobox(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        _property = property;

        setLayout(new QVBoxLayout(this));

        _combobox = new QComboBox(this);
        layout()->addWidget(_combobox);
        layout()->setMargin(0);

        // split the properties
        QString rawName(property->title());

        // check if structure is Title:Option1|Option2
        if(!(rawName.contains(":") && rawName.contains("|")))
        {
            qWarning() << "IPPropertyCombobox: Invalid title structure " << rawName;
            return;
        }

        QString name = rawName.split(":").at(0);
        QString rawOptions = rawName.split(":").at(1);
        QStringList options = rawOptions.split("|");

        _combobox->addItems(options);

        init();

        connect(_combobox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &IPPropertyCombobox::valueChanged);
    }

    void init()
    {
        int value = _property->value();

        _combobox->setCurrentIndex(value);
    }

    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    int value()             { return _combobox->currentIndex(); }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

signals:

public slots:
    void valueChanged()
    {
        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyInt*  _property;
    QComboBox*              _combobox;
};

#endif // IPPropertyCombobox_H
