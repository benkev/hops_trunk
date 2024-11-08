#include "ChanStructWrapper.hh"

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

void
ChanStructWrapper::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {

        //     char                chan_name[32];          /* External channel name */
        //     char                polarization;           /* R or L */
        //     double              sky_frequency;          /* Hz */
        //     char                net_sideband;           /* U or L */
        //     double              bandwidth;              /* Hz */
        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, char, char[32], &chan_struct::chan_name >( std::string("chan_name"), 32, &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, char, &chan_struct::polarization >( std::string("polarization"), &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, double, &chan_struct::sky_frequency >( std::string("sky_frequency"), &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, char, &chan_struct::net_sideband >( std::string("net_sideband"), &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, double, &chan_struct::bandwidth >( std::string("bandwidth"), &(this->fChan)) );

        //not sure if the linkword elements are needed
        //skipping these elements for now

//     char                band_id[32];            /* Linkword (internal use) */
//     char                chan_id[32];            /* Linkword (internal use) */
//     char                bbc_id[32];             /* Linkword (internal use) */
//     char                pcal_id[32];            /* Linkword (internal use) */
//     char                if_id[32];              /* Linkword (internal use) */

        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, char, char[32], &chan_struct::band_id >( std::string("band_id"), 32, &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, char, char[32], &chan_struct::chan_id >( std::string("chan_id"), 32, &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, char, char[32], &chan_struct::bbc_id >( std::string("bbc_id"), 32, &(this->fChan)) );

        //DISABLED FOR NOW!
        //node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, char, char[32], &chan_struct::pcal_id >( std::string("pcal_id"), 32, &(this->fChan)) );

        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, char, char[32], &chan_struct::if_id >( std::string("if_id"), 32, &(this->fChan)) );


        //     short               bbc_no;                 /* Physical BBC# */
        //     char                if_name[8];             /* Physical IF name */
        //     double              if_total_lo;            /* Hz */
        //     char                if_sideband;            /* U or L */
        //     float               pcal_spacing;           /* Hz */
        //     float               pcal_base_freq;         /* Hz */
        //     short               pcal_detect[16];        /* Integer tone #s */

        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, short, &chan_struct::bbc_no >( std::string("bbc_no"), &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, char, char[8], &chan_struct::if_name >( std::string("if_name"), 8, &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, double, &chan_struct::if_total_lo >( std::string("if_total_lo"), &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, char, &chan_struct::if_sideband >( std::string("if_sideband"), &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, float, &chan_struct::pcal_spacing >( std::string("pcal_spacing"), &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, float, &chan_struct::pcal_base_freq >( std::string("pcal_base_freq"), &(this->fChan)) );

        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, short, short[16], &chan_struct::pcal_detect >( std::string("pcal_detect"), 16, &(this->fChan)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("pcal_detect") ) );
        #endif
        #endif

        //     short               sign_tracks[4];         /* Track #s */
        //     short               sign_headstack;         /* 1-4 */
        //     short               mag_tracks[4];          /* Track #s */
        //     short               mag_headstack;          /* 1-4 */

        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, short, short[4], &chan_struct::sign_tracks >( std::string("sign_tracks"), 4, &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, short, &chan_struct::sign_headstack >( std::string("sign_headstack"), &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< chan_struct, short, short[4], &chan_struct::mag_tracks >( std::string("mag_tracks"), 4, &(this->fChan)) );
        node->AddChild(new VDPStructMemberPODOutputNode< chan_struct, short, &chan_struct::mag_headstack >( std::string("mag_headstack"), &(this->fChan)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("sign_tracks") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("sign_headstack") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("mag_tracks") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("mag_headstack") ) );
        #endif
        #endif

    }
}

void
ChanStructWrapper::DefineInputNode(VDPInputNode* node)
{
    //THIS DOES NOT YET WORK
    if(node != NULL)
    {

    }
}

}
