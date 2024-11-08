#include "VDPTestE.hh"

namespace vdp
{

void
VDPTestE::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {
        node->AddChild(new VDPStructMemberPODOutputNode< TestEStruct, int, &TestEStruct::ivalue >( std::string("ivalue"), &(this->fE) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< TestEStruct, double, &TestEStruct::dvalue >( std::string("dvalue"), &(this->fE)) );
        node->AddChild(new VDPStructMemberPODOutputNode< TestEStruct, float, &TestEStruct::fvalue >( std::string("fvalue"),  &(this->fE) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< TestEStruct, std::string, &TestEStruct::svalue >( std::string("svalue"),  &(this->fE) ) );
    }
}

void
VDPTestE::DefineInputNode(VDPInputNode* node)
{
    if(node != NULL)
    {
        node->AddChild(new VDPStructMemberPODInputNode< TestEStruct, int, &TestEStruct::ivalue>( std::string("ivalue"), &(this->fE) ) );
        node->AddChild(new VDPStructMemberPODInputNode< TestEStruct, double, &TestEStruct::dvalue >( std::string("dvalue"), &(this->fE) ) );
        node->AddChild(new VDPStructMemberPODInputNode< TestEStruct, float, &TestEStruct::fvalue >( std::string("fvalue"), &(this->fE) ) );
        node->AddChild(new VDPStructMemberPODInputNode< TestEStruct, std::string, &TestEStruct::svalue >( std::string("svalue"), &(this->fE) ) );
    }
}

} //end of vdp namespace
