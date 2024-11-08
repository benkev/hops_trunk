#ifndef VDPAssociatedPointerPODInputNode_HH__
#define VDPAssociatedPointerPODInputNode_HH__

#include "VDPPODInputNode.hh"
#include "VDPCallbackTypes.hh"

#define AddVDPInputForPointer(class,var,type) \
  node->AddChild(new VDPAssociatedPointerPODInputNode< class, type, &class::Set ## var>(std::string(#var), this) )

namespace vdp{


/**
*
*@file VDPAssociatedPointerPODInputNode.hh
*@class VDPAssociatedPointerPODInputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Jan  3 22:10:43 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/



template< typename CallType, typename SetType, void (CallType::*memberFunction)(const SetType*) >
class VDPAssociatedPointerPODInputNode: public VDPPODInputNode<SetType>
{
    public:

        VDPAssociatedPointerPODInputNode(std::string name, CallType* call_ptr):VDPPODInputNode< SetType >(name)
        {
            fCallPtr = call_ptr;
        };

        virtual ~VDPAssociatedPointerPODInputNode(){;};

        void FinalizeObject()
        {
            fCallback(fCallPtr, &(this->fValue) );
        }

    protected:

        CallType* fCallPtr;
        VDPPassByConstantPointerSet< CallType, SetType, memberFunction > fCallback;

};


}

#endif /* VDPAssociatedPointerPODInputNode_H__ */
