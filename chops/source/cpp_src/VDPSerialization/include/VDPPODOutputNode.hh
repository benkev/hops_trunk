#ifndef VDPPODOutputNode_HH__
#define VDPPODOutputNode_HH__

#include "VDPPODConverter.hh"
#include "VDPOutputNode.hh"

namespace vdp{


/**
*
*@file VDPPODOutputNode.hh
*@class VDPPODOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Fri Dec 28 23:28:49 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


//should work as long as U is a pod type of a std::vector<> of a pod type
template < typename U >
class VDPPODOutputNode: public VDPOutputNode
{
    public:


        VDPPODOutputNode(std::string name):VDPOutputNode(name)
        {
            fConverter = new VDPPODConverter< U >();
            fStringValue = "INVALID";
        };

        virtual ~VDPPODOutputNode()
        {
            delete fConverter;
        }

        virtual bool TagsAreSuppressed(){return false;};

        virtual bool IsComposite(){return false;};

        void SetValue(const U& val)
        {
            fConverter->ConvertParameterToString(fStringValue, val);
        }

        void SetValue(const U* val)
        {
            fConverter->ConvertParameterToString(fStringValue, val);
        }

    protected:

        virtual std::string GetSingleLine(){return fStringValue;};

        std::string fStringValue;
        VDPPODConverter< U >* fConverter;

    private:

        //cannot instantiate without providing a name
        VDPPODOutputNode(){;};

};


}//end of vdp namespace


#endif /* VDPPODOutputNode_H__ */
