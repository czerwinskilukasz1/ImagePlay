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

#include "IPLLine.h"

#include <sstream>

IPLLine::IPLLine(const IPLLine &other):
    _line(other._line)
{}

IPLLine::IPLLine(const cv::Vec4i& line):
    _line(line)
{}

IPLLine &IPLLine::operator=(const IPLLine &other)
{
    _line = cv::Vec4i(other._line);
    return *this;
}

IPLLine::~IPLLine()
{}

std::string IPLLine::toString() const
{
    std::stringstream s;
    s << "IPLLine((" << _line[0] << ", " << _line[1] << "), (" << _line[2] << ", " << _line[3] <<"))";

    return s.str();
}


ipl_basetype IPLLine::len() const
{
    if (!_length_calculated)
    {
        int x = x2()-x1();
        int y = y2()-y1();
        return sqrt(x*x + y*y);
    } else {
        return _length;
    }
}

ipl_basetype IPLLine::len()
{
    if (!_length_calculated)
    {
        const IPLLine* const _this = this;
        _length = _this->len();
        _length_calculated = true;
    }
    return _length;
}
