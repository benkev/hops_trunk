#ifndef SourceStructWrapper_HH__
#define SourceStructWrapper_HH__

#include "VDPStructuredASCIIHeaders.hh"

extern "C"
{
#include "ovex.h"
}


namespace vdp
{

class SourceStructWrapper: public VDPInputOutputObject
{
    public:

        SourceStructWrapper(source_struct aSource):VDPInputOutputObject()
        {
            fSourceStruct = aSource;
        };

        SourceStructWrapper():VDPInputOutputObject(){};

        virtual ~SourceStructWrapper(){};

        SourceStructWrapper(const SourceStructWrapper& copyObject):
        VDPInputOutputObject()
        {
            fSourceStruct = copyObject.fSourceStruct;
        };

        SourceStructWrapper& operator=(const SourceStructWrapper& rhs)
        {
            if(&rhs != this)
            {
                fSourceStruct = rhs.fSourceStruct;
            }
            return *this;
        };

        virtual const char* GetName() const {return "source_struct";};

        void DefineOutputNode(VDPOutputNode* node) const;

        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "source_struct"; };

    public:

        source_struct fSourceStruct;

};

DefineVDPClassName( SourceStructWrapper )


}

#endif /* end of include guard: SourceStructWrapper_HH__ */
