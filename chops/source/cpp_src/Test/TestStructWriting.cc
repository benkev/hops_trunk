extern "C"
{
    #include "vex.h"
    #include "mk4_vex.h"
}

extern int msglev = 1;
extern char progname[] = "TestStructWriting";



#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <limits>
#include <complex>
#include <cstdlib>
#include <iomanip>
#include <vector>


#include "VDPTestE.hh"

//#include "VDPObjectOutputNode.hh"
#include "VDPFixedSizeInputOutputObject.hh"

#include "VDPStructuredASCIIHeaders.hh"

#include "VDPFileWriter.hh"
#include "VDPFileReader.hh"

#include "VDPOutputCollector.hh"
#include "VDPInputCollector.hh"



using namespace vdp;



int main()
{

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////


    std::stringstream s;
    s << "./testVDP_struct_orig.xml";
    std::string outfile = s.str();
    s.clear();s.str("");
    s <<  "./testVDP_struct_copy.xml";
    std::string outfile2 = s.str();

    VDPFileWriter writer;

    VDPOutputCollector collector;

    collector.SetUseTabbingTrue();

    writer.SetFileName(outfile);
//    writer.SetModeRecreate();
//    writer.IncludeXMLGuards();

    VDPTestE e_obj;

    e_obj.fE.ivalue = 1;
    e_obj.fE.dvalue = 1.23456;
    e_obj.fE.fvalue = 9.9;
    e_obj.fE.svalue = "a_string";

    VDPOutputNode* root = new VDPOutputNode(std::string("root"));
    VDPObjectOutputNode< VDPTestE  >* test_e_node = new VDPObjectOutputNode< VDPTestE  >("TestE");

    if( writer.Open() )
    {
        test_e_node->AttachObjectToNode(&e_obj);

        root->AddChild(test_e_node);

        collector.SetFileWriter(&writer);
        collector.CollectOutput(root);

        writer.Close();
    }
    else
    {
        std::cout<<"Could not open file"<<std::endl;
    }


    delete root;

    std::cout<<"closing file"<<std::endl;

    VDPFileReader reader;
    reader.SetFileName(outfile);

    VDPInputCollector* in_collector = new VDPInputCollector();
    in_collector->SetFileReader(&reader);

    VDPInputNode* input_root = new VDPInputNode(std::string("root"));
    VDPObjectInputNode< VDPTestE >* input_e = new VDPObjectInputNode< VDPTestE >(std::string("TestE"));
    input_root->AddChild(input_e);

    std::cout<<"reading file"<<std::endl;
    if( reader.Open() )
    {
        in_collector->ForwardInput(input_root);
    }
    else
    {
        std::cout<<"Could not open file"<<std::endl;
    }

    VDPOutputCollector collector2;
    collector2.SetUseTabbingTrue();
    VDPFileWriter writer2;
    writer2.SetFileName(outfile2);
//    writer2.SetModeRecreate();
//    writer2.IncludeXMLGuards();

    VDPOutputNode* root2 = new VDPOutputNode(std::string("root"));
    VDPObjectOutputNode< VDPTestE >* copy_e = new VDPObjectOutputNode< VDPTestE >("TestE");
    copy_e->AttachObjectToNode(input_e->GetObject());
    root2->AddChild(copy_e);

    if( writer2.Open() )
    {
        collector2.SetFileWriter(&writer2);
        collector2.CollectOutput(root2);

        writer2.Close();

    }
    else
    {
        std::cout<<"Could not open file"<<std::endl;
    }
    std::cout<<"done"<<std::endl;

    delete root2;

    return 0;




}
