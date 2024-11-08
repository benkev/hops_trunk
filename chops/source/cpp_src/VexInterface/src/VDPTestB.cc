#include "VDPTestB.hh"


namespace vdp{


const char*
VDPTestB::GetName() const {return "VDPTestB";}

double VDPTestB::GetX() const {return fX;}
void VDPTestB::SetX(const double& x){fX = x;}

double VDPTestB::GetY() const {return fY;}
void VDPTestB::SetY(const double& y){fY = y;}


void VDPTestB::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {
        node->AddChild(new VDPAssociatedValuePODOutputNode< VDPTestB, double, &VDPTestB::GetX >( std::string("X"), this) );
        node->AddChild(new VDPAssociatedValuePODOutputNode< VDPTestB, double, &VDPTestB::GetY >( std::string("Y"), this) );
        node->AddChild(new VDPAssociatedPassedPointerPODArrayOutputNode<VDPTestB, double, &VDPTestB::GetArray>( std::string("Arr"), 3, this) );
    }
}


void VDPTestB::DefineInputNode(VDPInputNode* node)
{
    if(node != NULL)
    {
        node->AddChild(new VDPAssociatedReferencePODInputNode< VDPTestB, double, &VDPTestB::SetX >( std::string("X"), this) );
        node->AddChild(new VDPAssociatedReferencePODInputNode< VDPTestB, double, &VDPTestB::SetY >( std::string("Y"), this) );
        node->AddChild(new VDPAssociatedPointerPODArrayInputNode<VDPTestB, double, &VDPTestB::SetArray>( std::string("Arr"), 3, this) );
    }
}


}
