#ifndef VDPOutputObject_HH__
#define VDPOutputObject_HH__



namespace vdp{

/**
*
*@file VDPOutputObject.hh
*@class VDPOutputObject
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 19:24:13 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPOutputNode;

class VDPOutputObject
{
    public:
        VDPOutputObject(){;};
        virtual ~VDPOutputObject(){;};

        //defines the children to add to the node associated with this object
        virtual void DefineOutputNode(VDPOutputNode* node) const = 0;

    protected:

};



}

#endif /* VDPOutputObject_H__ */
