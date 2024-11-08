#ifndef VDPEmptyOutputNode_HH__
#define VDPEmptyOutputNode_HH__

#include <vector>

#include "VDPObject.hh"
#include "VDPOutputNode.hh"

namespace vdp{

class VDPEmptyOutputNode: public VDPOutputNode
{
    public:

        VDPEmptyOutputNode(std::string name):VDPOutputNode(name){};
        VDPEmptyOutputNode():VDPOutputNode(){};

        virtual ~VDPEmptyOutputNode(){};

        virtual void Initialize(){;};

        virtual bool TagsAreSuppressed(){return false;};

        virtual bool IsComposite(){return false;};

        virtual void AddChild(VDPEmptyOutputNode* /*child*/){ /* cannot add child */ };

        //next node will be set to NULL if the visitor traversing the tree
        //needs to move back to the parent, or stay on the current node
        virtual int GetNextNode(VDPEmptyOutputNode*& next_node)
        {
            next_node = NULL;
            return VDPNODE_MOVE_UPWARD;
        }

        virtual void Reset(){};

        virtual bool HasChildren() const { return false; };

        virtual void GetLine(std::string& line)
        {
            line = GetStopTag() + std::string(LINE_DELIM);
            fStatus = VDPNODE_MOVE_UPWARD;
            fNextNode = NULL; //next node is parent
        }

    protected:

};


}//end of vdp namespace


#endif /* VDPEmptyOutputNode_H__ */
