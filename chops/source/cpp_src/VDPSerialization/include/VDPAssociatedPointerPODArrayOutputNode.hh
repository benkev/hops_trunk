#ifndef VDPAssociatedPointerPODArrayOutputNode_HH__
#define VDPAssociatedPointerPODArrayOutputNode_HH__

#include "VDPCallbackTypes.hh"
#include "VDPPODArrayOutputNode.hh"

namespace vdp{


/**
*
*@file VDPAssociatedPointerPODArrayOutputNode.hh
*@class VDPAssociatedPointerPODArrayOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 21:12:33 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


template<typename CallType, typename ReturnType, const ReturnType* (CallType::*memberFunction)() const>
class VDPAssociatedPointerPODArrayOutputNode: public VDPPODArrayOutputNode< ReturnType >
{
    public:

        VDPAssociatedPointerPODArrayOutputNode(std::string name, unsigned int arr_size, const CallType* call_ptr):VDPPODArrayOutputNode< ReturnType >(name, arr_size)
        {
            VDPConstantReturnByPointerGet< CallType, ReturnType, memberFunction > callback;
            VDPPODArrayOutputNode< ReturnType >::SetValue(callback(call_ptr));
        }

        virtual ~VDPAssociatedPointerPODArrayOutputNode(){;};

    protected:

};



}//end of vdp namespace

#endif /* VDPAssociatedPointerPODArrayOutputNode_H__ */
