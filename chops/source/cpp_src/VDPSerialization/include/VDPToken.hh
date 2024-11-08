#ifndef VDPToken_HH__
#define VDPToken_HH__

#include <string>

namespace vdp{

enum class VDPTokenType
{
    NullToken=0,
    UknownToken,
    DocumentEncodingToken,
    CommentStartToken,
    CommentToken,
    CommentEndToken,
    OpenStartTagToken,
    ClosedStartTagToken,
    AttributeAssignmentToken,
    AttributeNameToken,
    AttributeValueToken,
    ElementValueToken,
    StopTagToken,
};

class VDPToken
{
    public:
        VDPToken();
        virtual ~VDPToken();
        
        void SetContents(std::string& contents);
        std::string GetContents() const;
        
        void SetTokenType(VDPTokenType type);
        VDPTokenType GetTokenType() const;

    protected:
        
        std::string fContents;
        VDPTokenType fType;

};


} //end vdp

#endif /* end of include guard: VDPToken_HH__ */
