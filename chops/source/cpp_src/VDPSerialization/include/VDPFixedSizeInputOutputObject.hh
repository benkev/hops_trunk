#ifndef VDPFixedSizeInputOutputObject_HH__
#define VDPFixedSizeInputOutputObject_HH__

#include "VDPInputOutputObject.hh"

namespace vdp{

/**
*
*@file VDPFixedSizeInputOutputObject.hh
*@class VDPFixedSizeInputOutputObject
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Jan 31 15:02:11 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/


class VDPFixedSizeInputOutputObject: public VDPInputOutputObject
{
    public:
        VDPFixedSizeInputOutputObject(){;};
        virtual ~VDPFixedSizeInputOutputObject(){;};

        //we probably ought to add a function which declares the size (number of items) in this classes

//        //inherits these functions, which are overloaded in the derived class
//        virtual void Initialize(){;};
//        virtual void DefineOutputNode(VDPOutputNode* node) const = 0;
//        virtual void DefineInputNode(VDPInputNode* node) = 0;

    protected:

};


}


#endif /* VDPFixedSizeInputOutputObject_H__ */
