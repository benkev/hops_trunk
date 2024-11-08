#ifndef VDPTokenizer_HH__
#define VDPTokenizer_HH__


#include "VDPDefinitions.hh"
#include <vector>
#include <string>

namespace vdp{

/**
*
*@file VDPTokenizer.hh
*@class VDPTokenizer
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Thu Dec 13 19:47:22 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/


class VDPTokenizer
{
    public:
        VDPTokenizer()
        {
            fDelim = " "; //default delim is space
            fString = NULL;
            fIncludeEmptyTokens = false;
        };
        virtual ~VDPTokenizer(){;};

        void SetIncludeEmptyTokensTrue(){fIncludeEmptyTokens = true;};
        void SetIncludeEmptyTokensFalse(){fIncludeEmptyTokens = false;};

        void SetString(const std::string* aString){fString = aString;};
        void SetDelimiter(const std::string& aDelim){fDelim = aDelim;};
        void GetTokens(std::vector< std::string>* tokens) const
        {
            if(tokens != NULL && fString != NULL)
            {
                tokens->clear();
                if(fDelim.size() > 0)
                {

                    size_t start = 0;
                    size_t end = 0;
                    size_t length = 0;
                    while( end != std::string::npos )
                    {
                        end = fString->find(fDelim, start);

                        if(end == std::string::npos)
                        {
                            length = std::string::npos;
                        }
                        else
                        {
                            length = end - start;
                        }


                        if( fIncludeEmptyTokens || ( (length > 0 ) && ( start < fString->size() ) ) )
                        {
                            tokens->push_back( fString->substr(start,length) );
                        }

                        if( end > std::string::npos - fDelim.size() )
                        {
                            start = std::string::npos;
                        }
                        else
                        {
                            start = end + fDelim.size();
                        }
                    }
                }
            }
        }


    protected:

        bool fIncludeEmptyTokens;
        std::string fDelim;
        const std::string* fString;

};

}//end of vdp namespace

#endif /* __VDPTokenizer_H__ */
