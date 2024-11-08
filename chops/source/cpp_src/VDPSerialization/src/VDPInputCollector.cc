#include "VDPInputCollector.hh"
#include <iostream>

namespace vdp{

VDPInputCollector::VDPInputCollector()
{
    fReader = NULL;
}

VDPInputCollector::~VDPInputCollector()
{

}

void
VDPInputCollector::ForwardInput(VDPInputNode* root)
{

    fNodeStack = std::stack< VDPInputNode* >();
    fNodeStack.push(root);

    bool file_status = false;
    if(fReader != NULL){file_status = true;};


    int count = 0;
    while(fNodeStack.size() != 0 && file_status)
    {

        //std::cout<<"calling the reader at line count: "<<count<<std::endl;

        file_status = fReader->GetLine(fLine);


        fNodeStack.top()->AddLine(fLine);

        fTempNode = NULL;
        //now figure out whether we needed to decend the tree, stay, or ascend
        fStatus = fNodeStack.top()->GetNextNode(fTempNode);

        if(fStatus == VDPNODE_MOVE_DOWNWARD && fTempNode != NULL)
        {
            //std::cout<<"moving downard to node: "<<fTempNode->GetName()<<std::endl;
            fNodeStack.push( fTempNode ); //descend to child
        }
        else if (fStatus == VDPNODE_MOVE_UPWARD)
        {
            //std::cout<<"moving upward to node: "<<fNodeStack.top()->GetName()<<std::endl;
            fNodeStack.pop();
        }
        else if (fStatus == VDPNODE_STAY)
        {
            //do nothing, stay on same node for another line
        }
        else
        {
            //std::cout<<"Error!"<<std::endl;
            break;
            //break, error
        }

        //std::cout<<"line = "<<fLine<<std::endl;

        count++;

    };

}

}//end of namespace
