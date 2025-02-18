#include "VDPOutputCollector.hh"
#include <iostream>

namespace vdp{

void VDPOutputCollector::SetFileWriter(VDPFileWriter* writer)
{
    fWriter = writer;
}

//recursively apply the operational visitor to every node
//below this one
void VDPOutputCollector::CollectOutput(VDPOutputNode* node)
{
    fNodeStack = std::stack< VDPOutputNode* >();
    fNodeStack.push(node);

    //add the root's start tag
    fLine = node->GetStartTag() + std::string(LINE_DELIM);

    fWriter->AddToFile(fLine);

    do
    {
        CollectNodeOutput( fNodeStack.top() );
        fTempNode = NULL;
        //now figure out whether we needed to decend the tree, stay, or ascend
        fStatus = fNodeStack.top()->GetNextNode(fTempNode);

        ForwardNodeOutput();

        if(fStatus == VDPNODE_MOVE_DOWNWARD && fTempNode != NULL)
        {
            //std::cout<<"moving downard to node: "<<fTempNode->GetName()<<std::endl;
            fNodeStack.push( fTempNode ); //descend to child
        }
        else if (fStatus == VDPNODE_MOVE_UPWARD)
        {
            //std::cout<<"popping node"<<std::endl;
            fNodeStack.pop();
        }
        else if (fStatus == VDPNODE_STAY)
        {
            //std::cout<<"waiting for another line"<<std::endl;
            //do nothing, stay on same node for another line
        }
        else
        {
            //break, error
        }
    }
    while(fNodeStack.size() != 0 );
}

void VDPOutputCollector::CollectNodeOutput(VDPOutputNode* node)
{
    node->GetLine(fLine);
    //std::cout<<"got line: "<<fLine<<std::endl;
}


void VDPOutputCollector::ForwardNodeOutput()
{
    fStream.str("");
    fStream.clear();


    //prepend the line with the same number of tabs,
    //as the current depth of the output tree
    //this has no use other than to make the output look pretty

    if(fUseTabbing)
    {
        int depth = fNodeStack.size();
        if(fStatus == VDPNODE_MOVE_UPWARD)
        {
            if(depth > 0)
            {
                depth -= 1;
            }
        }
        for(int i=0; i<depth; i++)
        {
            fStream << "\t";
        }
    }

    fStream << fLine;
    fWriter->AddToFile(fStream.str());

}


}//end of vdp
