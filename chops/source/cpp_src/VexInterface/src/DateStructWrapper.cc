#include "DateStructWrapper.hh"

// typedef struct date
//     {
//     short	year;
//     short	day;
//     short	hour;
//     short	minute;
//     float	second;
//     } date_struct;


namespace vdp
{

void
DateStructWrapper::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {
        node->AddChild(new VDPStructMemberPODOutputNode< date_struct, short, &date_struct::year >( std::string("year"), &(this->fDate)) );
        node->AddChild(new VDPStructMemberPODOutputNode< date_struct, short, &date_struct::day >( std::string("day"),  &(this->fDate) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< date_struct, short, &date_struct::hour >( std::string("hour"),  &(this->fDate) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< date_struct, short, &date_struct::minute >( std::string("minute"),  &(this->fDate) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< date_struct, float, &date_struct::second >( std::string("second"),  &(this->fDate) ) );
    }
}

void
DateStructWrapper::DefineInputNode(VDPInputNode* node)
{
    //THIS DOES NOT YET WORK
    if(node != NULL)
    {

    }
}

}
