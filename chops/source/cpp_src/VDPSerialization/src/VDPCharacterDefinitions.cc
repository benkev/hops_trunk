#include "VDPCharacterDefinitions.hh"

namespace vdp
{

const std::string VDPCharacterDefinitions::fEncodingStart = "<?";
const std::string VDPCharacterDefinitions::fEncodingEnd = "?>";

const std::string VDPCharacterDefinitions::fXMLASCIIEncoding = "<?xml version=\"1.0\" encoding=\"ASCII\" standalone=\"yes\"?>";
const std::string VDPCharacterDefinitions::fXMLUTF8EEncoding = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

const std::string VDPCharacterDefinitions::fStartTagBegin = "<";
const std::string VDPCharacterDefinitions::fStartTagEnd = ">";
const std::string VDPCharacterDefinitions::fStartTagStop = "/>";

const std::string VDPCharacterDefinitions::fStopTagBegin = "</";
const std::string VDPCharacterDefinitions::fStopTagEnd = ">";

const std::string VDPCharacterDefinitions::fSpace = " ";
const std::string VDPCharacterDefinitions::fTab = "\t";
const std::string VDPCharacterDefinitions::fNewLine = "\n";

const std::string VDPCharacterDefinitions::fEqualSign = "=";
const std::string VDPCharacterDefinitions::fDoubleQuote = "\"";
const std::string VDPCharacterDefinitions::fSingleQuote = "\'";
const std::string VDPCharacterDefinitions::fCommentStart = "<!--";
const std::string VDPCharacterDefinitions::fDoubleDash = "--";
const std::string VDPCharacterDefinitions::fCommentStop = "-->";

const std::string VDPCharacterDefinitions::fWhiteSpace = " \t";

const std::string VDPCharacterDefinitions::fValidNameStartCharacterList = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

} //end of namespace vdp
