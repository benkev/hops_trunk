#ifndef VDPDefinitions_HH__
#define VDPDefinitions_HH__

namespace vdp{

/**
*
*@file VDPDefinitions.hh
*@class VDPDefinitions
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Dec 15 01:16:51 EST 2012 J. Barrett (barrettj@mit.edu) First Version
*
*/

#define CHUNK 4194304
#define WRITE_CHUNK CHUNK
#define READ_CHUNK CHUNK

#define START_TAG_BEGIN "<"
#define START_TAG_END ">"
#define START_TAG_STOP "/>"
#define STOP_TAG_BEGIN "</"
#define STOP_TAG_END ">"
#define LINE_DELIM "\n"
#define LINE_PREFIX "\t"
#define ELEM_DELIM " "
#define XML_ENCODING "<?xml version=\"1.0\" encoding=\"ASCII\" standalone=\"yes\"?>"

}//end of vdp namespace

#endif /* __VDPDefinitions_H__ */
