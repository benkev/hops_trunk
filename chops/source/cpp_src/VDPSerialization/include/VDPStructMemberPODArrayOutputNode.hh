#ifndef VDPStructMemberPODArrayOutputNode_HH__
#define VDPStructMemberPODArrayOutputNode_HH__

#include "VDPCallbackTypes.hh"
#include "VDPPODOutputNode.hh"

namespace vdp{


/**
*
*@file VDPStructMemberPODArrayOutputNode.hh
*@class VDPStructMemberPODArrayOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 21:12:33 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

template <typename StructType, typename ReturnType, typename ArrayType, ArrayType StructType::*Member >
class VDPStructMemberPODArrayOutputNode: public VDPPODArrayOutputNode< ReturnType >
{
    public:

        VDPStructMemberPODArrayOutputNode(std::string name, unsigned int arr_size, const StructType* call_ptr ):VDPPODArrayOutputNode< ReturnType >(name, arr_size)
        {
            VDPStructConstantReturnArrayByPointerGet< StructType, ReturnType, ArrayType, Member > callback;
            VDPPODArrayOutputNode< ReturnType >::SetValue(callback(call_ptr));
        };

        virtual ~VDPStructMemberPODArrayOutputNode(){;};

    protected:

};



}//end of vdp namespace

#endif /* VDPStructMemberPODArrayOutputNode_H__ */
