#include "VDPToken.hh"

namespace vdp{

VDPToken::VDPToken():fContents(""),fType(VDPTokenType::NullToken){}
VDPToken::~VDPToken(){;};

void VDPToken::SetContents(std::string& contents){fContents = contents;};
std::string VDPToken::GetContents() const {return fContents;};

void VDPToken::SetTokenType(VDPTokenType type){fType = type;};
VDPTokenType VDPToken::GetTokenType() const {return fType;};

}
