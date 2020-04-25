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

#include "IPLHoughLineSegments.h"

#include "IPLLines.h"

void IPLHoughLineSegments::init()
{
    // init
    _overlay        = NULL;
    _result         = NULL;
    _lines          = NULL;

    // basic settings
    setClassName("IPLHoughLineSegments");
    setTitle("Hough Line Segments");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("Finds line segments in a binary image using the standard Hough transform.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_GRAYSCALE);
    addOutput("Hough Result", IPL_IMAGE_GRAYSCALE);
    addOutput("Line Overlay", IPL_IMAGE_COLOR);
    addOutput("Line Data", IPL_LINES);

    // properties
    addProcessPropertyDouble("rho", "Rho", "Distance resolution of the accumulator in pixels", 1, IPL_WIDGET_SLIDER, 0, 10);
    addProcessPropertyDouble("theta", "Theta", "Angle resolution of the accumulator in radians.", 0.01, IPL_WIDGET_SLIDER, 0, 5.14);
    addProcessPropertyInt("threshold", "Threshold", "Accumulator threshold parameter.", 1, IPL_WIDGET_SLIDER, 1, 1000);
    addProcessPropertyInt("minLength", "Min. Length", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
    addProcessPropertyInt("maxLineGap", "Max. Line Gap", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
}

void IPLHoughLineSegments::destroy()
{
    //delete _result;
}
bool IPLHoughLineSegments::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;
    delete _overlay;
    _overlay = NULL;
    delete _lines;
    _lines = NULL;

    std::vector<cv::Vec4i> lines;

    // WARNING: cv::HoughLinesP does not work in debug mode!!!
    //          destroys the std::vector<cv::Vec4i> lines;
//#ifdef _DEBUG
//    addError("cv::HoughLinesP does not work in debug mode");
//    return false;
//#endif

    // get properties
    double rho              = getProcessPropertyDouble("rho");
    double theta            = getProcessPropertyDouble("theta");
    int threshold           = getProcessPropertyInt("threshold");
    int minLength           = getProcessPropertyInt("minLength");
    int maxLineGap          = getProcessPropertyInt("maxLineGap");

    notifyProgressEventHandler(-1);
    cv::Mat input = image->toCvMat();
    cv::Mat overlay = input.clone();
    cv::Mat result = cv::Mat(image->height(), image->width(), CV_8UC1);
    result = cv::Scalar(0);
    overlay.convertTo(overlay, CV_8UC3);

    cv::HoughLinesP(input, lines, rho, theta, threshold, minLength, maxLineGap);
    notifyProgressEventHandler(50);

    std::stringstream s;
    s << "Lines found: ";
    s << lines.size();
    addInformation(s.str());

    for(int i = 0; i < (int) lines.size(); i++ )
    {
       cv::Vec4i l = lines[i];
       cv::line(overlay, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 2, cv::LINE_AA);

       // raw result
       cv::line(result, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255), 1, cv::LINE_AA);
     }
    notifyProgressEventHandler(90);

    _overlay = new IPLImage(overlay);
    _result = new IPLImage(result);
    _lines = new IPLLines(lines, image->width(), image->height());
    
	return true;
}

/*!
 * \brief IPLHoughLineSegments::getResultData
 *        index == 0: "Hough Result", IPL_IMAGE_GRAYSCALE
 *        index == 1: "Circle Overlay", IPL_IMAGE_COLOR
 *        index == 2: "Line Segments", IPL_LINES
 * \return
 */
IPLData* IPLHoughLineSegments::getResultData(int index)
{
    switch(index) {
    case 0:
        return _result;
    case 1:
        return _overlay;
    case 2:
        return _lines;
    default:
        addError("Wrong index");
        return NULL;
    }
}
