#include "VDPFileReader.hh"

#include <iostream>
#include <assert.h>

namespace vdp{


VDPFileReader::VDPFileReader():fIsOpen(false),fIsFileNameValid(false)
{

}

VDPFileReader::~VDPFileReader()
{

}

void VDPFileReader::SetFileName(std::string filename)
{
    fFileName = filename;

    //check if filename has the correct extension
    if(std::string::npos != filename.find(std::string(".xml")))
    {
        fIsFileNameValid = true;
    }
}


bool
VDPFileReader::Open()
{
    if(fIsOpen)
    {
        fFileStream.close();
    }

    fIsOpen = false;
    if(fIsFileNameValid)
    {
        fFileStream.open(fFileName.c_str(), std::ifstream::in);
        fIsOpen = fFileStream.is_open();
    }
    
    return fIsOpen;
}


bool
VDPFileReader::GetLine(std::string& line)
{
    if(fFileStream.good())
    {
        std::getline(fFileStream, fLine); //get the line

        //strip leading and trailing whitespace
        line = StripWhiteSpace();
        return true;
    }

    line = "";
    return false;
}

void VDPFileReader::Close()
{
    if(fIsOpen)
    {
        fFileStream.close();
    }
    fIsOpen = false;
}

std::string VDPFileReader::StripWhiteSpace()
{
    size_t begin;
    size_t end;
    size_t len;

    begin = fLine.find_first_not_of(VDPCharacterDefinitions::fWhiteSpace);

    if(begin != std::string::npos)
    {
        end = fLine.find_last_not_of(VDPCharacterDefinitions::fWhiteSpace);

        len = end - begin + 1;

        return fLine.substr(begin, len);
    }

    //empty string
    return "";
}




}//end of vdp namespace
