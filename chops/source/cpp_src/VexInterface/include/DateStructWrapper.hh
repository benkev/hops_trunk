// typedef struct date
//     {
//     short	year;
//     short	day;
//     short	hour;
//     short	minute;
//     float	second;
//     } date_struct;

#ifndef DateStructWrapper_HH__
#define DateStructWrapper_HH__

#include "VDPStructuredASCIIHeaders.hh"

extern "C"
{
#include "mk4_typedefs.h"
}

/*
*File: DateStructWrapper.hh
*Class: DateStructWrapper
*Author: J. Barrett
*Email: barrettj@mit.edu
*Date: Mon Jun 12 10:16:15 EDT 2017
*Description:
*/


namespace vdp
{

class DateStructWrapper: public VDPInputOutputObject
{
    public:

        DateStructWrapper(date_struct aDate):VDPInputOutputObject()
        {
            fDate = aDate;
        };

        DateStructWrapper():VDPInputOutputObject(){};

        virtual ~DateStructWrapper(){};

        DateStructWrapper(const DateStructWrapper& copyObject):
        VDPInputOutputObject()
        {
            fDate = copyObject.fDate;
        };

        DateStructWrapper& operator=(const DateStructWrapper& rhs)
        {
            if(&rhs != this)
            {
                fDate = rhs.fDate;
            }
            return *this;
        };

        virtual const char* GetName() const {return "date_struct";};

        void DefineOutputNode(VDPOutputNode* node) const;
        void DefineInputNode(VDPInputNode* node);

        virtual const char* ClassName() const { return "date_struct"; };

    public:

        date_struct fDate;

};

DefineVDPClassName( DateStructWrapper )

}

#endif /* end of include guard: DateStructWrapper */
