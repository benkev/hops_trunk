#ifndef ChanStructWrapper_HH__
#define ChanStructWrapper_HH__

#include "VDPStructuredASCIIHeaders.hh"

extern "C"
{
#include "ovex.h"
}


/*
*File: ChanStructWrapper.hh
*Class: ChanStructWrapper
*Author: J. Barrett
*Email: barrettj@mit.edu
*Date:
*Description:
*/

// struct chan_struct
//     {
//     char                chan_name[32];          /* External channel name */
//     char                polarization;           /* R or L */
//     double              sky_frequency;          /* Hz */
//     char                net_sideband;           /* U or L */
//     double              bandwidth;              /* Hz */
//     char                band_id[32];            /* Linkword (internal use) */
//     char                chan_id[32];            /* Linkword (internal use) */
//     char                bbc_id[32];             /* Linkword (internal use) */
//     char                pcal_id[32];            /* Linkword (internal use) */
//     char                if_id[32];              /* Linkword (internal use) */
//     short               bbc_no;                 /* Physical BBC# */
//     char                if_name[8];             /* Physical IF name */
//     double              if_total_lo;            /* Hz */
//     char                if_sideband;            /* U or L */
//     float               pcal_spacing;           /* Hz */
//     float               pcal_base_freq;         /* Hz */
//     short               pcal_detect[16];        /* Integer tone #s */
//     short               sign_tracks[4];         /* Track #s */
//     short               sign_headstack;         /* 1-4 */
//     short               mag_tracks[4];          /* Track #s */
//     short               mag_headstack;          /* 1-4 */
//     };




namespace vdp
{

class ChanStructWrapper: public VDPInputOutputObject
{
    public:

        ChanStructWrapper(chan_struct aChan):VDPInputOutputObject()
        {
            fChan = aChan;
        };

        ChanStructWrapper():VDPInputOutputObject(){};

        virtual ~ChanStructWrapper(){};

        ChanStructWrapper(const ChanStructWrapper& copyObject):
        VDPInputOutputObject()
        {
            fChan = copyObject.fChan;
        };

        ChanStructWrapper& operator=(const ChanStructWrapper& rhs)
        {
            if(&rhs != this)
            {
                fChan = rhs.fChan;
            }
            return *this;
        };

        virtual const char* GetName() const {return "chan_struct";};

        void DefineOutputNode(VDPOutputNode* node) const;
        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "chan_struct"; };

    public:

        chan_struct fChan;

};

DefineVDPClassName( ChanStructWrapper )

}

#endif /* end of include guard: ChanStructWrapper */
