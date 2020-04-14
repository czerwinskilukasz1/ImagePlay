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

#include "IPLLineFilter.h"

void IPLLineFilter::init()
{
    // init
    _result         = NULL;

    // basic settings
    setClassName("IPLLineFilter");
    setTitle("Line Filter");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("Filters line segments by length.");

    // inputs and outputs
    addInput("Lines", IPL_LINES);
    addOutput("Filtered Lines", IPL_LINES);

    // properties
    addProcessPropertyDouble("minLength", "Min. Length", "0 for no min", 1, IPL_WIDGET_SLIDER, 0, 1000);
    addProcessPropertyDouble("maxLength", "Max. Length", "0 for no max", 0, IPL_WIDGET_SLIDER, 0, 1000);
}

void IPLLineFilter::destroy()
{
    delete _result;
}

bool IPLLineFilter::processInputData(IPLData* data, int, bool)
{
    IPLLines* lines = data->toLines();

    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    int minLength           = getProcessPropertyDouble("minLength");
    int maxLength           = getProcessPropertyDouble("maxLength");

    notifyProgressEventHandler(-1);

    IPLLines* filteredLines = new IPLLines(lines->width(), lines->height());
    for (int i = 0; i < lines->size(); ++i) {
        notifyProgressEventHandler(i/lines->size());

        const cv::Vec4i& line = (*lines)[i];
        int linex = line[2] - line[0];
        int liney = line[3] - line[1];
        float len = sqrtf(linex*linex + liney*liney);
        if ((minLength == 0 || len >= minLength)
                &&
                (maxLength == 0 || len <= maxLength))
        {
            filteredLines->push_back(line);
        }
    }
    _result = filteredLines;

    std::stringstream s;
    s << "Lines before filtering: " << lines->size() << "\n";
    s << "Lines after filtering: " << filteredLines->_lines.size();
    addInformation(s.str());

	return true;
}

/*!
 * \brief IPLLineFilter::getResultData
 * \return
 */
IPLData* IPLLineFilter::getResultData(int index)
{
    return _result;
}
