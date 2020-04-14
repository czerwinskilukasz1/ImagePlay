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

#include "IPLLines.h"

#include <sstream>


IPLLines::IPLLines(const std::vector<cv::Vec4i>& lines, int width, int height):
    IPLData(IPL_LINES),
    _lines(lines),
    _width(width),
    _height(height)
{}

IPLLines::IPLLines(int width, int height):
    IPLData(IPL_LINES),
    _width(width),
    _height(height)
{}

IPLLines::IPLLines(const IPLLines &other):
    IPLData(IPL_LINES),
    _lines(std::vector<cv::Vec4i>(other._lines)),
    _width(other._width),
    _height(other._height)
{}

//IPLLines &IPLLines::operator=(const IPLLines &other)
//{
//    for(int i = 0; i < other._lines.size(); ++i) {
//        cv::Vec4i lineCopy = cv::Vec4i(other._lines[i]);
//        _lines.push_back(lineCopy);
//    }
//    return *this;
//}

IPLLines::~IPLLines()
{}

std::string IPLLines::toString() const
{
    std::stringstream s;
    s << "IPLLines(" << _width << "x" << _height << ", " << _lines.size() << " line(s))";

    return s.str();
}


//cv::Vec4i& IPLLines::line(int i)
//{
//    return _lines[i];
//}
