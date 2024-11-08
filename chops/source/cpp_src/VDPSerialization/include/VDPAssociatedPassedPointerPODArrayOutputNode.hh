#ifndef VDPAssociatedPassedPointerPODArrayOutputNode_HH__
#define VDPAssociatedPassedPointerPODArrayOutputNode_HH__


#include "VDPCallbackTypes.hh"
#include "VDPPODArrayOutputNode.hh"

#define AddVDPOutputForArray(class,var,type,size) \
  node->AddChild(new VDPAssociatedPassedPointerPODArrayOutputNode<class, type, &class::Get ## var ## Array>( std::string(#var), size, this) )


namespace vdp{


/**
*
*@file VDPAssociatedPassedPointerPODArrayOutputNode.hh
*@class VDPAssociatedPassedPointerPODArrayOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 29 21:12:33 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


template<typename CallType, typename ReturnType, void (CallType::*memberFunction)(ReturnType* ) const >
class VDPAssociatedPassedPointerPODArrayOutputNode: public VDPPODArrayOutputNode< ReturnType >
{
    public:

        VDPAssociatedPassedPointerPODArrayOutputNode(std::string name, unsigned int arr_size, const CallType* call_ptr):VDPPODArrayOutputNode< ReturnType >(name, arr_size)
        {
            VDPConstantReturnByPassedPointerGet< CallType, ReturnType, memberFunction > callback;
            ReturnType* val = new ReturnType[this->fArraySize];
            callback(call_ptr, val);
            VDPPODArrayOutputNode< ReturnType >::SetValue(val);
            delete[] val;
        }

        virtual ~VDPAssociatedPassedPointerPODArrayOutputNode(){;};

    protected:

};



}//end of vdp namespace

#endif /* VDPAssociatedPassedPointerPODArrayOutputNode_H__ */
