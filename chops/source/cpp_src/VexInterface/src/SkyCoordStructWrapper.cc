#include "SkyCoordStructWrapper.hh"

// typedef struct sky_coord
//     {
//     short       ra_hrs;                 /* Self-explanatory */
//     short       ra_mins;
//     float       ra_secs;
//     short       dec_degs;
//     short       dec_mins;
//     float       dec_secs;
//     } sky_coord_struct;

namespace vdp
{

void
SkyCoordStructWrapper::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {
        node->AddChild(new VDPStructMemberPODOutputNode< sky_coord_struct, short, &sky_coord_struct::ra_hrs >( std::string("ra_hrs"), &(this->fSkyCoord)) );
        node->AddChild(new VDPStructMemberPODOutputNode< sky_coord_struct, short, &sky_coord_struct::ra_mins >( std::string("ra_mins"),  &(this->fSkyCoord) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< sky_coord_struct, float, &sky_coord_struct::ra_secs >( std::string("ra_secs"),  &(this->fSkyCoord) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< sky_coord_struct, short, &sky_coord_struct::dec_degs >( std::string("dec_degs"),  &(this->fSkyCoord) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< sky_coord_struct, short, &sky_coord_struct::dec_mins >( std::string("dec_mins"),  &(this->fSkyCoord) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< sky_coord_struct, float, &sky_coord_struct::dec_secs >( std::string("dec_secs"),  &(this->fSkyCoord) ) );
    }
}

void
SkyCoordStructWrapper::DefineInputNode(VDPInputNode* node)
{
    //THIS DOES NOT YET WORK
    if(node != NULL)
    {

    }
}

}
