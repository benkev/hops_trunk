#ifndef VDPStructMemberPODInputNode_HH__
#define VDPStructMemberPODInputNode_HH__

#include "VDPPODInputNode.hh"
#include "VDPCallbackTypes.hh"

namespace vdp{


/**
*
*@file VDPStructMemberPODInputNode.hh
*@class VDPStructMemberPODInputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Jan  3 22:10:43 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/


template <typename StructType, typename SetType, SetType StructType::*Member >
class VDPStructMemberPODInputNode: public VDPPODInputNode<SetType>
{
    public:

        VDPStructMemberPODInputNode(std::string name, StructType* call_ptr):VDPPODInputNode< SetType >(name)
        {
            fCallPtr = call_ptr;
        };

        virtual ~VDPStructMemberPODInputNode(){;};

        void FinalizeObject()
        {
            fCallback(fCallPtr, this->fValue);
        }

    protected:

        StructType* fCallPtr;
        VDPStructMemberSet< StructType, SetType, Member > fCallback;

};


}

#endif /* VDPStructMemberPODInputNode_H__ */
