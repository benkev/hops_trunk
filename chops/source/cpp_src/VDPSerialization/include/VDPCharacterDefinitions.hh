#ifndef VDPCharacterDefinitions_HH__
#define VDPCharacterDefinitions_HH__

#include <string>

namespace vdp 
{

class VDPCharacterDefinitions
{
    public:
        VDPCharacterDefinitions(){};
        virtual ~VDPCharacterDefinitions(){};
    
    public: 
        
        static const std::string fEncodingStart;
        static const std::string fEncodingEnd;
        static const std::string fXMLASCIIEncoding;
        static const std::string fXMLUTF8EEncoding;

        static const std::string fStartTagBegin;
        static const std::string fStartTagEnd;
        static const std::string fStartTagStop;
        
        static const std::string fStopTagBegin;
        static const std::string fStopTagEnd;
        
        static const std::string fSpace;
        static const std::string fTab;
        static const std::string fNewLine;

        static const std::string fEqualSign;
        static const std::string fDoubleQuote;
        static const std::string fSingleQuote;
        
        static const std::string fCommentStart;
        static const std::string fDoubleDash;
        static const std::string fCommentStop;

        static const std::string fWhiteSpace;
        static const std::string fValidNameStartCharacterList;
};

} //end of namespace vdp

#endif /* end of include guard: VDPCharacterDefinitions_HH__ */
