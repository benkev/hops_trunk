#ifndef VDPTestA_HH__
#define VDPTestA_HH__

#include <vector>
#include <utility>
#include <iostream>

#include "VDPTestB.hh"

#include "VDPStructuredASCIIHeaders.hh"


namespace vdp{

/**
*
*@file VDPTestA.hh
*@class VDPTestA
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Tue Dec 18 09:57:48 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPTestA: public VDPOutputObject, public VDPInputObject
{
    public:

        VDPTestA():VDPOutputObject()
        {

        };


        VDPTestA(const VDPTestA& copyObject):
        VDPOutputObject(),
        VDPInputObject(),
        fB(copyObject.fB)
        {
            fData = copyObject.fData;
            fBVec = copyObject.fBVec;
        }

        virtual ~VDPTestA()
        {

        };

        virtual const char* GetName() const;

        void AddData(double data);
        void ClearData();

//        void GetData(std::vector<double>* data) const ;
        const std::vector<double>* GetData() const;
        void SetData(const std::vector<double>* data);

        const VDPTestB* GetB() const;
        void SetB(const VDPTestB& b);

        void ClearBVector()
        {
            fBVec.clear(); //yeah this is a memory leak, but i am too lazy to fix this for a VDPTest example
        }

        void AddBVector(std::vector< VDPTestB* >* vec)
        {
            fBVec.push_back(*vec);
        }


        VDPTestA& operator=(const VDPTestA& rhs)
        {
            if(&rhs != this)
            {
                fData = rhs.fData;
                fB = rhs.fB;
                fBVec = rhs.fBVec;
            }
            return *this;
        }

        virtual void DefineOutputNode(VDPOutputNode* node) const;

        virtual void DefineInputNode(VDPInputNode* node);


        virtual const char* ClassName() const { return "VDPTestA"; };

    protected:

        VDPTestB fB;
        std::vector<double> fData;
        std::vector< std::vector< VDPTestB* > > fBVec;

};

DefineVDPClassName( VDPTestA )


}


#endif /* VDPTestA_H__ */
