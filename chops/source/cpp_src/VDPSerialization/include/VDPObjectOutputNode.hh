#ifndef VDPObjectOutputNode_HH__
#define VDPObjectOutputNode_HH__

#include "VDPOutputNode.hh"
#include "VDPOutputObject.hh"
#include "VDPIsDerivedFrom.hh"

#include <list>
#include <vector>
#include <iostream>

namespace vdp{




/**
*
*@file VDPObjectOutputNode.hh
*@class VDPObjectOutputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Fri Dec 28 20:21:18 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



//forward declaration of the fixed size object
class VDPFixedSizeInputOutputObject;


//general case, T must inherit from VDPOutputObject but this is not explicitly enforced
template<typename T, unsigned int U = 0>
class VDPObjectOutputNode: public VDPOutputNode
{
  // general case: T is not derived from

    public:

        VDPObjectOutputNode(std::string name, const T* obj):VDPOutputNode(name),fObject(obj)
        {
            if(fObject != NULL) 
            {
                fObject->DefineOutputNode(this);
            }
        };

        VDPObjectOutputNode(std::string name):VDPOutputNode(name),fObject(NULL){;};

        VDPObjectOutputNode():VDPOutputNode(),fObject(NULL){;};

        virtual ~VDPObjectOutputNode(){;};

        virtual bool IsComposite(){return true;};

        void AttachObjectToNode(const T* object_ptr)
        {
            fObject = object_ptr;
            fObject->DefineOutputNode(this);
        };

        void DetachObjectFromNode(){fObject = NULL;};

        const T* GetObject() const {return fObject;};

    protected:

        const T* fObject;
};



//special case, T inherits from VDPFixedSizeInputOutputObject
template<typename T>
class VDPObjectOutputNode<T , 1>: public VDPOutputNode
{

    //this has not been changed from the general case yet...so everything should function
    //the same as before
    public:

        VDPObjectOutputNode(std::string name, const T* obj):VDPOutputNode(name),fObject(obj)
        {
            fTagSuppression = true;
        };

        VDPObjectOutputNode(std::string name):VDPOutputNode(name),fObject(NULL)
        {
            fTagSuppression = true;
        };

        VDPObjectOutputNode():VDPOutputNode(),fObject(NULL){;};

        virtual ~VDPObjectOutputNode(){;};

        void AttachObjectToNode(const T* object_ptr)
        {
            fObject = object_ptr;
            fObject->DefineOutputNode(this);
        };

        void DetachObjectFromNode(){fObject = NULL;};

        const T* GetObject() const {return fObject;};

        //need to redefine the following functions which were originally defined in VDPOutputNode

        virtual bool IsComposite(){return true;};

        virtual bool TagsAreSuppressed(){return fTagSuppression;}

        virtual void Initialize(){;}; //maybe don't need to do anything with this one

        virtual void AddChild(VDPOutputNode* child)
        {
            if( child->IsComposite() && !(child->TagsAreSuppressed()) )
            {
                fTagSuppression = false;
            }

            //need to associate this child and it's tag with a particlar position
            fChildren.push_back(child);

        };

//        //next node will be set to NULL if the visitor traversing the tree
//        //needs to move back to the parent, or stay on the current node
//        virtual int GetNextNode(VDPOutputNode*& next_node)
//        {
//            next_node = fNextNode;
//            return fStatus;
//        }

        virtual void Reset()
        {
            fTagSuppression = true;
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
                if( fIndex >=0 && fIndex < (int)fChildren.size() )
                {
                    //open the next node, or output its contents w/o tags
                    if( fChildren[fIndex]->IsComposite() )
                    {
                        line = fChildren[fIndex]->GetStartTag() + std::string(LINE_DELIM);
                        fStatus = VDPNODE_MOVE_DOWNWARD;
                        fNextNode = fChildren[fIndex];
                        fIndex++;
                    }
                    else
                    {
                        fChildren[fIndex]->GetLine(line);
                        fStatus = VDPNODE_STAY;
                        fNextNode = NULL;
                        fIndex++;
                    }
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
        //^^breaking the above rule, this is a composite node!
        virtual std::string GetSingleLine()
        {
            //loop over children adding their strings into a single line, they must
            //return a single line string!!

            return std::string("INVALID");


        };



        bool fTagSuppression; //only true if object is truely composed of fixed size objects

        //we need to map


        const T* fObject;
};




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//here is the partial specialization for vectors of objects


//T must inherit from VDPOutputObject
template< typename T >
class VDPObjectOutputNode< std::vector< T > >: public VDPOutputNode
{
    public:

        VDPObjectOutputNode(std::string name, const std::vector< T >* obj):VDPOutputNode(name),fObject(obj)
        {
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
            fElementName = VDPClassName<T>::name();
            //all elements must have the same name, this is required by input
        };

        VDPObjectOutputNode(std::string name):VDPOutputNode(name),fObject(NULL)
        {
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
            fElementName = VDPClassName<T>::name();
            //all elements must have the same name, this is required by input
        };

        VDPObjectOutputNode():VDPOutputNode(),fObject(NULL)
        {
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
            fElementName = VDPClassName<T>::name();
            //all elements must have the same name, this is required by input
        };

        virtual ~VDPObjectOutputNode()
        {
            delete fElementNode;
        };

        virtual void Initialize()
        {

        };

        void AttachObjectToNode(const std::vector< T >* object_ptr)
        {
            fObject = object_ptr;
        };

        void DetachObjectFromNode(){fObject = NULL;};

        const std::vector< T >* GetObject() const {return fObject;};

        virtual void GetLine(std::string& line)
        {
            if(fObject->size() != 0)
            {
                //this iterates over all the elements of the vector object
                //(they are considered the child nodes) anything in fChildren is ignored
  	            if( fIndex >=0 && fIndex < (int)(fObject->size()) )
                {
                    //open the next node
                    fElementNode->DetachObjectFromNode();
                    fElementNode->Reset();
                    fElementNode->SetName(fElementName );
                    fElementNode->AttachObjectToNode( &(fObject->at(fIndex)) );
                    fElementNode->Initialize();
                    line = fElementNode->GetStartTag() + std::string(LINE_DELIM);
                    fStatus = VDPNODE_MOVE_DOWNWARD;
                    VDPOutputNode::fNextNode = fElementNode;
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
                //vector is empty, so close it out with no elements
                line = GetStopTag() + std::string(LINE_DELIM);
                fStatus = VDPNODE_MOVE_UPWARD;
                fNextNode = NULL; //next node is parent
            }
        }

    protected:

        std::string fElementName;
        VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >* fElementNode;
        const std::vector< T >* fObject;

};




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//here is the partial specialization for vectors of pointers to objects


//T must inherit from VDPOutputObject
template< typename T >
class VDPObjectOutputNode< std::vector< T* > >: public VDPOutputNode
{
    public:

        VDPObjectOutputNode(std::string name, const std::vector< T* >* obj):VDPOutputNode(name),fObject(obj)
        {
            //fElementNode = new VDPObjectOutputNode< T >();
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
        };

        VDPObjectOutputNode(std::string name):VDPOutputNode(name),fObject(NULL)
        {
            //fElementNode = new VDPObjectOutputNode< T >();
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
        };

        VDPObjectOutputNode():VDPOutputNode(),fObject(NULL)
        {
            //fElementNode = new VDPObjectOutputNode< T >();
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
        };

        virtual ~VDPObjectOutputNode()
        {
            delete fElementNode;
        };

        virtual void Initialize()
        {

        };

        void AttachObjectToNode(const std::vector< T* >* object_ptr)
        {
            fObject = object_ptr;
        };

        void DetachObjectFromNode(){fObject = NULL;};

        const std::vector< T* >* GetObject() const {return fObject;};

        virtual void GetLine(std::string& line)
        {
            if(fObject->size() != 0)
            {
                //this iterates over all the elements of the vector object
                //(they are considered the child nodes) anything in fChildren is ignored
	        if( fIndex >=0 && fIndex < (int)(fObject->size()) )
                {
                    //open the next node
                    fElementNode->DetachObjectFromNode();
                    fElementNode->Reset();

                    //calling ClassName is needed in case there are objects
                    //which derive from T in the std::vector< T* >
                    fElementNode->SetName( fObject->at(fIndex)->ClassName() );
                    fElementNode->AttachObjectToNode( fObject->at(fIndex) );
                    fElementNode->Initialize();
                    line = fElementNode->GetStartTag() + std::string(LINE_DELIM);
                    fStatus = VDPNODE_MOVE_DOWNWARD;
                    VDPOutputNode::fNextNode = fElementNode;
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
                //vector is empty, so close it out with no elements
                line = GetStopTag() + std::string(LINE_DELIM);
                fStatus = VDPNODE_MOVE_UPWARD;
                fNextNode = NULL; //next node is parent
            }
        }

    protected:

        std::string fElementName;
//        VDPObjectOutputNode< T >* fElementNode;
        VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >* fElementNode;
        const std::vector< T* >* fObject;

};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//here is the partial specialization for lists of objects


//T must inherit from VDPOutputObject
template< typename T >
class VDPObjectOutputNode< std::list< T > >: public VDPOutputNode
{
    public:

        VDPObjectOutputNode(std::string name, const std::list< T >* obj):VDPOutputNode(name),fObject(obj)
        {
            fObjIT = fObject->begin();
//            fElementNode = new VDPObjectOutputNode< T >();
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
            fElementName = VDPClassName<T>::name();
            //all elements must have the same name, this is required by input
            fFirstCall = true;
        };

        VDPObjectOutputNode(std::string name):VDPOutputNode(name),fObject(NULL)
        {
//            fElementNode = new VDPObjectOutputNode< T >();
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
            fElementName = VDPClassName<T>::name();
            //all elements must have the same name, this is required by input
            fFirstCall = true;
        };

        VDPObjectOutputNode():VDPOutputNode(),fObject(NULL)
        {
           //fElementNode = new VDPObjectOutputNode< T >();
            fElementNode = new VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >();
        };

        virtual ~VDPObjectOutputNode()
        {
            delete fElementNode;
        };

        virtual void Initialize()
        {

        };

        void AttachObjectToNode(const std::list< T >* object_ptr)
        {
            fObject = object_ptr;
            fObjIT = fObject->begin();
            fFirstCall = true;
        };

        void DetachObjectFromNode(){fObject = NULL;};

        const std::list< T >* GetObject() const {return fObject;};

        virtual void GetLine(std::string& line)
        {
            if(fFirstCall)
            {
                fObjIT = fObject->begin();
                fFirstCall = false;
            }

            if(fObject->size() != 0)
            {
                //this iterates over all the elements of the vector object
                //(they are considered the child nodes) anything in fChildren is ignored
                if( fObjIT != fObject->end() )
                {
                    //open the next node
                    fElementNode->DetachObjectFromNode();
                    fElementNode->Reset();
                    fElementNode->SetName(fElementName );
                    fElementNode->AttachObjectToNode( &(*fObjIT) );
                    fElementNode->Initialize();
                    line = fElementNode->GetStartTag() + std::string(LINE_DELIM);
                    fStatus = VDPNODE_MOVE_DOWNWARD;
                    VDPOutputNode::fNextNode = fElementNode;
                    fObjIT++;
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
                //vector is empty, so close it out with no elements
                line = GetStopTag() + std::string(LINE_DELIM);
                fStatus = VDPNODE_MOVE_UPWARD;
                fNextNode = NULL; //next node is parent
            }
        }

    protected:

        std::string fElementName;
//        VDPObjectOutputNode< T >* fElementNode;
        VDPObjectOutputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >* fElementNode;
        const std::list< T >* fObject;
        typename std::list< T >::const_iterator fObjIT;
        bool fFirstCall;


};


} //end of vdp namespace


#endif /* VDPObjectOutputNode_H__ */
