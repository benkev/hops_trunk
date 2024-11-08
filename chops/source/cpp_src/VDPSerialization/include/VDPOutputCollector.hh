#ifndef VDPOutputCollector_H__
#define VDPOutputCollector_H__

#include <stack>
#include <string>
#include <sstream>

#include "VDPOutputNode.hh"
#include "VDPFileWriter.hh"

namespace vdp{

/**
*
*@file VDPOutputCollector.hh
*@class VDPOutputCollector
*@brief visits each node of an output tree below the
* given node in a depth first manner and pipes the output to the file writer
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Tue Aug 28 15:47:04 EDT 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPOutputCollector
{
    public:
        VDPOutputCollector(){fUseTabbing = false;};
        ~VDPOutputCollector(){;};

        void SetFileWriter(VDPFileWriter* writer);

        //recursively ask GetLine() from every node
        //below this one, this should only be called on the root node
        void CollectOutput(VDPOutputNode* node);

        void SetUseTabbingTrue(){fUseTabbing = true;}
        void SetUseTabbingFalse(){fUseTabbing = false;}


    protected:

        void CollectNodeOutput(VDPOutputNode* node);
        void ForwardNodeOutput();

        std::stack< VDPOutputNode* > fNodeStack;
        VDPOutputNode* fTempNode;
        bool fUseTabbing;
        int fStatus;
        std::string fLine;
        VDPFileWriter* fWriter;
        std::stringstream fStream;


};


}//end of vdp namespace

#endif /* __VDPOutputCollector_H__ */
