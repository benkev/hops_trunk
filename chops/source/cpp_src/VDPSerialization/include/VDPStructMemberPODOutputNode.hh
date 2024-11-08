#ifndef VDPStructMemberPODOutputNode_HH__
#define VDPStructMemberPODOutputNode_HH__

#include "VDPCallbackTypes.hh"
#include "VDPPODOutputNode.hh"

namespace vdp{


/**
*
*@file VDPStructMemberPODOutputNode.hh
*@class VDPStructMemberPODOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 21:12:33 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/



template <typename StructType, typename ReturnType, ReturnType StructType::*Member >
class VDPStructMemberPODOutputNode: public VDPPODOutputNode< ReturnType >
{
    public:
        VDPStructMemberPODOutputNode(std::string name, const StructType* call_ptr ):VDPPODOutputNode< ReturnType >(name)
        {
            VDPStructMemberGet< StructType, ReturnType, Member > callback;
            VDPPODOutputNode< ReturnType >::SetValue(callback(call_ptr));
        };

        virtual ~VDPStructMemberPODOutputNode(){;};

    protected:

};



}//end of vdp namespace

#endif /* VDPStructMemberPODOutputNode_H__ */
