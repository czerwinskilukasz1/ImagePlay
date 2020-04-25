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

#include "IPLAdaptiveThreshold.h"

void IPLAdaptiveThreshold::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLAdaptiveThreshold");
    setTitle("Adaptive Threshold");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setDescription("Adaptive threshold from OpenCV.\n"
                   "For arguments see OpenCV documentation.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_GRAYSCALE);
    addOutput("Image", IPL_IMAGE_BW);

    // properties
    addProcessPropertyInt("maxValue", "maxValue", "", 255, IPL_WIDGET_SLIDER, 0, 255);
    addProcessPropertyInt("adaptiveMethod", "adaptiveMethod:ADAPTIVE_THRESH_MEAN_C|ADAPTIVE_THRESH_GAUSSIAN_C", "", 0, IPL_WIDGET_RADIOBUTTONS);
    addProcessPropertyInt("thresholdType", "thresholdType:THRESH_BINARY|THRESH_BINARY_INV", "", 0, IPL_WIDGET_RADIOBUTTONS);

    addProcessPropertyInt("blockSize", "blockSize", "", 3, IPL_WIDGET_SLIDER_ODD, 3, 500);
    addProcessPropertyDouble("C", "C", "", 0, IPL_WIDGET_SLIDER, 0, 1000);
}

void IPLAdaptiveThreshold::destroy()
{
    delete _result;
}

bool IPLAdaptiveThreshold::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    cv::Mat input = image->toCvMat();
    cv::Mat output;

    // get properties
    int maxValue = getProcessPropertyInt("maxValue");

    cv::AdaptiveThresholdTypes adaptiveMethod;
    switch(getProcessPropertyInt("adaptiveMethod")) {
        case 0: adaptiveMethod = cv::ADAPTIVE_THRESH_MEAN_C; break;
        case 1: adaptiveMethod = cv::ADAPTIVE_THRESH_GAUSSIAN_C; break;
        default: assert(false);
    }

    cv::ThresholdTypes thresholdType;
    switch(getProcessPropertyInt("thresholdType")) {
        case 0: thresholdType = cv::THRESH_BINARY; break;
        case 1: thresholdType = cv::THRESH_BINARY_INV; break;
        default: assert(false);
    }

    int blockSize = getProcessPropertyInt("blockSize");
    double C = getProcessPropertyDouble("C");

    // process input
    notifyProgressEventHandler(-1);
    cv::adaptiveThreshold(input, output, maxValue, adaptiveMethod, thresholdType, blockSize, C);
    notifyProgressEventHandler(60);
    _result = new IPLImage(output);
    notifyProgressEventHandler(100);

    return true;
}

IPLData* IPLAdaptiveThreshold::getResultData( int )
{
    return _result;
}
