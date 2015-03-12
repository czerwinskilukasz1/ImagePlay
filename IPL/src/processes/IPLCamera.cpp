#include "IPLCamera.h"

void IPLCamera::init()
{
    // init
    _result         = NULL;

    // basic settings
    setClassName("IPLCamera");
    setTitle("Capture Camera");
    setDescription("Opens and captures images from the default camera which is connected to your computer. The frame rate depends on the complexity of your process chain, the next frame is captured only after all processing of the previous frame is finished.");
    setCategory(IPLProcess::CATEGORY_IO);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setIsSource(true);
    setIsSequence(true);

    // inputs and outputs
    addOutput("Image", IPLData::IMAGE_COLOR);

    // all properties which can later be changed by gui
    addProcessPropertyInt("width", "Width", "", IPL_INT_SLIDER, 640, 640, 1920);
    addProcessPropertyInt("height", "Height", "", IPL_INT_SLIDER, 480, 480, 1080);
    addProcessPropertyInt("brightness", "Brightness", "", IPL_INT_SLIDER, 128, 0, 255);
    addProcessPropertyInt("contrast", "Contrast", "", IPL_INT_SLIDER, 128, 0, 255);
    addProcessPropertyDouble("exposure", "Exposure", "", IPL_DOUBLE_SLIDER, 0.5, 0.0, 1.0);
}

void IPLCamera::destroy()
{
    delete _result;

    // the camera should be deinitialized automatically in VideoCapture destructor
    IPLCameraIO::release();
}

bool IPLCamera::processInputData(IPLImage*, int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    /*int width = getProcessPropertyInt("width");
    int height = getProcessPropertyInt("height");
    int brightness = getProcessPropertyInt("brightness");
    int contrast = getProcessPropertyInt("contrast");
    double exposure = getProcessPropertyFloat("exposure");*/

    // set camera properties
    /*_camera->set(CV_CAP_PROP_FRAME_WIDTH, width);
    _camera->set(CV_CAP_PROP_FRAME_HEIGHT, height);
    _camera->set(CV_CAP_PROP_BRIGHTNESS, brightness);
    _camera->set(CV_CAP_PROP_CONTRAST, contrast);
    _camera->set(CV_CAP_PROP_EXPOSURE, exposure);*/

    //notifyProgressEventHandler(99);

    //delete _camera;
    //_camera = NULL;

    _result = IPLCameraIO::grabFrame();

    // collect information
    /*std::stringstream s;
    s << "<b>Width: </b>" << _camera->get(CV_CAP_PROP_FRAME_WIDTH) << "\n";
    s << "<b>Height: </b>" << _camera->get(CV_CAP_PROP_FRAME_HEIGHT) << "\n";
    s << "<b>Brightness: </b>" << _camera->get(CV_CAP_PROP_BRIGHTNESS) << "\n";
    s << "<b>Contrast: </b>" << _camera->get(CV_CAP_PROP_CONTRAST) << "\n";
    s << "<b>Saturation: </b>" << _camera->get(CV_CAP_PROP_SATURATION) << "\n";
    s << "<b>Hue: </b>" << _camera->get(CV_CAP_PROP_HUE) << "\n";
    s << "<b>Gain: </b>" << _camera->get(CV_CAP_PROP_GAIN) << "\n";
    s << "<b>Exposure: </b>" << _camera->get(CV_CAP_PROP_EXPOSURE) << "\n";
    s << "<b>GUID: </b>" << _camera->get(CV_CAP_PROP_GUID) << "";

    addInformation(s.str());*/

    return true;
}

IPLImage *IPLCamera::getResultData(int)
{
    return _result;
}
