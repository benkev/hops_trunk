#ifndef VDPObjectInputNode_HH__
#define VDPObjectInputNode_HH__


#include "VDPInputNode.hh"
#include "VDPCallbackTypes.hh"
#include "VDPIsDerivedFrom.hh"
#include <vector>
#include <list>
#include <typeinfo>

namespace vdp{


/**
*
*@file VDPObjectInputNode.hh
*@class VDPObjectInputNode
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Mon Dec 31 15:03:00 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/



//forward declaration of the fixed size object
class VDPFixedSizeInputOutputObject;


//general case, T must inherit from VDPInputObject but this is not explicitly enforced
template<typename T, unsigned int U = 0>
class VDPObjectInputNode: public VDPInputNode
{
    public:

        VDPObjectInputNode(std::string name):
        VDPInputNode(name)
        {
            fObject = new T();
            fObject->DefineInputNode(this);
        }

        VDPObjectInputNode():
        VDPInputNode()
        {
            fObject = new T();
            fObject->DefineInputNode(this);
        }

        virtual ~VDPObjectInputNode()
        {
            delete fObject;
        }

        virtual bool TagsAreSuppressed(){return false;};

        virtual bool IsComposite(){return true;};

        virtual void Reset()
        {
            fIndex = 0;
            fStatus = VDPNODE_STAY;
            fNextNode = NULL;
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                fChildren[i]->Reset();
            }
        }


        virtual T* GetObject(){return fObject;};

        void AttachObjectToNode(T* &object_ptr) { object_ptr = fObject; }
        void DetachObjectFromNode(){fObject = NULL;};

        virtual void FinalizeObject()
        {
            //fObject define a function called Initialize()
            //this is guaranteed if fObject inherits from VDPInputOutputObject
            fObject->Initialize();
        };

        virtual void InitializeObject()
        {
            //fObject define a function called Initialize()
            //this is guaranteed if fObject inherits from VDPInputOutputObject
            fObject->Initialize();
        }

    protected:

        T* fObject;

};




//special case, T inherits from VDPFixedSizeInputOutputObject
template<typename T>
class VDPObjectInputNode<T , 1>: public VDPInputNode
{

    public:

        VDPObjectInputNode(std::string name):
        VDPInputNode(name)
        {
            fIndex = 0;
            fObject = new T();
            fObject->DefineInputNode(this);
            fTagSuppression = true;
        }

        VDPObjectInputNode():
        VDPInputNode()
        {
            fIndex = 0;
            fObject = new T();
            fObject->DefineInputNode(this);
            fTagSuppression = true;
        }

        virtual ~VDPObjectInputNode()
        {
            delete fObject;
        }

        virtual bool TagsAreSuppressed(){return fTagSuppression;};

        virtual bool IsComposite(){return true;};

        virtual void Reset()
        {
            fIndex = 0;
            fStatus = VDPNODE_STAY;
            fNextNode = NULL;
            fTagSuppression = true;
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                fChildren[i]->Reset();
            }
        }


        virtual T* GetObject(){return fObject;};

        void AttachObjectToNode(T* &object_ptr) { object_ptr = fObject; }
        void DetachObjectFromNode(){fObject = NULL;};

        virtual void FinalizeObject()
        {
            //fObject define a function called Initialize()
            //this is guaranteed if fObject inherits from VDPInputOutputObject
            fObject->Initialize();
        };

        virtual void InitializeObject()
        {
            //fObject define a function called Initialize()
            //this is guaranteed if fObject inherits from VDPInputOutputObject
            fObject->Initialize();
        }


        virtual void AddChild(VDPInputNode* child)
        {

            if( child->IsComposite() && !(child->TagsAreSuppressed()) )
            {
                fTagSuppression = false;
            }

            //need to associate this child and it's tag with a particlar position
            fChildren.push_back(child);

            unsigned int index;
            index = fChildren.size() - 1;
            fChildrenStartMap.insert( std::pair<std::string, unsigned int >(child->GetStartTag(), index ) );
            fChildrenStopMap.insert( std::pair<std::string, unsigned int >(child->GetStopTag(), index) );
        };


        virtual void AddLine(const std::string& line)
        {

            int temp;
            //assumes the line has been trimmed of uncessary whitespace!!
            if( LineIsChildStartTag(line, temp) )
            {
                fStatus = VDPNODE_MOVE_DOWNWARD;
                fChildren[temp]->Reset();
                fNextNode = fChildren[temp];
                fIndex++;
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
                fIndex = 0;
                fStatus = VDPNODE_STAY;
                fNextNode = NULL;
	        }
            else
            {
                //line is indexed data
                if( fIndex >=0 && fIndex < (int)fChildren.size() )
                {
                    if( !(fChildren[fIndex]->IsComposite() ) )
                    {
                        fChildren[fIndex]->AddLine(line);
                        fChildren[fIndex]->FinalizeObject();
                    }
                    fIndex++;
                    fStatus = VDPNODE_STAY;
                    fNextNode = NULL;
                }
            }

        }

        void HasData(const bool& choice) { fHasData = choice;}
        bool HasData() const { return fHasData; }

    protected:

        bool fTagSuppression;
        T* fObject;

};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//specialization instances where the object is passed to the node

template<typename T, unsigned int U = 0>
class VDPExternalObjectInputNode: public VDPInputNode
{
    public:

        VDPExternalObjectInputNode(std::string name):
        VDPInputNode(name)
        {
        }

        VDPExternalObjectInputNode():
        VDPInputNode()
        {
        }

        virtual ~VDPExternalObjectInputNode()
        {
 	        delete fObject;
        }

        virtual bool TagsAreSuppressed(){return false;};

        virtual bool IsComposite(){return true;};

        virtual void Reset()
        {
            fIndex = 0;
            fStatus = VDPNODE_STAY;
            fNextNode = NULL;
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                fChildren[i]->Reset();
            }
        }


        virtual T* GetObject(){return fObject;};

        void CloneFromObject(T* object_ptr)
        {fObject = object_ptr->ClonePrimitive();fObject->DefineInputNode(this);}

        void AttachObjectToNode(T* &object_ptr) { object_ptr = fObject; }
        void DetachObjectFromNode(){fObject = NULL;};

        virtual void FinalizeObject()
        {
            //fObject define a function called Initialize()
            //this is guaranteed if fObject inherits from VDPInputOutputObject
            fObject->Initialize();
        };

        virtual void InitializeObject()
        {
            //fObject define a function called Initialize()
            //this is guaranteed if fObject inherits from VDPInputOutputObject
            fObject->Initialize();
        }

    protected:

        T* fObject;

};


////////////////////////////////////////////////////////////////////////////////
//External object specialization for VDPFixedSizeInputOutputObject


//special case, T inherits from VDPFixedSizeInputOutputObject
template<typename T>
class VDPExternalObjectInputNode<T , 1>: public VDPInputNode
{

    public:

        VDPExternalObjectInputNode(std::string name):
        VDPInputNode(name)
        {
            fIndex = 0;
            fTagSuppression = true;
        }

        VDPExternalObjectInputNode():
        VDPInputNode()
        {
            fIndex = 0;
            fTagSuppression = true;
        }

        virtual ~VDPExternalObjectInputNode()
        {
            delete fObject;
        }

        virtual bool TagsAreSuppressed(){return fTagSuppression;};

        virtual bool IsComposite(){return true;};

        virtual void Reset()
        {
            fIndex = 0;
            fTagSuppression = true;
            fStatus = VDPNODE_STAY;
            fNextNode = NULL;
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                fChildren[i]->Reset();
            }
        }


        virtual T* GetObject(){return fObject;};

        void CloneFromObject(T* object_ptr)
        {fObject = object_ptr->ClonePrimitive();fObject->DefineInputNode(this);}

        void AttachObjectToNode(T* &object_ptr) { object_ptr = fObject; }
        void DetachObjectFromNode(){fObject = NULL;};

        virtual void FinalizeObject()
        {
            //fObject define a function called Initialize()
            //this is guaranteed if fObject inherits from VDPInputOutputObject
            fObject->Initialize();
        };

        virtual void InitializeObject()
        {
            //fObject define a function called Initialize()
            //this is guaranteed if fObject inherits from VDPInputOutputObject
            fObject->Initialize();
        }


        virtual void AddChild(VDPInputNode* child)
        {
            if( child->IsComposite() && !(child->TagsAreSuppressed()) )
            {
                fTagSuppression = false;
            }

            //need to associate this child and it's tag with a particlar position
            fChildren.push_back(child);

            unsigned int index;
            index = fChildren.size() - 1;
            fChildrenStartMap.insert( std::pair<std::string, unsigned int >(child->GetStartTag(), index ) );
            fChildrenStopMap.insert( std::pair<std::string, unsigned int >(child->GetStopTag(), index) );
        };

        virtual void AddLine(const std::string& line)
        {
            //assumes the line has been trimmed of uncessary whitespace!!
            int temp;
            if( LineIsChildStartTag(line, temp) )
            {
                fStatus = VDPNODE_MOVE_DOWNWARD;
                fChildren[fIndex]->Reset();
                fNextNode = fChildren[temp];
                fIndex++;
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
                fIndex = 0;
                fStatus = VDPNODE_STAY;
                fNextNode = NULL;
	        }
            else
            {
                //line is indexed data
                if( fIndex >=0 && fIndex < fChildren.size() )
                {
                    if( !(fChildren[fIndex]->IsComposite() ) )
                    {
                        fChildren[fIndex]->AddLine(line);
                        fChildren[fIndex]->FinalizeObject();
                    }
                    fIndex++;
                    fStatus = VDPNODE_STAY;
                    fNextNode = NULL;
                }
            }

        }

        void HasData(const bool& choice) { fHasData = choice;}
        bool HasData() const { return fHasData; }

    protected:

        bool fTagSuppression;
        T* fObject;

};



////////////////////////////////////////////////////////////////////////////////
///////Callback with reference
template<typename CallType, typename SetType, void (CallType::*memberFunction)(const SetType&) >
class VDPAssociatedReferenceObjectInputNode: public VDPObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >
{
    public:

        VDPAssociatedReferenceObjectInputNode(std::string name, CallType* call_ptr):VDPObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >( name )
        {
            fCallPtr = call_ptr;
        }

        void FinalizeObject()
        {
            this->InitializeObject();
            fCallback(fCallPtr, *(this->fObject) );
        }


        void SetCallbackObject(CallType* obj)
        {
            fCallPtr = obj;
        }


        virtual ~VDPAssociatedReferenceObjectInputNode()
        {

        };


    protected:

        CallType* fCallPtr;
        VDPPassByConstantReferenceSet< CallType, SetType, memberFunction > fCallback;


};

///////////////Callback with pointer
template<typename CallType, typename SetType, void (CallType::*memberFunction)(const SetType*) >
class VDPAssociatedPointerObjectInputNode: public VDPObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >
{
    public:

        VDPAssociatedPointerObjectInputNode(std::string name, CallType* call_ptr):VDPObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >( name )
        {
            fCallPtr = call_ptr;
        }

        void FinalizeObject()
        {
            this->InitializeObject();
            fCallback(fCallPtr, this->fObject );
        }


        void SetCallbackObject(CallType* obj)
        {
            fCallPtr = obj;
        }


        virtual ~VDPAssociatedPointerObjectInputNode()
        {

        };


    protected:

        CallType* fCallPtr;
        VDPPassByConstantPointerSet< CallType, SetType, memberFunction > fCallback;


};


///////////////Callback with pointer
//CallType must be a std::vector<Type*>
//and SetType must inherit from Type
template<typename CallType, typename SetType >
class VDPAssociatedAllocatedToVectorPointerObjectInputNode: public VDPObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >
{
    public:

        VDPAssociatedAllocatedToVectorPointerObjectInputNode(std::string name, CallType* call_ptr):VDPObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >( name )
        {
            fCallPtr = call_ptr;
        }

        VDPAssociatedAllocatedToVectorPointerObjectInputNode(std::string name):VDPObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >( name )
        {
            fCallPtr = NULL;
        }


        void FinalizeObject()
        {
            this->InitializeObject();
            fCallPtr->push_back( new SetType( *(this->fObject ) ) );
        }

        void SetCallbackObject(CallType* obj)
        {
            fCallPtr = obj;
        }


        virtual ~VDPAssociatedAllocatedToVectorPointerObjectInputNode()
        {

        };


    protected:

        CallType* fCallPtr;


};





///////////////Callback with pointer
//CallType must be a std::vector<Type*>
//and SetType must inherit from Type
template<typename CallType, typename SetType >
class VDPAssociatedAllocatedToVectorPointerExternalObjectInputNode: public VDPExternalObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >
{
    public:

        VDPAssociatedAllocatedToVectorPointerExternalObjectInputNode(std::string name, CallType* call_ptr):VDPExternalObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >( name )
        {
            fCallPtr = call_ptr;
        }

        VDPAssociatedAllocatedToVectorPointerExternalObjectInputNode(std::string name):VDPExternalObjectInputNode< SetType, VDPIsDerivedFrom< SetType, VDPFixedSizeInputOutputObject >::Is >( name )
        {
            fCallPtr = NULL;
        }


        void FinalizeObject()
        {
            this->InitializeObject();
            fCallPtr->push_back( this->fObject->ClonePrimitive() );
        }

        void SetCallbackObject(CallType* obj)
        {
            fCallPtr = obj;
        }

        virtual ~VDPAssociatedAllocatedToVectorPointerExternalObjectInputNode()
        {

        };


    protected:

        CallType* fCallPtr;

};




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//specialization for vectors of objects


template< typename T >
class VDPObjectInputNode< std::vector< T > >: public VDPInputNode
{
    public:

        VDPObjectInputNode(std::string name):
        VDPInputNode(name)
        {
            fObject = new std::vector< T >();
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedReferenceObjectInputNode< std::vector< T >, T, &std::vector< T >::push_back >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
            VDPInputNode::AddChild(fElementNode);

            fEnable = false;
            fObjectIsOwned = true;
            fIndex = 0;
        }


        VDPObjectInputNode(std::string name, std::vector< T >* object_ptr):
        VDPInputNode(name)
        {
            fObject = object_ptr;
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedReferenceObjectInputNode< std::vector< T >, T, &std::vector< T >::push_back >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
            VDPInputNode::AddChild(fElementNode);

            fEnable = false;
            fObjectIsOwned = false;
            fIndex = 0;
        }



        VDPObjectInputNode()
        {
            fObject = new std::vector< T >();
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedReferenceObjectInputNode< std::vector< T >, T, &std::vector< T >::push_back >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();

            fEnable = false;
            fObjectIsOwned = true;
            fIndex = 0;

            VDPInputNode::AddChild(fElementNode);
        }

        virtual ~VDPObjectInputNode()
        {
            if(fObjectIsOwned)
            {
                delete fObject;
            }
        }

        virtual void Reset()
        {
            fIndex = 0;
            fStatus = VDPNODE_STAY;
            fNextNode = NULL;
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                fChildren[i]->Reset();
            }

            fObject->clear(); //this isn't a memory leak :)
        }

        template <typename TheCallType, typename TheSetType >
        void AddChild(VDPAssociatedAllocatedToVectorPointerObjectInputNode< TheCallType, TheSetType>* child)
        {
            if(child != NULL)
            {
                //forward it on to fElementNode
                fElementNode->template AddChild<TheCallType, TheSetType>(child);

                //maybe add a check that T is a std::vector< U > before we forward
                //this on to the element node, otherwise this will cause a compiler error
                //although if a user does that it makes no sense anyways so maybe that is ok

            }
        }

        template <typename TheCallType, typename TheSetType >
        void AddChild(VDPAssociatedAllocatedToVectorPointerExternalObjectInputNode< TheCallType, TheSetType>* child)
        {
            if(child != NULL)
            {
                //forward it on to fElementNode
                fElementNode->template AddChild<TheCallType, TheSetType>(child);
            }
        }

        void FinalizeObject(){;};

        virtual std::vector< T >* GetObject(){return fObject;};

   protected:

        VDPObjectInputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >* fElementNode;
        std::vector< T >* fObject;
        std::string fElementName;
        bool fEnable;
        bool fObjectIsOwned;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//specialization for vectors of pointers to objects


template< typename T >
class VDPObjectInputNode< std::vector< T* > >: public VDPInputNode
{
    public:

        VDPObjectInputNode(std::string name):
        VDPInputNode(name)
        {
            fObject = new std::vector< T* >();
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedAllocatedToVectorPointerObjectInputNode<  std::vector< T* > , T >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
            VDPInputNode::AddChild(fElementNode);

            fEnable = false;
            fObjectIsOwned = true;
            fIndex = 0;
        }


        VDPObjectInputNode(std::string name, std::vector< T* >* object_ptr):
        VDPInputNode(name)
        {
            fObject = object_ptr;
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedAllocatedToVectorPointerObjectInputNode<  std::vector< T* > , T >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
            VDPInputNode::AddChild(fElementNode);

            fEnable = false;
            fObjectIsOwned = false;
            fIndex = 0;
        }



        VDPObjectInputNode()
        {
            fObject = new std::vector< T* >();
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedAllocatedToVectorPointerObjectInputNode<  std::vector< T* > , T >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();

            fEnable = false;
            fObjectIsOwned = true;
            fIndex = 0;

            VDPInputNode::AddChild(fElementNode);
        }

        virtual ~VDPObjectInputNode()
        {
            if(fObjectIsOwned)
            {
                delete fObject;
            }
        }

        virtual void Reset()
        {
            fIndex = 0;
            fStatus = VDPNODE_STAY;
            fNextNode = NULL;
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                fChildren[i]->Reset();
            }

            fObject->clear();
            //this^^^ isn't directly a memory leak, since the pointers
            //still exist (after they've been push_back'd into the callback object
            //that being said...they should be destroyed in the callback objects destructor
            //or there will be a mem leak
        }

        template <typename TheCallType, typename TheSetType >
        void AddChild(VDPAssociatedAllocatedToVectorPointerObjectInputNode< TheCallType, TheSetType>* child)
        {
            if(child != NULL)
            {
                T* ptr = NULL;
                ptr = dynamic_cast< T* >(child->GetObject() );

                if(ptr != NULL)
                {
                    //T and Type must be virtual and either the same or base and derived
                    child->SetCallbackObject(fObject);
                    VDPInputNode::AddChild(child);
                }
                //T and Type are not base and derived...so ignore them
            }
        }

        template <typename TheCallType, typename TheSetType >
        void AddChild(VDPAssociatedAllocatedToVectorPointerExternalObjectInputNode< TheCallType, TheSetType>* child)
        {
            if(child != NULL)
            {
                T* ptr = NULL;
                ptr = dynamic_cast< T* >(child->GetObject() );

                if(ptr != NULL)
                {
                    //T and Type must be virtual and either the same or base and derived
                    child->SetCallbackObject(fObject);
                    VDPInputNode::AddChild(child);
                }
                //T and Type are not base and derived...so ignore them
            }
        }

        virtual std::vector< T* >* GetObject(){return fObject;};


   protected:

        VDPObjectInputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >* fElementNode;
        std::vector< T* >* fObject;
        std::string fElementName;
        bool fEnable;
        bool fObjectIsOwned;

};



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//specialization for lists of objects


template< typename T >
class VDPObjectInputNode< std::list< T > >: public VDPInputNode
{
    public:

        VDPObjectInputNode(std::string name):
        VDPInputNode(name)
        {
            fObject = new std::list< T >();
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedReferenceObjectInputNode< std::list< T >, T, &std::list< T >::push_back >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
            AddChild(fElementNode);

            fEnable = false;
            fObjectIsOwned = true;
            fIndex = 0;
        }


        VDPObjectInputNode(std::string name, std::list< T >* object_ptr):
        VDPInputNode(name)
        {
            fObject = object_ptr;
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedReferenceObjectInputNode< std::list< T >, T, &std::list< T >::push_back >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();
            AddChild(fElementNode);

            fEnable = false;
            fObjectIsOwned = false;
            fIndex = 0;
        }



        VDPObjectInputNode()
        {
            fObject = new std::list< T >();
            fObject->clear();
            fElementName = VDPClassName< T >::name();
            fElementNode = new VDPAssociatedReferenceObjectInputNode< std::list< T >, T, &std::list< T >::push_back >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();

            fEnable = false;
            fObjectIsOwned = true;
            fIndex = 0;

            AddChild(fElementNode);
        }

        virtual ~VDPObjectInputNode()
        {
            if(fObjectIsOwned)
            {
                delete fObject;
            }
        }

        virtual void Reset()
        {
            fIndex = 0;
            fStatus = VDPNODE_STAY;
            fNextNode = NULL;
            for(unsigned int i = 0; i<fChildren.size(); i++)
            {
                delete fChildren[i];
            }

            fObject->clear();
            fElementName = VDPClassName<T>::name();
            fElementNode = new VDPAssociatedReferenceObjectInputNode< std::list< T >, T, &std::list< T >::push_back >(fElementName, fObject);

            //the only child is the fElementNode
            fChildren.clear();
            fChildrenStartMap.clear();
            fChildrenStopMap.clear();

            AddChild(fElementNode);
        }

        void FinalizeObject(){;};

        virtual std::list< T >* GetObject(){return fObject;};

   protected:

        VDPObjectInputNode< T, VDPIsDerivedFrom< T, VDPFixedSizeInputOutputObject >::Is >* fElementNode;
        std::list< T >* fObject;
        std::string fElementName;
        bool fEnable;
        bool fObjectIsOwned;

};



}//end of namespace


#endif /* VDPObjectInputNode_H__ */
