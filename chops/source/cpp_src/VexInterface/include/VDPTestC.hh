#ifndef VDPTestC_HH__
#define VDPTestC_HH__

#include <vector>
#include <utility>
#include <iostream>

#include "VDPTestA.hh"
#include "VDPStructuredASCIIHeaders.hh"

namespace vdp{

/**
*
*@file VDPTestC.hh
*@class VDPTestC
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Tue Dec 18 09:57:48 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPTestC: public VDPTestA
{
    public:

        VDPTestC():VDPTestA()
        {

        };


        VDPTestC(const VDPTestC& copyObject):
        VDPTestA(copyObject)
        {
            fCData = copyObject.fCData;
        }

        virtual ~VDPTestC()
        {

        };

        virtual const char* GetName() const;

        VDPTestC& operator=(const VDPTestC& rhs)
        {
            if(&rhs != this)
            {
                VDPTestA::operator = (rhs);
                fCData = rhs.fCData;
            }
            return *this;
        }

        double GetCData() const;
        void SetCData(const double& x);

        void DefineOutputNode(VDPOutputNode* node) const;

        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "VDPTestC"; };

    protected:


        double fCData;

};

DefineVDPClassName( VDPTestC )


}


#endif /* VDPTestC_H__ */
