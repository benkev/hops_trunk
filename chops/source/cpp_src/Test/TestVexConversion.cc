#include <string>
#include <iostream>

extern "C"
{
    #include "vex.h"
    #include "mk4_vex.h"
}

extern int msglev = 1;
extern char progname[] = "TestVexConversion";

#include "ScanStructWrapper.hh"

using namespace vdp;

int main(int argc, char** argv)
{
    struct vex root;

    std::string vexfile(argv[1]);

    std::string outfile = vexfile + std::string(".xml");

    int ret_val = parse_vexfile( const_cast<char *>(vexfile.c_str()) );

    std::cout<<"parse_vexfile return value = "<<ret_val<<std::endl;

    if (get_vex (const_cast<char *>(vexfile.c_str()), OVEX | EVEX | IVEX | LVEX, "", &root) != 0)
    {
        std::cout<<"Error reading root file: "<<vexfile<<"."<<std::endl;

    }

    std::cout<<"ovex exp num = "<<root.ovex->exper_num<<std::endl;

    std::cout<<"ovex exp name = "<<root.ovex->exper_name<<std::endl;

    std::cout<<"acc period = "<<root.evex->ap_length<<std::endl;

    std::cout<<"speed up factor = "<<root.evex->speedup_factor<<std::endl;

    std::cout<<"n stations = "<<root.ovex->nst<<std::endl;

    std::cout<<"station 1 = "<<(root.ovex->st)[0].site_name<<std::endl;

    ScanStructWrapper ovexWrapper;
    //does not work in full
    ovexWrapper.fScanStruct = *(root.ovex);


    // std::stringstream s;
    // s << "./testVDP_ovex_orig.xml";
    // std::string outfile = s.str();
    // s.clear();s.str("");
    // s <<  "./testVDP_struct_copy.xml";
    // std::string outfile2 = s.str();

    VDPFileWriter writer;

    VDPOutputCollector collector;

    collector.SetUseTabbingTrue();

    writer.SetFileName(outfile);
//    writer.SetModeRecreate();
    writer.IncludeXMLEncoding();

    VDPOutputNode* root_node = new VDPOutputNode(std::string("root"));
    VDPObjectOutputNode< ScanStructWrapper  >* test_ovex_node = new VDPObjectOutputNode< ScanStructWrapper >("ovex");

    if( writer.Open() )
    {
        test_ovex_node->AttachObjectToNode(&ovexWrapper);

        root_node->AddChild(test_ovex_node);

        collector.SetFileWriter(&writer);
        collector.CollectOutput(root_node);

        writer.Close();
    }
    else
    {
        std::cout<<"Could not open file"<<std::endl;
    }


    delete root_node;


    return 0;
}
