#ifndef VDPObject_HH__
#define VDPObject_HH__

#include "VDPDefinitions.hh"
#include <string>
#include <vector>
#include <map>
#include <list>



namespace vdp{

//we want to have the flexibility to have objects of the
//same type to have different names, but we also need to know
//their class name also, this can be defined for any class that
//needs it by adding the line:
// DefineVDPClassName(MyClass); to the header of class (after its definition)


template< typename Type >
class VDPClassName
{
    public:
    static std::string name()
    {
        return "INVALID";
    }
};

template< typename Type >
class VDPClassName< Type* >
{
    public:
    static std::string name()
    {
        return VDPClassName<Type>::name();
    }
};


template< typename Type >
class VDPClassName< std::vector< Type > >
{
    public:
    static std::string name()
    {
        std::string full_name = "vector";
        return full_name;
    }
};

template< typename Type >
class VDPClassName< std::list< Type > >
{
    public:
    static std::string name()
    {
        std::string full_name = "list";
        return full_name;
    }
};

template< typename Type >
class VDPClassName< std::vector< Type* > >
{
    public:
    static std::string name()
    {
        std::string full_name = "vector";
        return full_name;
    }
};

template< typename Type >
class VDPClassName< std::list< Type* > >
{
    public:
    static std::string name()
    {
        std::string full_name = "list";
        return full_name;
    }
};


template< typename TypeA, typename TypeB >
class VDPClassName< std::map< TypeA, TypeB > >
{
    public:
    static std::string name()
    {
        std::string full_name = "map";
        return full_name;
    }
};


template< typename TypeA, typename TypeB >
class VDPClassName< std::map< TypeA*, TypeB > >
{
    public:
    static std::string name()
    {
        std::string full_name = "map";
        return full_name;
    }
};

template< typename TypeA, typename TypeB >
class VDPClassName< std::map< TypeA, TypeB* > >
{
    public:
    static std::string name()
    {
        std::string full_name = "map";
        return full_name;
    }
};

template< typename TypeA, typename TypeB >
class VDPClassName< std::map< TypeA*, TypeB* > >
{
    public:
    static std::string name()
    {
        std::string full_name = "map";
        return full_name;
    }
};

#define DefineVDPClassName( className )		\
  						\
  template<> class VDPClassName< className >	\
  {						\
  public:					\
    static std::string name()			\
    {						\
      return #className;			\
    }						\
  };


/**
*
*@file VDPObject.hh
*@class VDPObject
*@brief container for a name string and start/stop tags
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Fri Dec 21 23:53:50 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

class VDPObject
{
    public:

        VDPObject()
        {
            fVDPObjectName = "INVALID";
            fVDPObjectStartTag = "INVALID";
            fVDPObjectStopTag = "INVALID";
            fVDPEmptyObjectTag = "INVALID";
        };

        VDPObject(std::string name):
        fVDPObjectName(name),
        fVDPObjectStartTag(std::string(START_TAG_BEGIN) + fVDPObjectName + std::string(START_TAG_END)),
        fVDPObjectStopTag(std::string(STOP_TAG_BEGIN) + fVDPObjectName + std::string(STOP_TAG_END)),
        fVDPEmptyObjectTag(std::string(START_TAG_BEGIN) + fVDPObjectName + std::string(START_TAG_STOP))
        {;};

        virtual ~VDPObject(){;};

        void SetName(std::string name)
        {
            fVDPObjectName = name;
            fVDPObjectStartTag = std::string(START_TAG_BEGIN) + fVDPObjectName + std::string(START_TAG_END);
            fVDPObjectStopTag = std::string(STOP_TAG_BEGIN) + fVDPObjectName + std::string(STOP_TAG_END);
            fVDPEmptyObjectTag = std::string(START_TAG_BEGIN) + fVDPObjectName + std::string(START_TAG_STOP);
        };

        virtual std::string GetName() const {return fVDPObjectName;};

        virtual std::string GetStartTag() const
        {
            return fVDPObjectStartTag;
        }

        virtual std::string GetStopTag() const
        {
            return fVDPObjectStopTag;
        }

        virtual std::string GetEmptyTag() const
        {
            return fVDPEmptyObjectTag;
        }

    protected:

        std::string fVDPObjectName;
        std::string fVDPObjectStartTag;
        std::string fVDPObjectStopTag;
        std::string fVDPEmptyObjectTag;

};



}//end of vdp namespace


#endif /* VDPObject_H__ */
