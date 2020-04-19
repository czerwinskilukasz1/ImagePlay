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

#ifndef IPLLine_H
#define IPLLine_H

#include "IPL_global.h"

#include "opencv2/core/matx.hpp"

#include <string>

/**
 * @brief The IPLLine class represents a 2D line.
 */
class IPLSHARED_EXPORT IPLLine
{
public:
    IPLLine(const IPLLine& line);
    IPLLine(const cv::Vec4i& line);
    IPLLine& operator=(const IPLLine &other);
    ~IPLLine();

    std::string toString() const;

    ipl_basetype len() const;
    ipl_basetype len();


protected:
    /**
     * @brief _line = [x1, y1, x2, y2]
     */
    cv::Vec4i _line;

    ipl_basetype _length;
    bool _length_calculated;

    int& x1() { return _line[0]; }
    int& y1() { return _line[1]; }
    int& x2() { return _line[2]; }
    int& y2() { return _line[3]; }

    const int& x1() const { return _line[0]; }
    const int& y1() const { return _line[1]; }
    const int& x2() const { return _line[2]; }
    const int& y2() const { return _line[3]; }

};

#endif // IPLLine_H
