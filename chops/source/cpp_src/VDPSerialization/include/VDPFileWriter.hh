#ifndef VDPFileWriter_HH__
#define VDPFileWriter_HH__

#include <string>
#include <fstream>

namespace vdp{

class VDPFileWriter
{
    public:
        VDPFileWriter();
        virtual ~VDPFileWriter();

        //must be called before opening the file stream
        void SetFileName(std::string filename);
        void IncludeXMLEncoding(){fWriteEncoding = true;}; //if called, will write encoding line


        bool Open();
        void AddToFile(const std::string& data);
        void FlushStream() { fFileStream.flush(); }
        void Close();

        std::string GetFileName() const { return fFileName; }

    protected:

        bool fIsOpen;
        bool fIsFileNameValid;
        bool fWriteEncoding;

        std::string fFileName;
        std::ofstream fFileStream;
};


}//end of vdp namespace

#endif /* VDPFileWriter_H__ */
