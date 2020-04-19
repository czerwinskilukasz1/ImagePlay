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

#ifndef IPLLines_H
#define IPLLines_H

#include "IPL_global.h"

#include "IPLData.h"

#include "opencv2/core/matx.hpp"

#include <vector>

class IPLLineFilter;

/**
 * @brief The IPLLines class contains a vector of 2D lines.
 */
class IPLSHARED_EXPORT IPLLines : public IPLData
{
public:
    /**
     * @brief Creates a new IPLLines object with a copy of each line from the vector given.
     */
    IPLLines(const std::vector<cv::Vec4i>& lines, int width, int height);
    IPLLines(int width, int height);
    IPLLines(const IPLLines& lines);
//    IPLLines& operator=(const IPLLines &other);
    ~IPLLines();

    int size() const { return _lines.size(); }
    const cv::Vec4i& operator[](int i) const { return _lines[i]; }

//    cv::Vec4i& line(int i);
//    const cv::Vec4i& line(int i) const;

    void push_back(const cv::Vec4i& line) { _lines.push_back(line); }


    std::string toString() const;

    int width(void)                 { return _width; }
    int height(void)                { return _height; }

protected:
    int _width;
    int _height;
    std::vector<cv::Vec4i> _lines;

    friend class IPLLineFilter;
};

#endif // IPLLines_H
