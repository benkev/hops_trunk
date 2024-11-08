#ifndef ScanStructWrapper_HH__
#define ScanStructWrapper_HH__

#include "VDPStructuredASCIIHeaders.hh"

extern "C"
{
#include "ovex.h"
}


namespace vdp
{

class ScanStructWrapper: public VDPInputOutputObject
{
    public:

        ScanStructWrapper(scan_struct aScan):VDPInputOutputObject()
        {
            fScanStruct = aScan;
            fScanStruct.st = new station_struct[aScan.nst];
            for(int i=0; i<fScanStruct.nst; i++)
            {
                fScanStruct.st[i] = aScan.st[i];
            }
            fIsOwned = true;
        };

        ScanStructWrapper():fIsOwned(false){};

        virtual ~ScanStructWrapper()
        {
            if(fIsOwned)
            {
                    delete[] fScanStruct.st;
            }
        };

        ScanStructWrapper(const ScanStructWrapper& copyObject):
        VDPInputOutputObject()
        {
            fScanStruct = copyObject.fScanStruct;
            fScanStruct.st = new station_struct[fScanStruct.nst];
            for(int i=0; i<fScanStruct.nst; i++)
            {
                fScanStruct.st[i] = copyObject.fScanStruct.st[i];
            }
            fIsOwned = true;
        };

        ScanStructWrapper& operator=(const ScanStructWrapper& rhs)
        {
            if(&rhs != this)
            {
                fScanStruct = rhs.fScanStruct;
                fScanStruct.st = new station_struct[fScanStruct.nst];
                for(int i=0; i<fScanStruct.nst; i++)
                {
                    fScanStruct.st[i] = rhs.fScanStruct.st[i];
                }
            }
            return *this;
            fIsOwned = true;
        };

        virtual const char* GetName() const {return "scan_struct";};

        void DefineOutputNode(VDPOutputNode* node) const;

        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "scan_struct"; };

    public:

        bool fIsOwned;
        scan_struct fScanStruct;

};

DefineVDPClassName( ScanStructWrapper )


}

#endif /* end of include guard: ScanStructWrapper_HH__ */
