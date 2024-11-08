#include "VDPFileWriter.hh"

#include <iostream>
#include <assert.h>

#include "VDPDefinitions.hh"

namespace vdp{

VDPFileWriter::VDPFileWriter():
fIsOpen(false),
fIsFileNameValid(false),
fWriteEncoding(false)
{

}


VDPFileWriter::~VDPFileWriter()
{

}

void VDPFileWriter::SetFileName(std::string filename)
{
    fFileName = filename;

    //check if filename has the correct extension
    if(std::string::npos != filename.find(std::string(".xml")))
    {
        fIsFileNameValid = true;
    }
}


bool
VDPFileWriter::Open()
{
    if(fIsOpen)
    {
        fFileStream.close();
    }
    fIsOpen = false;

    //only allowed mode is recreate/truncate
    if(fIsFileNameValid)
    {
        fFileStream.open(fFileName.c_str(), std::ofstream::trunc);
        fIsOpen = fFileStream.is_open();

        if(fIsOpen)
        {
            if(fWriteEncoding)
            {
                //immediately dump xml encoding
                fFileStream << XML_ENCODING << "\n"; 
            }
        }

    }

    return fIsOpen;
}

void
VDPFileWriter::AddToFile(const std::string& data)
{
    if(fFileStream.good())
    {
        fFileStream << data;
    }
}

void
VDPFileWriter::Close()
{
    if(fIsOpen)
    {
        fFileStream.flush();
        fFileStream.close();
    }
    fIsOpen = false;
}




}
