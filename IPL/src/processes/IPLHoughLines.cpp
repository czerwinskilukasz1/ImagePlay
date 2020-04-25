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

#include "IPLHoughLines.h"

void IPLHoughLines::init()
{
    // init
    _overlay        = NULL;
    _result         = NULL;

    // basic settings
    setClassName("IPLHoughLines");
    setTitle("Hough Lines");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("Finds lines in a binary image using the standard Hough transform.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_GRAYSCALE);
    addOutput("Hough Result", IPL_IMAGE_GRAYSCALE);
    addOutput("Line Overlay", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyDouble("rho", "Rho", "Distance resolution of the accumulator in pixels", 1, IPL_WIDGET_SLIDER, 0, 10);
    addProcessPropertyDouble("theta", "Theta", "Angle resolution of the accumulator in radians.", 0.01, IPL_WIDGET_SLIDER, 0, 5.14);
    addProcessPropertyInt("threshold", "Threshold", "Accumulator threshold parameter.", 1, IPL_WIDGET_SLIDER, 1, 1000);
    //addProcessPropertyInt("minLenght", "Min. Length", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
    //addProcessPropertyInt("maxLineGap", "Max. Line Gap", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
}

void IPLHoughLines::destroy()
{
    //delete _result;
}
bool IPLHoughLines::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;
    delete _overlay;
    _overlay = NULL;

    // WARNING: cv::HoughLines does not work in debug mode!!!
    //          destroys the std::vector<cv::Vec4i> lines;
//#ifdef _DEBUG
//    addError("cv::HoughLines does not work in debug mode");
//    return false;
//#endif

    // get properties
    double rho              = getProcessPropertyDouble("rho");
    double theta            = getProcessPropertyDouble("theta");
    int threshold           = getProcessPropertyInt("threshold");
    //int minLength           = getProcessPropertyInt("minLenght");
    //int maxLineGap          = getProcessPropertyInt("maxLineGap");

    notifyProgressEventHandler(-1);
    cv::Mat result = cv::Mat(image->height(), image->width(), CV_8UC1);
    result = cv::Scalar(0);
    cv::Mat input = image->toCvMat();

    std::vector<cv::Vec2f> lines;
    cv::HoughLines(input, lines, rho, theta, threshold);
    notifyProgressEventHandler(50);

    std::stringstream s;
    s << "Lines found: ";
    s << lines.size();
    addInformation(s.str());

    cv::Mat overlay = input.clone();
    overlay.convertTo(overlay, CV_8UC3);

    for(int i = 0; i < (int) lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
       cv::line(overlay, pt1, pt2, cv::Scalar(0,0,255), 2, cv::LINE_AA);

       // raw result
       cv::line(result, pt1, pt2, cv::Scalar(255), 1, cv::LINE_AA);
     }
    notifyProgressEventHandler(90);

    _overlay = new IPLImage(overlay);
    _result = new IPLImage(result);

	return true;
}

/*!
 * \brief IPLHoughLines::getResultData
 *        index == 0: "Hough Result", IPL_IMAGE_GRAYSCALE
 *        index == 1: "Circle Overlay", IPL_IMAGE_COLOR
 * \return
 */
IPLData* IPLHoughLines::getResultData(int index)
{
    if(index == 0)
        return _result;
    else
        return _overlay;
}
