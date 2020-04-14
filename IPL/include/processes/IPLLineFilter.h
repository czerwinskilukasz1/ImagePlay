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

#ifndef IPLLineFilter_H
#define IPLLineFilter_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLLines.h"

#include <string>
#include <deque>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLLineFilter class. Filters lines by length.
 */
class IPLSHARED_EXPORT IPLLineFilter : public IPLClonableProcess<IPLLineFilter>
{
public:
                            IPLLineFilter() : IPLClonableProcess() { init(); }
                            ~IPLLineFilter()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLData*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLLines*               _result;
};

#endif // IPLLineFilter_H
