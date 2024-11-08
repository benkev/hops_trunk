#ifndef VDPPODInputNode_HH__
#define VDPPODInputNode_HH__

#include "VDPPODConverter.hh"
#include "VDPInputNode.hh"

namespace vdp{


/**
*
*@file VDPPODInputNode.hh
*@class VDPPODInputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Fri Dec 28 23:28:49 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


//should work as long as U is a pod type or a std::vector<> of a pod type
template < typename U >
class VDPPODInputNode: public VDPInputNode
{
    public:


        VDPPODInputNode(std::string name):VDPInputNode(name)
        {
            fConverter = new VDPPODConverter< U >();
            fChildren.clear();
        };

        virtual ~VDPPODInputNode()
        {
            delete fConverter;
        }

        virtual bool TagsAreSuppressed(){return true;};

        virtual bool IsComposite(){return false;};

        virtual void AddChild(VDPInputNode* /*child*/)
        {
            //no children allowed
        };

        //next node will be set to NULL if the visitor traversing the tree
        //needs to move back to the parent, or stay on the current node
        virtual int GetNextNode(VDPInputNode*& next_node)
        {
            next_node = fNextNode;
            return fStatus;
        }

        virtual bool HasChildren() const
        {
            return false;
        };

        void FinalizeObject(){;};

        void AddLine(const std::string& line)
        {
            //assumes the line has been trimmed of uncessary whitespace!!
            if( LineIsStopTag(line) )
            {
                fStatus = VDPNODE_MOVE_UPWARD;
                fNextNode = NULL;
                FinalizeObject();
            }
            else
            {
                //must be the string equivalent of the parameter
                fConverter->ConvertStringToParameter(line, fValue);
                fStatus = VDPNODE_STAY; //wait for stop tag
                fNextNode = NULL;
            }
        }

    protected:

        U fValue;
        VDPPODConverter< U >* fConverter;

    private:

        //cannot instantiate without providing a name
        VDPPODInputNode(){;};

};


}//end of vdp namespace


#endif /* VDPPODInputNode_H__ */
