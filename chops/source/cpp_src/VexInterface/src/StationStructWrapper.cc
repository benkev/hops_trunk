#include "StationStructWrapper.hh"
#include "DateStructWrapper.hh"
#include "ChanStructWrapper.hh"

// struct station_struct
//     {
//     short               start_offset;           /* Seconds */
//     short               stop_offset;            /* Seconds */
//     float               start_tapepos;          /* Meters */
//     short               tape_motion;            /* Defines above */
//     short               early_start;            /* Seconds */
//     short               late_finish;            /* Seconds */
//     short               tape_gap;               /* Seconds */
//     char                subpass;                /* Standard vex meaning */
//     short               passno;                 /* Standard vex meaning */
//     short               drive_no;
//     short               site_type;              /* Defines above */
//     char                site_name[9];
//     char                site_id[3];             /* International 2 char code */
//     char                mk4_site_id;            /* 1-char correlator alias */
//     double              coordinates[3];         /* Meters */
//     struct date         coordinate_epoch;       /* Standard Mk4 struct */
//     double              site_velocity[3];       /* Meters/sec */
//     float               zenith_atm;             /* Seconds */
//     char                occucode[5];            /* Standard 4-char code */
//     short               axis_type;              /* Defines above */
//     float               axis_offset;            /* Meters */
//     short               recorder_type;          /* Defines above */
//     short               rack_type;              /* Defines above */
//     float               record_density;         /* Bits/inch */
//     float               tape_length;            /* Meters */
//     short               recorder_id;            /* Unique integer */
//     float               clock_early;            /* Seconds */
//     struct date         clockrate_epoch;        /* Standard Mk4 struct */
//     float               clockrate;              /* sec/sec */
//     char                tape_id[9];             /* Standard tape label */
//     double              samplerate;             /* Samples/sec */
//     short               track_format;           /* Defines above */
//     short               modulation;             /* Defines above */
//     short               bits_sample;            /* 1 or 2 */
//     short               multiplex_ratio;        /* 1, 2 or 4 */
//     char                pass_direction;         /* F or R */
//     float               head_position[4];       /* Meters */
//     short               roll;                   /* Defines above */
//     short               roll_increment;         /* Frames */
//     float               roll_period;            /* Seconds */
//     short               roll_seq[5][34][32];    /* Track numbers */
//     struct chan_struct  channels[MAX_CHAN];
//     };


namespace vdp
{

void
StationStructWrapper::DefineOutputNode(VDPOutputNode* node) const
{
    if(node != NULL)
    {

        //     short               start_offset;           /* Seconds */
        //     short               stop_offset;            /* Seconds */
        //     float               start_tapepos;          /* Meters */
        //     short               tape_motion;            /* Defines above */
        //     short               early_start;            /* Seconds */
        //     short               late_finish;            /* Seconds */
        //     short               tape_gap;               /* Seconds */

        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::start_offset >( std::string("start_offset"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::stop_offset >( std::string("stop_offset"), &(this->fStation)) );

        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, float, &station_struct::start_tapepos >( std::string("start_tapepos"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::tape_motion >( std::string("tape_motion"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::early_start >( std::string("early_start"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::late_finish >( std::string("late_finish"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::tape_gap >( std::string("tape_gap"), &(this->fStation)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("start_tapepos") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("tape_motion") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("early_start") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("late_finish") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("tape_gap") ) );
        #endif
        #endif

        //     char                subpass;                /* Standard vex meaning */
        //     short               passno;                 /* Standard vex meaning */


        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, char, &station_struct::subpass >( std::string("subpass"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::passno >( std::string("passno"), &(this->fStation)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("subpass") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("passno") ) );
        #endif
        #endif

        //     short               drive_no;
        //     short               site_type;              /* Defines above */
        //     char                site_name[9];
        //     char                site_id[3];             /* International 2 char code */
        //     char                mk4_site_id;            /* 1-char correlator alias */

        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::drive_no >( std::string("drive_no"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::site_type >( std::string("site_type"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< station_struct, char, char[9], &station_struct::site_name >( std::string("site_name"), 9, &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< station_struct, char, char[3], &station_struct::site_id >( std::string("site_id"), 3, &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, char, &station_struct::mk4_site_id >( std::string("mk4_site_id"), &(this->fStation)) );

        //     double              coordinates[3];         /* Meters */
        //     struct date         coordinate_epoch;       /* Standard Mk4 struct */
        //     double              site_velocity[3];       /* Meters/sec */
        //     float               zenith_atm;             /* Seconds */
        //     char                occucode[5];            /* Standard 4-char code */
        //     short               axis_type;              /* Defines above */
        //     float               axis_offset;            /* Meters */
        //     short               recorder_type;          /* Defines above */
        //     short               rack_type;              /* Defines above */

        node->AddChild(new VDPStructMemberPODArrayOutputNode< station_struct, double, double[3], &station_struct::coordinates >( std::string("coordinates"), 3, &(this->fStation)) );

        node->AddChild(new VDPObjectOutputNode< DateStructWrapper >( std::string("coordinate_epoch"), new DateStructWrapper(this->fStation.coordinate_epoch) ) );
        // #ifndef DEACTIVATE_DEPRECATED_DATA
        // node->AddChild(new VDPObjectOutputNode< DateStructWrapper >( std::string("coordinate_epoch"), new DateStructWrapper(this->fStation.coordinate_epoch) ) );
        // #else
        // #ifdef ACTIVATE_EMPTY_ELEMENTS
        // node->AddChild(new VDPEmptyOutputNode( std::string("coordinate_epoch") ) );
        // #endif
        // #endif

        node->AddChild(new VDPStructMemberPODArrayOutputNode< station_struct, double, double[3], &station_struct::site_velocity >( std::string("site_velocity"), 3, &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, float, &station_struct::zenith_atm >( std::string("zenith_atm"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODArrayOutputNode< station_struct, char, char[5], &station_struct::occucode >( std::string("occucode"), 5, &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::axis_type >( std::string("axis_type"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, float, &station_struct::axis_offset >( std::string("axis_offset"), &(this->fStation)) );

        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::recorder_type >( std::string("recorder_type"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::rack_type >( std::string("rack_type"), &(this->fStation)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("recorder_type") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("rack_type") ) );
        #endif
        #endif

        //     float               record_density;         /* Bits/inch */
        //     float               tape_length;            /* Meters */
        //     short               recorder_id;            /* Unique integer */
        //     float               clock_early;            /* Seconds */
        //     struct date         clockrate_epoch;        /* Standard Mk4 struct */
        //     float               clockrate;              /* sec/sec */
        //     char                tape_id[9];             /* Standard tape label */

        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, float, &station_struct::record_density >( std::string("record_density"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, float, &station_struct::tape_length >( std::string("tape_length"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::recorder_id >( std::string("recorder_id"), &(this->fStation)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("record_density") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("tape_length") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("recorder_id") ) );
        #endif
        #endif

        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, float, &station_struct::clock_early >( std::string("clock_early"), &(this->fStation)) );
        node->AddChild(new VDPObjectOutputNode< DateStructWrapper >( std::string("clockrate_epoch"), new DateStructWrapper(this->fStation.clockrate_epoch) ) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, float, &station_struct::clockrate >( std::string("clockrate"), &(this->fStation)) );

        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODArrayOutputNode< station_struct, char, char[9], &station_struct::tape_id >( std::string("tape_id"), 9, &(this->fStation)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("tape_id") ) );
        #endif
        #endif

        //     double              samplerate;             /* Samples/sec */
        //     short               track_format;           /* Defines above */
        //     short               modulation;             /* Defines above */
        //     short               bits_sample;            /* 1 or 2 */
        //     short               multiplex_ratio;        /* 1, 2 or 4 */
        //     char                pass_direction;         /* F or R */

        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, double, &station_struct::samplerate >( std::string("samplerate"), &(this->fStation)) );

        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::track_format >( std::string("track_format"), &(this->fStation)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("track_format") ) );
        #endif
        #endif

        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::modulation >( std::string("modulation"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::bits_sample >( std::string("bits_sample"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::multiplex_ratio >( std::string("multiplex_ratio"), &(this->fStation)) );

        //this data element may contain illegal (null) characters
        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, char, &station_struct::pass_direction >( std::string("pass_direction"), &(this->fStation)) );
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("pass_direction") ) );
        #endif
        #endif

        //     float               head_position[4];       /* Meters */
        //     short               roll;                   /* Defines above */
        //     short               roll_increment;         /* Frames */
        //     float               roll_period;            /* Seconds */
        //     short               roll_seq[5][34][32];    /* Track numbers */

        #ifndef DEACTIVATE_DEPRECATED_DATA
        node->AddChild(new VDPStructMemberPODArrayOutputNode< station_struct, float, float[4], &station_struct::head_position >( std::string("head_position"), 4, &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::roll >( std::string("roll"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, short, &station_struct::roll_increment >( std::string("roll_increment"), &(this->fStation)) );
        node->AddChild(new VDPStructMemberPODOutputNode< station_struct, float, &station_struct::roll_period >( std::string("roll_period"), &(this->fStation)) );
        //no handling for multi-dim arrays yet, roll_seq disabled
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("roll_seq") ) );
        #endif
        #else
        #ifdef ACTIVATE_EMPTY_ELEMENTS
        node->AddChild(new VDPEmptyOutputNode( std::string("head_position") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("roll") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("roll_increment") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("roll_period") ) );
        node->AddChild(new VDPEmptyOutputNode( std::string("roll_seq") ) );
        #endif
        #endif

        for(int i=0; i<MAX_CHAN; i++)
        {
            //only write out channels which have a name which is defined
            if( std::string(this->fStation.channels[i].chan_name) != "")
            {
                node->AddChild(new VDPObjectOutputNode< ChanStructWrapper >( std::string("channel"), new ChanStructWrapper(this->fStation.channels[i]) ) );
            }
        }

    }
}

void
StationStructWrapper::DefineInputNode(VDPInputNode* node)
{
    //THIS DOES NOT YET WORK
    if(node != NULL)
    {

    }
}

}
