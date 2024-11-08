#ifndef VDPAssociatedPassedPointerPODOutputNode_HH__
#define VDPAssociatedPassedPointerPODOutputNode_HH__


#include "VDPCallbackTypes.hh"
#include "VDPPODOutputNode.hh"

namespace vdp{


/**
*
*@file VDPAssociatedPassedPointerPODOutputNode.hh
*@class VDPAssociatedPassedPointerPODOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 21:12:33 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


template<typename CallType, typename ReturnType, void (CallType::*memberFunction)(ReturnType* ) const >
class VDPAssociatedPassedPointerPODOutputNode: public VDPPODOutputNode< ReturnType >
{
    public:

        VDPAssociatedPassedPointerPODOutputNode(std::string name, const CallType* call_ptr):VDPPODOutputNode< ReturnType >( name )
        {
            VDPConstantReturnByPassedPointerGet< CallType, ReturnType, memberFunction > callback;
            ReturnType val;
            callback(call_ptr, &val);
            VDPPODOutputNode< ReturnType >::SetValue(&val);
        }

        virtual ~VDPAssociatedPassedPointerPODOutputNode(){;};

    protected:

};



}//end of vdp namespace

#endif /* VDPAssociatedPassedPointerPODOutputNode_H__ */
