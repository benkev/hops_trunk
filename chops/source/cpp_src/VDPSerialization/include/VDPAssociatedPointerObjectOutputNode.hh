#ifndef VDPAssociatedPointerObjectOutputNode_HH__
#define VDPAssociatedPointerObjectOutputNode_HH__

#include "VDPCallbackTypes.hh"
#include "VDPObjectOutputNode.hh"

namespace vdp{


/**
*
*@file VDPAssociatedPointerObjectOutputNode.hh
*@class VDPAssociatedPointerObjectOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sun Dec 30 23:07:40 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


template<typename CallType, typename ReturnType, const ReturnType* (CallType::*memberFunction)() const>
class VDPAssociatedPointerObjectOutputNode: public VDPObjectOutputNode< ReturnType, VDPIsDerivedFrom< ReturnType, VDPFixedSizeInputOutputObject >::Is >
{
    public:
        VDPAssociatedPointerObjectOutputNode(std::string name, const CallType* call_ptr):VDPObjectOutputNode< ReturnType, VDPIsDerivedFrom< ReturnType, VDPFixedSizeInputOutputObject >::Is >( name )
        {
            VDPConstantReturnByPointerGet< CallType, ReturnType, memberFunction > callback;
            VDPObjectOutputNode< ReturnType, VDPIsDerivedFrom< ReturnType, VDPFixedSizeInputOutputObject >::Is  >::AttachObjectToNode(callback(call_ptr));
        }

        virtual ~VDPAssociatedPointerObjectOutputNode(){;};


    protected:

};


}


#endif /* VDPAssociatedPointerObjectOutputNode_H__ */
