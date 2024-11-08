#ifndef VDPFileReader_HH__
#define VDPFileReader_HH__


#include <string>
#include <fstream>

#include "VDPCharacterDefinitions.hh"

namespace vdp{

//performs opening, reading, closing of file
//and first level of tokenization, by splitting on newlines

class VDPFileReader
{
    public:
        VDPFileReader();
        virtual ~VDPFileReader();

        void SetFileName(std::string filename);
        bool Open();
        void Close();

        bool GetLine(std::string& line);

    protected:

        std::string StripWhiteSpace();

        bool fIsOpen;
        bool fIsFileNameValid;
        std::string fFileName;
        std::string fLine;
        std::ifstream fFileStream;
};


}//end of vdp namespace

#endif /* VDPFileReader_H__ */
