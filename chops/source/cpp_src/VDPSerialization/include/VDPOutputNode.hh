#ifndef VDPOutputNode_HH__
#define VDPOutputNode_HH__

#include <vector>

#include "VDPObject.hh"
#include <vector>

#define VDPNODE_MOVE_DOWNWARD -1 //indicates we need to decend to a child node
#define VDPNODE_MOVE_UPWARD 1 //indicates we need to ascend to parent
#define VDPNODE_STAY 0 //indicates we need to stay on the current node

namespace vdp{


/**
*
*@file VDPOutputNode.hh
*@class VDPOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Dec 27 23:05:43 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPOutputNode: public VDPObject
{
    public:

        VDPOutputNode(std::string name):
        VDPObject(name),
        fStatus(VDPNODE_STAY),
        fSingle(0),
        fIndex(0),
        fNextNode(NULL)
        {
            fChildren.clear();
        };

        VDPOutputNode():
        VDPObject(),
        fStatus(VDPNODE_STAY),
        fSingle(0),
        fIndex(0),
        fNextNode(NULL)
        {
            fChildren.clear();
        };

        virtual ~VDPOutputNode()
        {
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                delete fChildren[i];
		        fChildren[i] = NULL;
            }

            fChildren.clear();
        };

        virtual void Initialize(){;};

        virtual bool TagsAreSuppressed(){return false;};

        virtual bool IsComposite(){return false;};

        virtual void AddChild(VDPOutputNode* child){fChildren.push_back(child);};

        //next node will be set to NULL if the visitor traversing the tree
        //needs to move back to the parent, or stay on the current node
        virtual int GetNextNode(VDPOutputNode*& next_node)
        {
            next_node = fNextNode;
            return fStatus;
        }

        virtual void Reset()
        {
            fIndex = 0;
            fStatus = VDPNODE_STAY;
            fSingle = 0;
            fNextNode = NULL;
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                delete fChildren[i];
            }

            fChildren.clear();
        }

        virtual bool HasChildren() const
        {
            if( fChildren.size() == 0 ){return false;};
            return true;
        };

        virtual void GetLine(std::string& line)
        {
            if(fChildren.size() != 0)
            {
                //this iterates over all the children of a composite node
 	        if( fIndex >=0 && fIndex < ((int)fChildren.size()) )
                {
                    //open the next node
                    line = fChildren[fIndex]->GetStartTag() + std::string(LINE_DELIM);
                    fStatus = VDPNODE_MOVE_DOWNWARD;
                    fNextNode = fChildren[fIndex];
                    fIndex++;
                }
                else
                {
                    line = GetStopTag() + std::string(LINE_DELIM); //close out the current node
                    fStatus = VDPNODE_MOVE_UPWARD;
                    fNextNode = NULL; //next node is parent
                }
            }
            else
            {
                //this is here specifically for POD and non-composite types that
                //can be stringified into a single line without child nodes
                if(fSingle == 0)
                {
                    line = GetSingleLine() + std::string(LINE_DELIM);
                    fStatus = VDPNODE_STAY;
                    fNextNode = NULL;
                    fSingle = 1;
                }
                else
                {
                    line = GetStopTag() + std::string(LINE_DELIM);
                    fStatus = VDPNODE_MOVE_UPWARD;
                    fNextNode = NULL; //next node is parent
                }

            }
        }


    protected:

        //must defined in a POD node, in composite nodes it is never called
        virtual std::string GetSingleLine(){return std::string("INVALID");};

        int fStatus;
        int fSingle;
        int fIndex;
        VDPOutputNode* fNextNode;
        std::vector< VDPOutputNode* > fChildren;
};


}//end of vdp namespace


#endif /* VDPOutputNode_H__ */
