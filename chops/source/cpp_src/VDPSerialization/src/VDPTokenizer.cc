#include "VDPTokenizer.hh"

namespace vdp{

VDPTokenizer::VDPTokenizer()
{
    fString = NULL;
};

VDPTokenizer::~VDPTokenizer(){;};

void VDPTokenizer::SetString(const std::string* aString)
{
    fString = aString;
};

void VDPTokenizer::ProcessString()
{
    fTokens.clear();

    if(fString != NULL)
    {
        fStart = 0;
        fLength = 0;
        fEnd = fString->size();
        
        while(fStart < fEnd && fState != TokenizerState::Error )
        {

            switch(fState)
            {
                case TokenizerState::BeginDocument:
                break;

                case TokenizerState::ReadyForNextElement:
                    PrepareNextElement();
                break;
                
                case TokenizerState::ProcessingEncoding:
                    ProcessingEncoding();
                break;
                
                case TokenizerState::ProcessingComment:
                    ProcessComment();
                break;
                
                case TokenizerState::ProcessingElementStartTag:
                    ProcessElementStartTag();
                break;
                
                case TokenizerState::ProcessingElementStopTag:
                    ProcessElementStopTag();
                break;
                
                case TokenizerState::ProcessingElementAttributes:
                    ProcessAttributeAssignment();
                break;
                
                case TokenizerState::ProcessingElementContents:
                    ProcessElementContents();
                break;
                
                default:
                    //tokenizer entered an unknown state
                    fState = TokenizerState::Error
            }
        }
    }
}

void VDPTokenizer::GetTokens(std::vector< std::string>* tokens) const;
{
    tokens->clear();
    if(tokens != NULL && fString != NULL)
    {

    }
}

void VDPTokenizer::PrepareNextElement()
{
    //locate the next '<' character
    size_t elem_start = fString->find_first_of(VDPCharacterDefinitions::fStartTagBegin, fStart);
    if(elem_start != std::string::npos)
    {
        //now figure out if we have a start tag, encoding statement, or comment
        if( StartsWithSubstring( elem_start, VDPCharacterDefinitions::fCommentStart) )
        {
            //we have a comment
            fState = TokenizerState::ProcessingComment;
            //skip ahead to the end of the comment start tag
            fStart = elem_start;
        }
        else if( StartsWithSubstring(elem_start, VDPCharacterDefinitions::fEncodingStart) )
        {
            //we have an encoding statement
            fState = TokenizerState::ProcessingEncoding;
            fStart = elem_start;
        }
        else if( NextCharacterIsAny(elem_start, VDPCharacterDefinitions::fValidNameStartCharacterList) )
        {
            //we have an element start tag
            fState = TokenizerState::ProcessElementStartTag;
            fStart = elem_start;
        }
        else
        {
            //got a '<' but could not determine the document element
            fState = TokenizerState::Error;
            fStart = elem_start;
        }
    }
    else
    {
        //error, tokenizer was expecting a document element
        //but there was no '<' 
        fState = TokenizerState::Error;
    }
}

void VDPTokenizer::ProcessDocumentEncoding()
{
    
}

void VDPTokenizer::ProcessComment()
{
    size_t elem_start = fString->find(VDPCharacterDefinitions::fDoubleDash, fStart + VDPCharacterDefinitions::fCommentStart.size() );
    if(elem_start != std::string::npos)
    {
        //found a double dash
        if( StartsWithSubstring(elem_start, VDPCharacterDefinitions::fCommentStop) )
        {
            //got a comment end tag 
            fState = TokenizerState::ReadyForNextElement;
            fStart = elem_start + VDPCharacterDefinitions::fCommentStop.size();
        }
        else
        {
            //error, either illegal '--' or nested comment present 
            //may want to loosen this restriction at some point
            fState = TokenizerState::Error;
            return;
        }
    }
    else
    {
        //didn't find the '--' indicator for the comment end tag
        //remain in comment processing mode, but skip to end of string
        fState = TokenizerState::ProcessingComment;
        fStart = fEnd;
    }    
}

void VDPTokenizer::ProcessElementStartTag()
{    
    size_t start_tag_bracket_end = fString->find(VDPCharacterDefinitions::fStartTagEnd, fStart);
    size_t start_tag_space_end = fString->find_first_of(VDPCharacterDefinitions::fWhiteSpace, fStart);

    //assume that start tag spans the entire line (we do not check the characters within for validity)
    if(start_tag_space_end == std::string::npos && start_tag_bracket_end == std::string::npos)
    {
        VDPToken token;
        token.SetContents(fString->substr(fStart, fEnd - fStart) );
        token.SetTokenType(VDPTokenType::OpenStartTagToken);
        fTokens.push_back(token);
        fStart = fEnd;
        fState = TokenizerState::ProcessAttributeAssignment;
        return;
    }

    //start tag ends with a space
    if(start_tag_space_end < start_tag_bracket_end)
    {
        /* we have element start tag delimited by a space that looks like:
        
        <element_name attribute="1"/>

        or

        <element_name 
            attribute="1"
        />

        */
        VDPToken token;
        token.SetContents(fString->substr(fStart, (start_tag_space_end-fStart)) );
        token.SetTokenType(VDPTokenType::OpenStartTagToken);
        fTokens.push_back(token);
        fStart = start_tag_space_end + VDPCharacterDefinitions::fSpace.size();
        fState = TokenizerState::ProcessAttributeAssignment;
        return;
    }

    //start tag ends with a bracket
    if(start_tag_bracket_end != std::string::npos)
    {
        VDPToken token;
        token.SetContents(fString->substr(fStart, ( (start_tag_bracket_end + VDPCharacterDefinitions::fStartTagEnd.size()) - fStart) ) );
        token.SetTokenType(VDPTokenType::ClosedStartTagToken);
        fTokens.push_back(token);
        fStart = start_tag_bracket_end + VDPCharacterDefinitions::fStartTagEnd.size();
        fState = TokenizerState::ProcessElementContents;
        return;
    }

    //unknown state
    fState = TokenizerState::Error;
}

void VDPTokenizer::ProcessElementStopTag()
{
    
}

void VDPTokenizer::ProcessElementContents()
{
    
}

void VDPTokenizer::ProcessAttributeAssignment()
{
    //generate primitive tokens delimited by white space
    std::vector< std::string > tokens = TokenizeOnDelimiter( fString, VDPCharacterDefinitions::fWhiteSpace, fStart, fEnd);

    //loop over attribute tokens (which must contain a '=' character)
    for(size_t i=0; i<tokens.size(); i++)
    {
        size_t eq_location = tokens[i].find(VDPCharacterDefinitions::fEqualSign)
        if(eq_location != std::string::npos)
        {
            //check if it contains the closing tag: '/>'
            size_t close_location = tokens[i].find(VDPCharacterDefinitions::fStartTagStop)
            if(close_location != std::string::npos)
            {
                //just an ordinary attribute token
                //now split the simple token (at the equal sign) into attribute name and attribute value tokens
                std::string name_token = tokens[i].substr(0, eq_location);
                std::string value_token = tokens[i].substr(eq_location + VDPCharacterDefinitions::fEqualSign.size(), tokens[i].size() - (eq_location + VDPCharacterDefinitions::fEqualSign.size() ) );
                
                //strip leading and trailing quotes from the value token

            
                if(name_token.size() != 0 and value_token.size() != 0)
                {

                }

            }
            else
            {
                
            }
        }
        else
        {
            //not an attribute assignment token, better be a closing tag: '/>'
            if(token[i].StartsWithSubstring())
        }
    }


if( NextCharacterIsAny(elem_start, VDPCharacterDefinitions::fValidNameStartCharacterList) )


}

bool VDPTokenizer::StartsWithSubstring(size_t position, const std::string& substring)
{
    size_t location = fString->find(substring, position);
    return location == position;
}

bool VDPTokenizer::NextCharacterIsAny(size_t position, const std::string& character_list)
{
    if(position + 1 < fString->size())
    {
        size_t location = character_list.find( fString->at(position+1) );
        
        if (location != std::string::npos)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


std::vector< std::string > 
VDPTokenizer::TokenizeOnDelimiter(const std::string& content, const std::string& delim, size_t aStart, size_t anEnd);
{
    std::vector< std::string > simple_tokens;
    simple_tokens.clear();
    
    //we tokenize the contents from the start position to end
    //base on the (collection of) delimiters we are given

    size_t start = aStart;
    size_t end = 0;
    size_t length = 0;
    while( end != anEnd && start < anEnd)
    {
        end = content.find_first_of(delim, start);

        if(end == std::string::npos)
        {
            end = anEnd;
        }

        length = end - start;

        //make sure no empty tokens (double spaces, etc.) are included
        if( length > 0  )
        {
            simple_tokens.push_back( content.substr(start,length) );
        }
        start = end + delim.size();
    }
}




} //end of namespace vdp
