#include "SourceStructWrapper.hh"
#include "SkyCoordStructWrapper.hh"
#include "DateStructWrapper.hh"

#include <iostream>

// struct source_struct
//     {
//     char                source_name[32];
//     char                iau_name[32];
//     short               source_type;            /* Defines above */
//     short               calibrator;             /* True or false */
//     struct sky_coord    position;               /* Standard Mk4 struct */
//     struct date         position_epoch;         /* Standard Mk4 struct */
//     short               position_ref_frame;     /* Defines above */
//     float               ra_rate;                /* Radians/sec */
//     float               dec_rate;               /* Radians/sec */
//     };
// 

namespace vdp{



void
SourceStructWrapper::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {
        node->AddChild(new VDPStructMemberPODArrayOutputNode< source_struct, char, char[32], &source_struct::source_name >( std::string("source_name"), 32, &(this->fSourceStruct) ) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< source_struct, char, char[32], &source_struct::iau_name >( std::string("iau_name"), 32, &(this->fSourceStruct) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< source_struct, short, &source_struct::source_type >( std::string("source_type"),  &(this->fSourceStruct) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< source_struct, short, &source_struct::calibrator >( std::string("calibrator"),  &(this->fSourceStruct) ) );
        node->AddChild(new VDPObjectOutputNode< SkyCoordStructWrapper >( std::string("position"), new SkyCoordStructWrapper(this->fSourceStruct.position) ) );
        node->AddChild(new VDPObjectOutputNode< DateStructWrapper >( std::string("position_epoch"), new DateStructWrapper(this->fSourceStruct.position_epoch) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< source_struct, short, &source_struct::position_ref_frame >( std::string("position_ref_frame"),  &(this->fSourceStruct) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< source_struct, float, &source_struct::ra_rate >( std::string("ra_rate"),  &(this->fSourceStruct) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< source_struct, float, &source_struct::dec_rate >( std::string("dec_rate"),  &(this->fSourceStruct) ) );
    }
}

void
SourceStructWrapper::DefineInputNode(VDPInputNode* node)
{
    //THIS DOES NOT YET WORK
    if(node != NULL)
    {

    }
}



}
