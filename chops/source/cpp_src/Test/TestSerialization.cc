extern "C"
{
    #include "vex.h"
    #include "mk4_vex.h"
}

extern int msglev = 1;
extern char progname[] = "TestSerialization";



#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <limits>
#include <complex>
#include <cstdlib>
#include <iomanip>
#include <vector>


#include "VDPTestA.hh"
#include "VDPTestB.hh"
#include "VDPTestC.hh"
#include "VDPTestD.hh"

//#include "VDPObjectOutputNode.hh"
#include "VDPFixedSizeInputOutputObject.hh"

#include "VDPStructuredASCIIHeaders.hh"

#include "VDPFileWriter.hh"
#include "VDPFileReader.hh"

#include "VDPOutputCollector.hh"
#include "VDPInputCollector.hh"


using namespace vdp;

double
UniformRandom(double lower_limit, double upper_limit)
{
    double r = 0;
    //we don't need high quality random numbers here, so we use rand()
    double m = RAND_MAX;
    m += 1;// do not want the range to be inclusive of the upper limit
    double r1 = rand();
    r = r1/m;
    return lower_limit + (upper_limit - lower_limit)*r;
}

int main()
{
    //the the specialization:

    std::cout<<"is VDPTestB derived from fixed size obj?  = " << std::endl;
    unsigned int test = VDPIsDerivedFrom< VDPTestB, VDPFixedSizeInputOutputObject >::Is;
    std::cout<<"test = "<<test<<std::endl;

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////


    std::stringstream s;
    s << "./testVDP_orig.xml";
    std::string outfile = s.str();
    s.clear();s.str("");
    s <<  "./testVDP_copy.xml";
    std::string outfile2 = s.str();

    VDPFileWriter writer;

    VDPOutputCollector collector;

    collector.SetUseTabbingTrue();

    writer.SetFileName(outfile);
//    writer.SetModeRecreate();
//    writer.IncludeXMLGuards();


    VDPOutputNode* root = new VDPOutputNode(std::string("root"));
    VDPObjectOutputNode< std::vector< VDPTestC >  >* test_c_vec_node = new VDPObjectOutputNode< std::vector< VDPTestC >  >("TestCVector");

    if( writer.Open() )
    {
        int n = 5; //number of objects
        int nv = 8; //number of doubles in vector

        std::vector<VDPTestC>* C_vec = new std::vector<VDPTestC>();

        VDPTestC C_obj;
        VDPTestB B_obj;
        VDPTestD D_obj;
        double temp[3] = {1.,2.,3.};

        std::vector<VDPTestB*> BVec;

        for(int i=0; i<n; i++)
        {
            C_obj.ClearData();
            C_obj.ClearBVector();

            for(int j=0; j<nv; j++)
            {
                C_obj.AddData(UniformRandom(0,1)*1e-15);
            }


            B_obj.SetX(UniformRandom(0,1));
            B_obj.SetY(UniformRandom(0,1));
            D_obj.SetX(UniformRandom(0,1));
            D_obj.SetY(UniformRandom(0,1));
            D_obj.SetD(UniformRandom(0,1));
            temp[0] = UniformRandom(0,1);
            temp[1] = UniformRandom(0,1);
            temp[2] = UniformRandom(0,1);

            B_obj.SetArray(temp);
            D_obj.SetArray(temp);

            C_obj.SetB(B_obj);

            BVec.clear();
            B_obj.SetX(UniformRandom(0,1));
            BVec.push_back(new VDPTestB(B_obj));
            D_obj.SetD(UniformRandom(0,1));
            BVec.push_back(new VDPTestD(D_obj));

            C_obj.AddBVector(&BVec);

            BVec.clear();
            B_obj.SetY(UniformRandom(0,1));
            BVec.push_back(new VDPTestB(B_obj));
            B_obj.SetY(UniformRandom(0,1));
            BVec.push_back(new VDPTestD(D_obj));
            B_obj.SetY(UniformRandom(0,1));
            BVec.push_back(new VDPTestB(B_obj));

            C_obj.AddBVector(&BVec);

            C_obj.SetCData(UniformRandom(0,1));

            C_vec->push_back(C_obj);
        }


        test_c_vec_node->AttachObjectToNode(C_vec);

        root->AddChild(test_c_vec_node);

        collector.SetFileWriter(&writer);
        collector.CollectOutput(root);

        writer.Close();

        delete C_vec;

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
    VDPObjectInputNode< std::vector< VDPTestC > >* input_c_vec = new VDPObjectInputNode< std::vector< VDPTestC > >(std::string("TestCVector"));
    input_root->AddChild(input_c_vec);

    std::cout<<"reading file"<<std::endl;
    if( reader.Open() )
    {
        in_collector->ForwardInput(input_root);
    }
    else
    {
        std::cout<<"Could not open file"<<std::endl;
    }

    std::cout<<"vector size = "<<input_c_vec->GetObject()->size()<<std::endl;

    VDPOutputCollector collector2;
    collector2.SetUseTabbingTrue();
    VDPFileWriter writer2;
    writer2.SetFileName(outfile2);
//    writer2.SetModeRecreate();
//    writer2.IncludeXMLGuards();

    VDPOutputNode* root2 = new VDPOutputNode(std::string("root"));
    VDPObjectOutputNode< std::vector< VDPTestC >  >* copy_c_vec = new VDPObjectOutputNode< std::vector< VDPTestC >  >("TestCVector");
    copy_c_vec->AttachObjectToNode(input_c_vec->GetObject());
    root2->AddChild(copy_c_vec);

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
