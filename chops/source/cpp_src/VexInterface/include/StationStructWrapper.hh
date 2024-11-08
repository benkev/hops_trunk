#ifndef StationStructWrapper_HH__
#define StationStructWrapper_HH__

/*
*File: StationStructWrapper.hh
*Class: StationStructWrapper
*Author: J. Barrett
*Email: barrettj@mit.edu
*Date:
*Description:
*/

#include "VDPStructuredASCIIHeaders.hh"

extern "C"
{
#include "ovex.h"
}


namespace vdp
{

class StationStructWrapper: public VDPInputOutputObject
{
    public:

        StationStructWrapper(station_struct aStation):VDPInputOutputObject()
        {
            fStation = aStation;
        };

        StationStructWrapper():VDPInputOutputObject(){};

        virtual ~StationStructWrapper(){};

        StationStructWrapper(const StationStructWrapper& copyObject):
        VDPInputOutputObject()
        {
            fStation = copyObject.fStation;
        };

        StationStructWrapper& operator=(const StationStructWrapper& rhs)
        {
            if(&rhs != this)
            {
                fStation = rhs.fStation;
            }
            return *this;
        };

        virtual const char* GetName() const {return "station_struct";};

        void DefineOutputNode(VDPOutputNode* node) const;

        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "station_struct"; };

    public:

        station_struct fStation;

};

DefineVDPClassName( StationStructWrapper )


}


#endif /* end of include guard: StationStructWrapper */
