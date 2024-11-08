// typedef struct sky_coord
//     {
//     short       ra_hrs;                 /* Self-explanatory */
//     short       ra_mins;
//     float       ra_secs;
//     short       dec_degs;
//     short       dec_mins;
//     float       dec_secs;
//     } sky_coord_struct;

#ifndef SkyCoordStructWrapper_HH__
#define SkyCoordStructWrapper_HH__

#include "VDPStructuredASCIIHeaders.hh"

extern "C"
{
#include "mk4_typedefs.h"
}



/*
*File: SkyCoordStructWrapper.hh
*Class: SkyCoordStructWrapper
*Author: J. Barrett
*Email: barrettj@mit.edu
*Date: Mon Jun 12 12:38:14 EDT 2017
*Description:
*/

namespace vdp
{

class SkyCoordStructWrapper: public VDPInputOutputObject
{
    public:

        SkyCoordStructWrapper(sky_coord_struct aSkyCoord):VDPInputOutputObject()
        {
            fSkyCoord = aSkyCoord;
        };

        SkyCoordStructWrapper():VDPInputOutputObject(){};

        virtual ~SkyCoordStructWrapper(){};

        SkyCoordStructWrapper(const SkyCoordStructWrapper& copyObject):
        VDPInputOutputObject()
        {
            fSkyCoord = copyObject.fSkyCoord;
        };

        SkyCoordStructWrapper& operator=(const SkyCoordStructWrapper& rhs)
        {
            if(&rhs != this)
            {
                fSkyCoord = rhs.fSkyCoord;
            }
            return *this;
        };

        virtual const char* GetName() const {return "sky_coord_struct";};

        void DefineOutputNode(VDPOutputNode* node) const;
        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "sky_coord_struct"; };

    public:

        sky_coord_struct fSkyCoord;

};

DefineVDPClassName( SkyCoordStructWrapper )

}

#endif /* end of include guard: SkyCoordStructWrapper */
