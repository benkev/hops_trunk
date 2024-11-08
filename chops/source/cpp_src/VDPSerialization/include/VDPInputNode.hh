#ifndef VDPInputNode_HH__
#define VDPInputNode_HH__

#define VDPNODE_MOVE_DOWNWARD -1 //indicates we need to decend to a child node
#define VDPNODE_MOVE_UPWARD 1 //indicates we need to ascend to parent
#define VDPNODE_STAY 0 //indicates we need to stay on the current node

#include "VDPObject.hh"

#include <map>
#include <vector>
#include <iostream>

namespace vdp{


/**
*
*@file VDPInputNode.hh
*@class VDPInputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Dec 27 23:03:26 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPInputNode: public VDPObject
{
    public:
        VDPInputNode(std::string name):
        VDPObject(name)
        {
            fStatus = VDPNODE_STAY;
            fIndex = 0;
	        fHasData = false;
            fNextNode = NULL;
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
        };

        VDPInputNode():
        VDPObject()
        {
            fStatus = VDPNODE_STAY;
            fIndex = 0;
	        fHasData = false;
            fNextNode = NULL;
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
        };

        virtual ~VDPInputNode()
        {
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                delete fChildren[i];
            }

            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
        };

        virtual bool TagsAreSuppressed(){return false;};

        virtual bool IsComposite(){return false;};

        virtual void AddChild(VDPInputNode* child)
        {
            fChildren.push_back( child );
            unsigned int index;
            index = fChildren.size() - 1;
            fChildrenStartMap.insert( std::pair<std::string, unsigned int >(child->GetStartTag(), index ) );
            fChildrenStopMap.insert( std::pair<std::string, unsigned int >(child->GetStopTag(), index) );
        };

        //next node will be set to NULL if the visitor traversing the tree
        //needs to move back to the parent, or stay on the current node
        virtual int GetNextNode(VDPInputNode*& next_node)
        {
            next_node = fNextNode;
            return fStatus;
        }

        virtual void Reset()
        {
            fStatus = VDPNODE_STAY;
            fIndex = 0;
            fNextNode = NULL;
	        fHasData = false;
        }

        virtual bool HasChildren() const
        {
            if( fChildren.size() == 0 ){return false;};
            return true;
        };

        virtual void FinalizeObject(){;};

        virtual void InitializeObject(){;}

        virtual void AddLine(const std::string& line)
        {
            //assumes the line has been trimmed of uncessary whitespace!!
            if( LineIsChildStartTag(line, fIndex) )
            {
                fStatus = VDPNODE_MOVE_DOWNWARD;
                fChildren[fIndex]->Reset();
                fNextNode = fChildren[fIndex];
            }
            else if( LineIsStopTag(line) )
            {
                fStatus = VDPNODE_MOVE_UPWARD;
                fNextNode = NULL;
                FinalizeObject();
            }
	        else if ( LineIsStartTag(line) )
	        {
                fHasData = true;
                fStatus = VDPNODE_STAY;
                fNextNode = NULL;
	        }
            else
            {
                //line is ignored, stay on current node
                fStatus = VDPNODE_STAY;
                fNextNode = NULL;
            }

        }

        void HasData(const bool& choice) { fHasData = choice;}
        bool HasData() const { return fHasData; }

    protected:

        bool LineIsStopTag(const std::string& line)
        {
            //check that first character is a START_TAG_BEGIN
            if(line.size() != 0)
            {
                if(line.at(0) == STOP_TAG_BEGIN[0] )
                {
                    if( line.find( GetStopTag() ) == 0)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        bool LineIsStartTag(const std::string& line)
        {
            //check that first character is a START_TAG_BEGIN
            if(line.size() != 0)
            {
                if(line.at(0) == START_TAG_BEGIN[0] )
                {
                    if( line.find( GetStartTag() ) == 0)
                    {
                        return true;
                    }
                }
            }

            return false;
        }


        bool LineIsChildStartTag(const std::string& line, int& index)
        {
            //check that first character is a START_TAG_BEGIN
            if(line.size() != 0)
            {
                if(line.at(0) == START_TAG_BEGIN[0] )
                {
                    fStartIT = fChildrenStartMap.find(line);
                    if(fStartIT != fChildrenStartMap.end() )
                    {
                        index = (*fStartIT).second;
                        return true;
                    }
                }
            }

            index = -1;
            return false;
        }

        bool LineIsChildStopTag(const std::string& line, int& index)
        {
            //check that first character is a START_TAG_BEGIN
            if(line.size() != 0)
            {
                if(line.at(0) == STOP_TAG_BEGIN[0] )
                {
                    fStopIT = fChildrenStopMap.find(line);
                    if(fStopIT != fChildrenStopMap.end() )
                    {
                        index = (*fStopIT).second;
                        return true;
                    }
                }
            }

            index = -1;
            return false;
        }


        int fStatus;
        int fIndex;
        bool fHasData;
        VDPInputNode* fNextNode;
        std::map< std::string, unsigned int > fChildrenStartMap;
        std::map< std::string, unsigned int > fChildrenStopMap;
        std::map< std::string, unsigned int >::iterator fStartIT;
        std::map< std::string, unsigned int >::iterator fStopIT;
        std::vector< VDPInputNode* > fChildren;

};



}


#endif /* VDPInputNode_H__ */
