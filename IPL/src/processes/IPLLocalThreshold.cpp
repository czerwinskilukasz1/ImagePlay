#include "IPLLocalThreshold.h"

void IPLLocalThreshold::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLLocalThreshold");
    setTitle("Local Threshold");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER, 1, 1, 9);
    addProcessPropertyDouble("aboveMean", "Above Mean", "", IPL_DOUBLE_SLIDER, 0.5, 0.0, 9.0);
}

void IPLLocalThreshold::destroy()
{
    delete _result;
}

bool IPLLocalThreshold::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    if(image->type() == IPLImage::IMAGE_GRAYSCALE)
        _result = new IPLImage(IPLImage::IMAGE_BW, width, height);
    else
        _result = new IPLImage(image->type(), width, height);

    // get properties
    int window = getProcessPropertyInt("window");
    float aboveMean = getProcessPropertyDouble("aboveMean");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        int w2 = window/2;
        double area = (double)window*(double)window;

        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x=0; x<width; x++)
            {
                double localMean = 0.0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        localMean += (double)plane->bp(x+kx,y+ky);
                    }
                }
                localMean /= area;
                double deviation = 0.0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        double diff =  (double)plane->bp(x+kx, y+ky) - localMean;
                        deviation += diff * diff;
                    }
                }
                deviation = sqrt( deviation / area );
                int T = (int) (localMean + aboveMean*deviation);

                newplane->p(x,y) = (plane->p(x,y) >= T) ? 1.0 : 0.0;
            }
        }
    }
    return true;
}

IPLData* IPLLocalThreshold::getResultData( int )
{
    return _result;
}
