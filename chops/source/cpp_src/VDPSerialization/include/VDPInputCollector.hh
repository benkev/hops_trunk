#ifndef VDPInputCollector_HH__
#define VDPInputCollector_HH__


#include "VDPFileReader.hh"
#include "VDPInputNode.hh"
#include <stack>

namespace vdp{


/**
*
*@file VDPInputCollector.hh
*@class VDPInputCollector
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Fri Jan  4 13:55:45 EST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPInputCollector
{
    public:
        VDPInputCollector();
        virtual ~VDPInputCollector();

        void SetFileReader(VDPFileReader* reader){fReader = reader;};

        void ForwardInput(VDPInputNode* root);

    protected:

        std::stack< VDPInputNode* > fNodeStack;
        VDPInputNode* fTempNode;
        int fStatus;
        std::string fLine;
        VDPFileReader* fReader;

};


}


#endif /* VDPInputCollector_H__ */
