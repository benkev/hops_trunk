#ifndef VDPPODArrayOutputNode_HH__
#define VDPPODArrayOutputNode_HH__

#include "VDPCallbackTypes.hh"
#include "VDPPODArrayOutputNode.hh"

namespace vdp{

/**
*
*@file VDPPODArrayOutputNode.hh
*@class VDPPODArrayOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Mon Jan 14 09:45:58 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/


template < typename U >
class VDPPODArrayOutputNode: public VDPOutputNode
{
    public:
        VDPPODArrayOutputNode(std::string name, unsigned int arr_size):VDPOutputNode(name),fArraySize(arr_size)
        {
            fConverter = new VDPPODConverter< std::vector< U > >();
            fStringValue = "INVALID";
        };

        virtual ~VDPPODArrayOutputNode()
        {
            delete fConverter;
        }

        void SetValue(const U* val)
        {
            fVal.clear();
            fVal.reserve(fArraySize);
            for(unsigned int i=0; i<fArraySize; i++)
            {
                fVal.push_back(val[i]);
            }
            fConverter->ConvertParameterToString(fStringValue, &fVal);
        }

    protected:

        virtual std::string GetSingleLine(){return fStringValue;};

        unsigned int fArraySize;
        std::string fStringValue;
        std::vector< U > fVal;
        VDPPODConverter< std::vector< U > >* fConverter;

    private:

        //cannot instantiate without providing a name
        VDPPODArrayOutputNode(){;};

    protected:

};

//chars are special
template < >
class VDPPODArrayOutputNode<char>: public VDPOutputNode
{
    public:
        VDPPODArrayOutputNode(std::string name, unsigned int arr_size):VDPOutputNode(name),fArraySize(arr_size)
        {
            fStringValue = "INVALID";
        };

        virtual ~VDPPODArrayOutputNode(){}

        void SetValue(const char* val)
        {
            fStringValue = std::string(val);
        }

    protected:

        virtual std::string GetSingleLine(){return fStringValue;};

        unsigned int fArraySize;
        std::string fStringValue;

    private:

        //cannot instantiate without providing a name
        VDPPODArrayOutputNode(){;};

    protected:

};


}


#endif /* VDPPODArrayOutputNode_H__ */
