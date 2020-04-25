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

#include "IPLImage.h"

int IPLImage::_instanceCount = 0;

IPLImage::IPLImage() : IPLData(IPL_UNDEFINED)
{
    _width = 0;
    _height = 0;
    _nrOfPlanes = 0;

    _instanceCount++;
}

IPLImage::IPLImage(const IPLImage& other):
    IPLData(other)
{
    _type = other._type;
    _width = other._width;
    _height = other._height;

    if( _type == IPL_IMAGE_COLOR ) _nrOfPlanes = 3; else _nrOfPlanes = 1;
    for( int i=0; i<_nrOfPlanes; i++ )
        _planes.push_back(new IPLImagePlane( *(other._planes[i]) ));

    _instanceCount++;
}

IPLImage::IPLImage( IPLDataType t, int width, int height )
{
    _type = t;
    _width = width;
    _height = height;

    if( _type == IPL_IMAGE_COLOR ) _nrOfPlanes = 3; else _nrOfPlanes = 1;
    for( int i=0; i<_nrOfPlanes; i++ )
        _planes.push_back(new IPLImagePlane( width, height ));
    fillColor( 0 );

    _instanceCount++;
}

IPLImage::IPLImage(const cv::Mat &cvMat)
{
    // _type = other._type;
    _width  = cvMat.cols;
    _height = cvMat.rows;
    _type   = IPL_IMAGE_COLOR;
    _nrOfPlanes = cvMat.channels();
    if(_nrOfPlanes > 1)
        _type   = IPL_IMAGE_COLOR;
    else
        _type   = IPL_IMAGE_GRAYSCALE;

    if(_nrOfPlanes > 3)
        _nrOfPlanes = 3;

    for(int i=0; i<_nrOfPlanes; i++)
    {
        _planes.push_back(new IPLImagePlane(_width, _height));
    }

    if(cvMat.channels() == 1)
    {
        cvMat.convertTo(cvMat, CV_8UC1);

        for(int y = 0; y<_height; y++)
        {
            for(int x = 0; x<_width; x++)
            {
                _planes[0]->p(x,y) = cvMat.at<uchar>(y,x) * FACTOR_TO_FLOAT;
            }
        }
    }
    else if(cvMat.channels() == 2)
    {
        cvMat.convertTo(cvMat, CV_8UC2);

        for(int y = 0; y<_height; y++)
        {
            for(int x = 0; x<_width; x++)
            {
                _planes[0]->p(x,y) = cvMat.at<cv::Vec2b>(y,x).val[0] * FACTOR_TO_FLOAT;
                _planes[1]->p(x,y) = cvMat.at<cv::Vec2b>(y,x).val[1] * FACTOR_TO_FLOAT;
            }
        }
    }
    else if(cvMat.channels() == 3)
    {
        cvMat.convertTo(cvMat, CV_8UC3);

        for(int y = 0; y<_height; y++)
        {
            for(int x = 0; x<_width; x++)
            {
                _planes[0]->p(x,y) = cvMat.at<cv::Vec3b>(y,x).val[2] * FACTOR_TO_FLOAT;   // R
                _planes[1]->p(x,y) = cvMat.at<cv::Vec3b>(y,x).val[1] * FACTOR_TO_FLOAT;   // G
                _planes[2]->p(x,y) = cvMat.at<cv::Vec3b>(y,x).val[0] * FACTOR_TO_FLOAT;   // B
            }
        }
    }
    else if(cvMat.channels() == 4)
    {
        cvMat.convertTo(cvMat, CV_8UC4);

        for(int y = 0; y<_height; y++)
        {
            for(int x = 0; x<_width; x++)
            {
                _planes[0]->p(x,y) = cvMat.at<cv::Vec4b>(y,x).val[2] * FACTOR_TO_FLOAT;   // R
                _planes[1]->p(x,y) = cvMat.at<cv::Vec4b>(y,x).val[1] * FACTOR_TO_FLOAT;   // G
                _planes[2]->p(x,y) = cvMat.at<cv::Vec4b>(y,x).val[0] * FACTOR_TO_FLOAT;   // B
            }
        }
    }

    _instanceCount++;
}

IPLImage::~IPLImage()
{
    for( int i=0; i<_nrOfPlanes; i++ )
        if( _planes[i] ) delete _planes[i];

    _instanceCount--;
}

IPLImagePlane* IPLImage::plane(int planeNr)
{
    if(planeNr >= (int)_planes.size() || planeNr < 0)
    {
        std::stringstream error;
        error << ("Invalid plane number: ");
        error << planeNr;
        throw std::runtime_error(error.str());
        return NULL;
    }

    return _planes[planeNr];
}

void IPLImage::fillColor(ipl_basetype value)
{
    for( int planeNr = 0; planeNr < _nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = _planes[planeNr];
        for( int x=0; x<_width; x++ )
            for( int y=0; y<_height; y++ )
                plane->p(x,y) = value;
    }
}

std::string IPLImage::toString(int x, int y)
{
    std::stringstream s;
    s << plane(0)->cp(x, y);
    for(int i=1; i<_nrOfPlanes; i++)
    {
        s << ", " << plane(i)->cp(x, y);
    }

    return s.str();
}

cv::Mat IPLImage::toCvMat()
{
    // determine dimensions of the output image

    int noOfOutputPlanes;

    if (_type == IPL_IMAGE_ORIENTED)
        noOfOutputPlanes = 4;
    else
        noOfOutputPlanes = getNumberOfPlanes();


    int type;
    switch(noOfOutputPlanes) {
    case 1: type = CV_8UC1; break;
    case 2: type = CV_8UC2; break;
    case 3: type = CV_8UC3; break;
    case 4: type = CV_8UC4; break;
    default: assert(false); // this shouldn't happen as image has always 1-4 dimensions
    }

    // prepare a temporary data structure
    std::vector<uchar> data;
    data.resize(_height * _width * noOfOutputPlanes);


    // rgb32() cannot be used to fill the data because it always returns a 4-channel Mat,
    // even for grayscale/b&w images.
    // We need to fill in Mat manually.

    if(_type == IPL_IMAGE_BW)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                uchar val = plane(0)->p(x,y) * FACTOR_TO_UCHAR;
                val = val < 0x80 ? 0x00 : 0xFF;
                data[i++] = val;
            }
        }
    }
    else if(_type == IPL_IMAGE_GRAYSCALE)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                uchar val = (plane(0)->p(x,y) * FACTOR_TO_UCHAR);
                data[i++] = val;
            }
        }
    }
    else if(_type == IPL_IMAGE_COLOR)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                // TODO: It should actually be faster if we iterate over planes first due to memory cache in the processor.

                uchar r = plane(0)->p(x,y) * FACTOR_TO_UCHAR;
                uchar g = plane(1)->p(x,y) * FACTOR_TO_UCHAR;
                uchar b = plane(2)->p(x,y) * FACTOR_TO_UCHAR;

                data[i++] = b;
                data[i++] = g;
                data[i++] = r;

                if (getNumberOfPlanes() == 4)
                    data[i++] = 0xFF;
            }
        }
    }
    else if(_type == IPL_IMAGE_ORIENTED)
    {
        // Always 4 output planes

        double maxMag = 0.0;
        for(int x=0; x<_width; x++)
            for(int y=0; y<_height; y++)
                if( plane(0)->p(x,y) > maxMag ) maxMag = plane(0)->p(x,y);

        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                ipl_basetype phase = fmod(plane(1)->p(x,y), 1.0);
                ipl_basetype magnitude = plane(0)->p(x,y) / maxMag;

                if(phase < 0)
                    phase = 0;
                if(phase > 1)
                    phase = 1;

                IPLColor color = IPLColor::fromHSV(phase, 1.0,  magnitude);

                uchar r = color.red()   * FACTOR_TO_UCHAR;
                uchar g = color.green() * FACTOR_TO_UCHAR;
                uchar b = color.blue()  * FACTOR_TO_UCHAR;
                data[i++] = b;
                data[i++] = g;
                data[i++] = r;
                data[i++] = 0xFF;
            }
        }
    }

    // create a Mat (this constructor does NOT copy the underlying array)
    cv::Mat mat(_height, _width, type, data.data());

    // We need to get a copy of the data so that we don't use local data.
    // Of course it would be faster to allocate 'data' dynamically but then we would need to
    // take care of deallocating it when the return Mat is gone, which would need some more changes in ImagePlay,
    // e.g. creating a MatHolder type, which deallocates the underlying data on destruction.
    // Premature optimization is evil, so for now we will just leave it like this.
    cv::Mat mat_ret = mat.clone();

    return mat_ret;
}


uchar* IPLImage::rgb32()
{
    _rgb32.resize(_height * _width * 4);
    if(_type == IPL_IMAGE_BW)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                uchar val = plane(0)->p(x,y) * FACTOR_TO_UCHAR;
                val = val < 0x80 ? 0x00 : 0xFF;
                _rgb32[i++] = val;
                _rgb32[i++] = val;
                _rgb32[i++] = val;
                _rgb32[i++] = 0xFF;
            }
        }
    }
    else if(_type == IPL_IMAGE_GRAYSCALE)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                uchar val = (plane(0)->p(x,y) * FACTOR_TO_UCHAR);
                _rgb32[i++] = val;
                _rgb32[i++] = val;
                _rgb32[i++] = val;
                _rgb32[i++] = 0xFF;
            }
        }
    }
    else if(_type == IPL_IMAGE_COLOR)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                uchar r = plane(0)->p(x,y) * FACTOR_TO_UCHAR;
                uchar g = plane(1)->p(x,y) * FACTOR_TO_UCHAR;
                uchar b = plane(2)->p(x,y) * FACTOR_TO_UCHAR;
                _rgb32[i++] = b;
                _rgb32[i++] = g;
                _rgb32[i++] = r;
                _rgb32[i++] = 0xFF;
            }
        }
    }
    else if(_type == IPL_IMAGE_ORIENTED)
    {
        double maxMag = 0.0;
        for(int x=0; x<_width; x++)
            for(int y=0; y<_height; y++)
                if( plane(0)->p(x,y) > maxMag ) maxMag = plane(0)->p(x,y);

        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                ipl_basetype phase = fmod(plane(1)->p(x,y), 1.0);
                ipl_basetype magnitude = plane(0)->p(x,y) / maxMag;

                if(phase < 0)
                    phase = 0;
                if(phase > 1)
                    phase = 1;

                IPLColor color = IPLColor::fromHSV(phase, 1.0,  magnitude);

                uchar r = color.red()   * FACTOR_TO_UCHAR;
                uchar g = color.green() * FACTOR_TO_UCHAR;
                uchar b = color.blue()  * FACTOR_TO_UCHAR;
                _rgb32[i++] = b;
                _rgb32[i++] = g;
                _rgb32[i++] = r;
                _rgb32[i++] = 0xFF;
            }
        }
    }

    return _rgb32.data();
}

void IPLImage::rgb32CleanupHandler(void *info)
{
    /*IPLImage* image = static_cast<IPLImage*>(info);
    if(image)
        image->_rgb32.resize(0);*/
}
