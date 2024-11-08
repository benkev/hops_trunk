#ifndef VDPAssociatedPointerPODArrayInputNode_HH__
#define VDPAssociatedPointerPODArrayInputNode_HH__

#include "VDPPODInputNode.hh"
#include "VDPCallbackTypes.hh"

#define AddVDPInputForArray(class,var,type,size) \
  node->AddChild(new VDPAssociatedPointerPODArrayInputNode<class, type, &class::Set ## var ## Array>( std::string(#var), size, this) )

namespace vdp{


/**
*
*@file VDPAssociatedPointerPODArrayInputNode.hh
*@class VDPAssociatedPointerPODArrayInputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Jan  3 22:10:43 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/



template< typename CallType, typename SetType, void (CallType::*memberFunction)(const SetType*) >
class VDPAssociatedPointerPODArrayInputNode: public VDPPODInputNode< std::vector<SetType> >
{
    public:

        VDPAssociatedPointerPODArrayInputNode(std::string name, unsigned int arr_size, CallType* call_ptr):VDPPODInputNode< std::vector< SetType > >(name),fArraySize(arr_size)
        {
            fCallPtr = call_ptr;
        };

        virtual ~VDPAssociatedPointerPODArrayInputNode(){;};

        void FinalizeObject()
        {
            fArray = new SetType[fArraySize];
            for(unsigned int i=0; i<fArraySize; i++)
            {
                fArray[i] = (this->fValue)[i];
            }

            fCallback(fCallPtr, fArray );

            delete[] fArray;
        }

    protected:

        CallType* fCallPtr;
        unsigned int fArraySize;
        SetType* fArray;
        VDPPassByConstantPointerSet< CallType, SetType, memberFunction > fCallback;

};


}

#endif /* VDPAssociatedPointerPODArrayInputNode_H__ */
