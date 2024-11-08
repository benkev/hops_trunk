#include "VDPTestA.hh"
#include "VDPTestD.hh"
#include <iostream>

namespace vdp{

const char* VDPTestA::GetName() const {return "VDPTestA";}

void VDPTestA::AddData(double data){fData.push_back(data);}
void VDPTestA::ClearData(){fData.clear(); }

//void VDPTestA::GetData(std::vector<double>* data) const {*data = fData; };

const std::vector<double>* VDPTestA::GetData() const
{
    return &fData;
}

void VDPTestA::SetData(const std::vector<double>* data){fData = *data;}

const VDPTestB* VDPTestA::GetB() const {return &fB;}

void VDPTestA::SetB(const VDPTestB& b)
{
    fB = b;
}


void VDPTestA::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {
        node->AddChild(new VDPAssociatedPointerPODOutputNode< VDPTestA, std::vector< double >, &VDPTestA::GetData >( std::string("data"), this) );
        node->AddChild(new VDPAssociatedPointerObjectOutputNode< VDPTestA, VDPTestB, &VDPTestA::GetB >( std::string("VDPTestB"), this) );
        typedef std::vector< std::vector< VDPTestB* > > Bvecvec;
        node->AddChild(new VDPObjectOutputNode< Bvecvec >( VDPClassName<Bvecvec>::name() , &fBVec) );
    }
}


void VDPTestA::DefineInputNode(VDPInputNode* node)
{
    if(node != NULL)
    {
        node->AddChild(new VDPAssociatedReferenceObjectInputNode< VDPTestA, VDPTestB, &VDPTestA::SetB >(std::string("VDPTestB"), this) );
        node->AddChild(new VDPAssociatedPointerPODInputNode< VDPTestA, std::vector< double >, &VDPTestA::SetData >( std::string("data"), this) );

        typedef std::vector< std::vector< VDPTestB* > > Bvecvec;
        VDPObjectInputNode< Bvecvec >* complicated_node = new VDPObjectInputNode< Bvecvec >( VDPClassName<Bvecvec>::name() , &fBVec);
        complicated_node->AddChild<std::vector< VDPTestB* >, VDPTestD>( new VDPAssociatedAllocatedToVectorPointerObjectInputNode<  std::vector< VDPTestB* > , VDPTestD >( VDPClassName<VDPTestD>::name() ) );
        node->AddChild(complicated_node );

    }
}


}
