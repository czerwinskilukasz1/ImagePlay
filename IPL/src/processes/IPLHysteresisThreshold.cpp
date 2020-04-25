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

#include "IPLHysteresisThreshold.h"

void IPLHysteresisThreshold::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLHysteresisThreshold");
    setTitle("Hysteresis Threshold");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setDescription("Hysteresis thresholding of edge pixels. Starting at pixels with a "
                   "value greater than the high threshold, trace a connected sequence "
                   "of pixels that have a value greater than the low threhsold.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_BW);

    // properties
    addProcessPropertyDouble("lowThreshold", "Low Threshold", "", 0.3, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyDouble("highThreshold", "High Threshold", "", 0.6, IPL_WIDGET_SLIDER, 0.0, 1.0);
}

void IPLHysteresisThreshold::destroy()
{
    delete _result;
}

bool IPLHysteresisThreshold::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    if( image->type() == IPL_IMAGE_GRAYSCALE )
        _result = new IPLImage( IPL_IMAGE_BW, width, height );
    else
        _result = new IPLImage( image->type(), width, height );

    // get properties
    float lowThreshold = getProcessPropertyDouble("lowThreshold");
    float highThreshold = getProcessPropertyDouble("highThreshold");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
               if(plane->p(x,y) >= highThreshold)
                   trace(x, y, plane, newplane, lowThreshold);
            }
        }
    }
    return true;
}

int IPLHysteresisThreshold::trace(int x, int y, IPLImagePlane* plane, IPLImagePlane* image, float lowThreshold)
{
    int width = plane->width();
    int height = plane->height();
    Queue queue;
    queue.push_back(Pixel(x, y));
    while(!queue.empty())
    {
        Pixel c = queue.front();
        queue.pop_front();

        if(c.x < width && c.x >= 0.0 && c.y < height && c.y >= 0.0)
        {
            if(plane->p(c.x,c.y)>lowThreshold && (image->p(c.x,c.y) == 0.0))
            {
                image->p(c.x,c.y) = 1.0;
                queue.push_back( Pixel( c.x+1, c.y ) );
                queue.push_back( Pixel( c.x, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y ) );
                queue.push_back( Pixel( c.x, c.y-1 ) );
                queue.push_back( Pixel( c.x+1, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y-1 ) );
                queue.push_back( Pixel( c.x+1, c.y-1 ) );
            }
        }
    }
    return 0;
}

IPLData* IPLHysteresisThreshold::getResultData( int )
{
    return _result;
}
