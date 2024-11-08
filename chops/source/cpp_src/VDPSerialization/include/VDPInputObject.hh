#ifndef VDPInputObject_HH__
#define VDPInputObject_HH__




namespace vdp{

/**
*
*@file VDPInputObject.hh
*@class VDPInputObject
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 19:24:13 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPInputNode;

class VDPInputObject
{
    public:
        VDPInputObject(){;};
        virtual ~VDPInputObject(){;};

        virtual void Initialize(){;};

        //defines the children to add to the node associated with this object
        virtual void DefineInputNode(VDPInputNode* node) = 0;

    protected:

};



}

#endif /* VDPInputObject_H__ */
