#ifndef VDPTestB_HH__
#define VDPTestB_HH__

#include <utility>
#include <vector>

#include "VDPStructuredASCIIHeaders.hh"

namespace vdp{


/**
*
*@file VDPTestB.hh
*@class VDPTestB
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Tue Dec 18 10:00:46 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

//class VDPTestB: public VDPFixedSizeInputOutputObject
class VDPTestB: public VDPInputOutputObject
{
    public:

        VDPTestB()
        {
            fX = 0;
            fY = 0;
        }

        VDPTestB(const VDPTestB& copyObject):
        VDPInputOutputObject()
        {
            fX = copyObject.fX;
            fY = copyObject.fY;
            fArr[0] = copyObject.fArr[0];
            fArr[1] = copyObject.fArr[1];
            fArr[2] = copyObject.fArr[2];
        }

        VDPTestB& operator=(const VDPTestB& rhs)
        {
            if(&rhs != this)
            {
                fX = rhs.fX;
                fY = rhs.fY;
                fArr[0] = rhs.fArr[0];
                fArr[1] = rhs.fArr[1];
                fArr[2] = rhs.fArr[2];
            }
            return *this;
        }

        virtual ~VDPTestB()
        {

        };

        virtual const char* GetName() const;


        double GetX() const;
        void SetX(const double& x);

        double GetY() const;
        void SetY(const double& y);

        void SetArray(const double* arr){fArr[0] = arr[0]; fArr[1] = arr[1]; fArr[2] = arr[2];};

        void GetArray(double* arr) const {arr[0] = fArr[0]; arr[1] = fArr[1]; arr[2] = fArr[2];};

        void DefineOutputNode(VDPOutputNode* node) const;

        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "VDPTestB"; };

    protected:

        double fX;
        double fY;
        double fArr[3];

};

DefineVDPClassName( VDPTestB )


}

#endif /* VDPTestB_H__ */
