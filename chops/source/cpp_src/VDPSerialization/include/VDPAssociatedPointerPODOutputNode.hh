#ifndef VDPAssociatedPointerPODOutputNode_HH__
#define VDPAssociatedPointerPODOutputNode_HH__


#include "VDPCallbackTypes.hh"
#include "VDPPODOutputNode.hh"

namespace vdp{

#define AddVDPOutputForPointer(class,var,type) \
  node->AddChild(new VDPAssociatedPointerPODOutputNode<class, type, &class::Get ## var>( std::string(#var), this) )

/**
*
*@file VDPAssociatedPointerPODOutputNode.hh
*@class VDPAssociatedPointerPODOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 21:12:33 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


template<typename CallType, typename ReturnType, const ReturnType* (CallType::*memberFunction)() const>
class VDPAssociatedPointerPODOutputNode: public VDPPODOutputNode< ReturnType >
{
    public:

        VDPAssociatedPointerPODOutputNode(std::string name, const CallType* call_ptr):VDPPODOutputNode< ReturnType >( name )
        {
            VDPConstantReturnByPointerGet< CallType, ReturnType, memberFunction > callback;
            VDPPODOutputNode< ReturnType >::SetValue(callback(call_ptr));
        }

        virtual ~VDPAssociatedPointerPODOutputNode(){;};

    protected:

};



}//end of vdp namespace

#endif /* VDPAssociatedPointerPODOutputNode_H__ */
