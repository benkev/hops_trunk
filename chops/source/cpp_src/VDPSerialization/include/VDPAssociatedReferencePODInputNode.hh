#ifndef VDPAssociatedReferencePODInputNode_HH__
#define VDPAssociatedReferencePODInputNode_HH__

#include "VDPPODInputNode.hh"
#include "VDPCallbackTypes.hh"

#define AddVDPInputFor(class,var,type) \
  node->AddChild(new VDPAssociatedReferencePODInputNode< class, type, &class::Set ## var>(std::string(#var), this) )

namespace vdp{


/**
*
*@file VDPAssociatedReferencePODInputNode.hh
*@class VDPAssociatedReferencePODInputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Jan  3 22:10:43 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/



template< typename CallType, typename SetType, void (CallType::*memberFunction)(const SetType&) >
class VDPAssociatedReferencePODInputNode: public VDPPODInputNode<SetType>
{
    public:

        VDPAssociatedReferencePODInputNode(std::string name, CallType* call_ptr):VDPPODInputNode< SetType >(name)
        {
            fCallPtr = call_ptr;
        };

        virtual ~VDPAssociatedReferencePODInputNode(){;};

        void FinalizeObject()
        {
            fCallback(fCallPtr, this->fValue);
        }

    protected:

        CallType* fCallPtr;
        VDPPassByConstantReferenceSet< CallType, SetType, memberFunction > fCallback;

};


}

#endif /* VDPAssociatedReferencePODInputNode_H__ */
