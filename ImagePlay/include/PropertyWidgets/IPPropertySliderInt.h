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

#ifndef IPPropertySliderInt_H
#define IPPropertySliderInt_H

#include <QWidget>
#include <QSlider>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertySliderInt : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertySliderInt(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        _lastValue = 0;

        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _property = property;

        _slider = new QSlider(Qt::Horizontal, this);
        _spinner = new QSpinBox(this);

        layout()->addWidget(_slider);
        layout()->addWidget(_spinner);

        init();

        //connect(_slider, &QSlider::sliderReleased, this, &IPPropertySliderInt::updateValue );
        connect(_slider, &QSlider::valueChanged, this, &IPPropertySliderInt::onSliderChanged );
        connect(_spinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IPPropertySliderInt::onSpinnerChanged);

    }

    void init()
    {
        int min   = _property->min();
        int max   = _property->max();
        int value = _property->value();

        _slider->setMinimum(min);
        _slider->setMaximum(max);
        _slider->setValue(value);
        _slider->setSingleStep(1);

        _spinner->setMinimum(min);
        _spinner->setMaximum(max);
        _spinner->setValue(value);
    }

    void setMinimum(int v)  { _slider->setMinimum(v);  _spinner->setMinimum(v); }
    void setMaximum(int v)  { _slider->setMaximum(v);  _spinner->setMaximum(v); }
    int value()             { return _slider->value(); }
    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

signals:

public slots:
    void onSliderChanged(int v)
    {
        _spinner->setValue(v);

        updateValue();
    }

    void onSpinnerChanged(int v)
    {
        _slider->setValue(v);
    }

    void updateValue()
    {
        int v = _slider->value();

        // prevent double changes
        if(v == _lastValue)
        {
            return;
        }

        if(_property)
            saveValue();

        emit changed();

        _lastValue = v;
    }

private:
    IPLProcessPropertyInt* _property;
    QSlider* _slider;
    QSpinBox* _spinner;
    int _lastValue;

};

#endif // IPPropertySliderInt_H
