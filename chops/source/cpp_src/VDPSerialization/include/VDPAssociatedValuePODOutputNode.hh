#ifndef VDPAssociatedValuePODOutputNode_HH__
#define VDPAssociatedValuePODOutputNode_HH__

#include "VDPCallbackTypes.hh"
#include "VDPPODOutputNode.hh"

#define AddVDPOutputFor(class,var,type) \
  node->AddChild(new VDPAssociatedValuePODOutputNode< class, type, &class::Get ## var>(std::string(#var), this) )

namespace vdp{


/**
*
*@file VDPAssociatedValuePODOutputNode.hh
*@class VDPAssociatedValuePODOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 21:12:33 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


template<typename CallType, typename ReturnType, ReturnType (CallType::*memberFunction)() const>
class VDPAssociatedValuePODOutputNode: public VDPPODOutputNode< ReturnType >
{
    public:
        VDPAssociatedValuePODOutputNode(std::string name, const CallType* call_ptr):VDPPODOutputNode< ReturnType >(name)
        {
            VDPConstantReturnByValueGet< CallType, ReturnType, memberFunction > callback;
            VDPPODOutputNode< ReturnType >::SetValue(callback(call_ptr));
        };

        virtual ~VDPAssociatedValuePODOutputNode(){;};

    protected:

};



}//end of vdp namespace

#endif /* VDPAssociatedValuePODOutputNode_H__ */
