#ifndef VDPInputOutputObject_HH__
#define VDPInputOutputObject_HH__

#include "VDPInputObject.hh"
#include "VDPOutputObject.hh"

namespace vdp{


/**
*
*@file VDPInputOutputObject.hh
*@class VDPInputOutputObject
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Tue Jan  8 09:31:01 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPInputOutputObject: public VDPInputObject, public VDPOutputObject
{
    public:
        VDPInputOutputObject(){};
        virtual ~VDPInputOutputObject(){};

//       //inherits these functions
//       virtual void Initialize(){;};
//       virtual void DefineOutputNode(VDPOutputNode* node) const = 0;
//       virtual void DefineInputNode(VDPInputNode* node) = 0;

    protected:

};

}//end of vdp

#endif /* VDPInputOutputObject_H__ */
