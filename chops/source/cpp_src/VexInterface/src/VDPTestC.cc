#include "VDPTestC.hh"

namespace vdp{

const char* VDPTestC::GetName() const {return "VDPTestC";}

double VDPTestC::GetCData() const {return fCData;}

void VDPTestC::SetCData(const double& x){fCData = x;}

void VDPTestC::DefineOutputNode(VDPOutputNode* node) const
{
    VDPTestA::DefineOutputNode(node);
    node->AddChild(new VDPAssociatedValuePODOutputNode< VDPTestC, double, &VDPTestC::GetCData >( std::string("CData"), this) );

}

void VDPTestC::DefineInputNode(VDPInputNode* node)
{
    if(node != NULL)
    {
        VDPTestA::DefineInputNode(node);
        node->AddChild(new VDPAssociatedReferencePODInputNode< VDPTestC, double, &VDPTestC::SetCData >(std::string("CData"), this) );
    }
}



}
