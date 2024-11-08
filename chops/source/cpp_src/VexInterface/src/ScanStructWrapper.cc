#include "ScanStructWrapper.hh"
#include "DateStructWrapper.hh"
#include "SourceStructWrapper.hh"
#include "StationStructWrapper.hh"

#include <iostream>

namespace vdp{


    // struct scan_struct
    //     {
    //     char                        filename[256];  /* Name of input vex file */
    //     short                       exper_num;      /* Standard 4-digit */
    //     char                        exper_name[32];
    //     char                        correlator[32];
    //     char                        scan_name[32];
    //     struct date                 start_time;     /* Standard Mk4 struct */
    //     struct date                 ffit_reftime;   /* Standard Mk4 struct */
    //     float                       tai_utc;        /* EOP parameters (global) */
    //     float                       a1_tai;
    //     int                         neop;           /* Number of eop entries */
    //     struct date                 eop_reftime;    /* Time of 1st entry */
    //     int                         eop_interval;   /* Seconds */
    //     float                       ut1_utc[10];    /* Seconds */
    //     float                       x_wobble[10];   /* Radians */
    //     float                       y_wobble[10];   /* Radians */
    //     struct source_struct        src;
    //     short                       nst;            /* Number of st elements */
    //     struct station_struct       *st;            /* Allocated */
    //     };

void
ScanStructWrapper::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {
        //TODO need to check values of elements for 'UNDEFINED' entries, and ignore those which have not been set/read-in
        node->AddChild(new VDPStructMemberPODArrayOutputNode< scan_struct, char, char[256], &scan_struct::filename >( std::string("filename"), 256, &(this->fScanStruct) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< scan_struct, short, &scan_struct::exper_num >( std::string("exper_num"), &(this->fScanStruct) ) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< scan_struct, char, char[32], &scan_struct::exper_name >( std::string("exper_name"), 32, &(this->fScanStruct) ) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< scan_struct, char, char[32], &scan_struct::correlator >( std::string("correlator"), 32, &(this->fScanStruct) ) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< scan_struct, char, char[32], &scan_struct::scan_name >( std::string("scan_name"), 32, &(this->fScanStruct) ) );
        node->AddChild(new VDPObjectOutputNode< DateStructWrapper >( std::string("start_time"), new DateStructWrapper(this->fScanStruct.start_time) ) );
        node->AddChild(new VDPObjectOutputNode< DateStructWrapper >( std::string("ffit_reftime"), new DateStructWrapper(this->fScanStruct.ffit_reftime) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< scan_struct, float, &scan_struct::tai_utc >( std::string("tai_utc"), &(this->fScanStruct)) );
        node->AddChild(new VDPStructMemberPODOutputNode< scan_struct, float, &scan_struct::a1_tai >( std::string("a1_tai"),  &(this->fScanStruct) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< scan_struct, int, &scan_struct::neop >( std::string("neop"),  &(this->fScanStruct) ) );
        node->AddChild(new VDPObjectOutputNode< DateStructWrapper >( std::string("eop_reftime"), new DateStructWrapper(this->fScanStruct.eop_reftime) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< scan_struct, int, &scan_struct::eop_interval >( std::string("eop_interval"),  &(this->fScanStruct) ) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< scan_struct, float, float[10], &scan_struct::ut1_utc >( std::string("ut1_utc"), 10, &(this->fScanStruct) ) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< scan_struct, float, float[10], &scan_struct::x_wobble >( std::string("x_wobble"), 10, &(this->fScanStruct) ) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< scan_struct, float, float[10], &scan_struct::y_wobble >( std::string("y_wobble"), 10, &(this->fScanStruct) ) );

        node->AddChild(new VDPObjectOutputNode< SourceStructWrapper >( std::string("src"), new SourceStructWrapper(this->fScanStruct.src) ) );

        node->AddChild(new VDPStructMemberPODOutputNode< scan_struct, short, &scan_struct::nst >( std::string("nst"),  &(this->fScanStruct) ) );

        //skipping station_struct
        for( int i=0; i<this->fScanStruct.nst; i++)
        {
            node->AddChild(new VDPObjectOutputNode< StationStructWrapper >( std::string("station_struct"), new StationStructWrapper(this->fScanStruct.st[i]) ) );
        }

    }
}

void
ScanStructWrapper::DefineInputNode(VDPInputNode* node)
{
    //THIS DOES NOT YET WORK
    if(node != NULL)
    {
        // node->AddChild(new VDPStructMemberPODInputNode< scan_struct, short, &scan_struct::exper_num >( std::string("exper_num"), &(this->fScanStruct) ) );
        // node->AddChild(new VDPStructMemberPODInputNode< scan_struct, float, &scan_struct::tai_utc >( std::string("tai_utc"), &(this->fScanStruct)) );
        // node->AddChild(new VDPStructMemberPODInputNode< scan_struct, float, &scan_struct::a1_tai >( std::string("a1_tai"),  &(this->fScanStruct) ) );
        // node->AddChild(new VDPStructMemberPODInputNode< scan_struct, int, &scan_struct::eop_interval >( std::string("eop_interval"),  &(this->fScanStruct) ) );
    }
}



}
