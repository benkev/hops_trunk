#ifndef VDPCallbackTypes_HH__
#define VDPCallbackTypes_HH__


#define CALL_MEMBER_FN(object_ptr, ptrToMember)  (object_ptr->*(ptrToMember))


namespace vdp{

/**
*
*@file VDPCallbackTypes.hh
*@class VDPCallbackTypes
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sun Dec 23 14:00:39 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//GETTERS
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


template<typename CallType, typename ReturnType, ReturnType (CallType::*memberFunction)() const>
struct VDPConstantReturnByValueGet
{
    ReturnType operator()(const CallType* ptr)
    {
        return CALL_MEMBER_FN(ptr, memberFunction)();
    };
};


template<typename CallType, typename ReturnType, const ReturnType* (CallType::*memberFunction)() const>
struct VDPConstantReturnByPointerGet
{
    const ReturnType* operator()(const CallType* ptr)
    {
        return CALL_MEMBER_FN(ptr, memberFunction)();
    };
};

template<typename CallType, typename ReturnType, void (CallType::*memberFunction)(ReturnType& ) const>
struct VDPConstantReturnByPassedReferenceGet
{
    void operator()(const CallType* ptr, ReturnType& val_ref)
    {
        CALL_MEMBER_FN(ptr, memberFunction)(val_ref);
    };
};

template< typename CallType, typename ReturnType, void (CallType::*memberFunction)(ReturnType* ) const>
struct VDPConstantReturnByPassedPointerGet
{
    void operator()(const CallType* ptr, ReturnType* val_ptr)
    {
        CALL_MEMBER_FN(ptr, memberFunction)(val_ptr);
    };
};


template <typename StructType, typename ReturnType, ReturnType StructType::*Member >
struct VDPStructMemberGet
{
    ReturnType operator()( const StructType* object)
    {
        return (*object).*Member;
    }
};

//just had to cross this off my bucket list...
template <typename StructType, typename ReturnType, typename ArrayType, ArrayType StructType::*Member >
struct VDPStructConstantReturnArrayByPointerGet
{
    const ReturnType* operator()( const StructType* object)
    {
        return static_cast<const ReturnType*>( &( ( (*object).*Member )[0]) );
    }
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////SETTERS
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

template< typename CallType, typename SetType, void (CallType::*memberFunction)(SetType) >
struct VDPPassByValueSet
{
    void operator()(CallType* ptr, SetType val)
    {
        CALL_MEMBER_FN(ptr, memberFunction)(val);
    };
};



template< typename CallType, typename SetType, void (CallType::*memberFunction)(const SetType&) >
struct VDPPassByConstantReferenceSet
{
    void operator()(CallType* ptr, const SetType& val)
    {
        CALL_MEMBER_FN(ptr, memberFunction)(val);
    };
};


template< typename CallType, typename SetType, void (CallType::*memberFunction)(const SetType*) >
struct VDPPassByConstantPointerSet
{
    void operator()(CallType* ptr, const SetType* val)
    {
        CALL_MEMBER_FN(ptr, memberFunction)(val);
    };
};

template <typename StructType, typename SetType, SetType StructType::*Member >
struct VDPStructMemberSet
{
    void operator()( StructType* object, SetType value )
    {
        (*object).*Member = value;
    }
};

}//end of vdp namespace

#endif /* VDPCallbackTypes_H__ */
