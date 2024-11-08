#ifndef VDPAssociatedValuePODInputNode_HH__
#define VDPAssociatedValuePODInputNode_HH__

#include "VDPPODInputNode.hh"
#include "VDPCallbackTypes.hh"

namespace vdp{


/**
*
*@file VDPAssociatedValuePODInputNode.hh
*@class VDPAssociatedValuePODInputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Jan  3 22:10:43 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/



template< typename CallType, typename SetType, void (CallType::*memberFunction)(SetType) >
class VDPAssociatedValuePODInputNode: public VDPPODInputNode<SetType>
{
    public:

        VDPAssociatedValuePODInputNode(std::string name, CallType* call_ptr):VDPPODInputNode< SetType >(name)
        {
            fCallPtr = call_ptr;
        };

        virtual ~VDPAssociatedValuePODInputNode(){;};

        void FinalizeObject()
        {
            fCallback(fCallPtr, this->fValue);
        }

    protected:

        CallType* fCallPtr;
        VDPPassByValueSet< CallType, SetType, memberFunction > fCallback;

};


}

#endif /* VDPAssociatedValuePODInputNode_H__ */
