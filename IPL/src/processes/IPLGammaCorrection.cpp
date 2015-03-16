#include "IPLGammaCorrection.h"

void IPLGammaCorrection::init()
{
    // init
    _result     = NULL;
    _gamma      = 1.0;

    // basic settings
    setClassName("IPLGammaCorrection");
    setTitle("Gamma Correction");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLImage::IMAGE_BW);

    // properties
    addProcessPropertyDouble("gamma", "Gamma", "0.0 < threshold < 1.0", IPL_DOUBLE_SLIDER, _gamma, 0.1, 10.0);
}

void IPLGammaCorrection::destroy()
{
    delete _result;
}

bool IPLGammaCorrection::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    double gamma = getProcessPropertyDouble("gamma");
    if(gamma == 0)
        gamma = 1.0;

    double inverse_gamma = 1.0 / gamma;

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        {
            for(int y=0; y<height; y++)
            {
                // progress
                notifyProgressEventHandler(100*progress++/maxProgress);
                for(int x=0; x<width; x++)
                {
                    double value = pow(plane->p(x,y), inverse_gamma);
                    newplane->p(x,y) = value;
                }
            }
        }
    }

    return true;
}

IPLImage* IPLGammaCorrection::getResultData( int )
{
    return _result;
}
