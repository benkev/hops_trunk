#include "VDPTestD.hh"


namespace vdp{



void
VDPTestD::DefineOutputNode(VDPOutputNode* node) const
{
    VDPTestB::DefineOutputNode(node);
    if(node != NULL)
    {
         node->AddChild(new VDPAssociatedValuePODOutputNode< VDPTestD, double, &VDPTestD::GetD >( std::string("D"), this) );
    }

}

void
VDPTestD::DefineInputNode(VDPInputNode* node)
{
    VDPTestB::DefineInputNode(node);
    if(node != NULL)
    {
        node->AddChild(new VDPAssociatedReferencePODInputNode< VDPTestD, double, &VDPTestD::SetD >( std::string("D"), this) );
    }
}


}
