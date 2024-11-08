#ifndef VDPTestD_HH__
#define VDPTestD_HH__


#include "VDPTestB.hh"

#include "VDPStructuredASCIIHeaders.hh"

namespace vdp{


/**
*
*@file VDPTestD.hh
*@class VDPTestD
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Wed Jan 16 12:42:15 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPTestD: public VDPTestB
{
    public:
        VDPTestD(){;}
        virtual ~VDPTestD(){;}

        VDPTestD(const VDPTestD& copyObject):
        VDPTestB(copyObject)
        {
            fD = copyObject.fD;
        }

        VDPTestD& operator=(const VDPTestD& rhs)
        {
            if(&rhs != this)
            {
                fX = rhs.fX;
                fY = rhs.fY;
                fArr[0] = rhs.fArr[0];
                fArr[1] = rhs.fArr[1];
                fArr[2] = rhs.fArr[2];
                fD = rhs.fD;
            }
            return *this;
        }

        virtual void DefineOutputNode(VDPOutputNode* node) const;

        virtual void DefineInputNode(VDPInputNode* node);

        void SetD(const double& d){fD = d;};
        double GetD() const {return fD;};

        virtual const char* ClassName() const { return "VDPTestD"; };

    protected:

        double fD;

};


DefineVDPClassName( VDPTestD )

}



#endif /* VDPTestD_H__ */
