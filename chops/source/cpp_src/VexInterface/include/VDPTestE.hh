#ifndef VDPTestE_HH__
#define VDPTestE_HH__

#include "VDPStructuredASCIIHeaders.hh"

namespace vdp
{

struct TestEStruct
{
    int ivalue;
    double dvalue;
    float fvalue;
    std::string svalue;
};


class VDPTestE: public VDPInputOutputObject
{
    public:

        VDPTestE():VDPInputOutputObject()
        {
            fE.ivalue = 0;
            fE.dvalue = 0;
            fE.fvalue = 0;
            fE.svalue = "";
        };

        virtual ~VDPTestE(){};

        VDPTestE(const VDPTestE& copyObject):
        VDPInputOutputObject()
        {
            fE = copyObject.fE;
        };

        VDPTestE& operator=(const VDPTestE& rhs)
        {
            if(&rhs != this)
            {
                fE = rhs.fE;
            }
            return *this;
        };

        virtual const char* GetName() const {return "VDPTestE";};

        void DefineOutputNode(VDPOutputNode* node) const;

        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "VDPTestE"; };

    public:

        struct TestEStruct fE;

};

DefineVDPClassName( VDPTestE )


}

#endif /* end of include guard: VDPTestE_HH__ */
