#!/bin/bash

#check if we were passed the flag --checksum-only, if so, we only need to
#compare the files, and return 0 (1) if they are the same (different)

me=`basename $0 2>&-` || me=sh
[ "$me" = import_hops.sh ] && return=exit || return=return

CHKSUM=0
if [ "$1" == "--checksum-only" ]; then
	CHKSUM=1
fi

ret_val=0

#check for the existence of the directory variable, or if they are unset
#check if we are running the script from the chops directory
if [[ -e ./import_hops.sh ]]; then
    CHOPS_SRC_DIR=$PWD
    HOPS_SRC_DIR=${CHOPS_SRC_DIR}/../
fi

if [ "$CHOPS_SRC_DIR" != "" ] && [ "$HOPS_SRC_DIR" != "" ]; then

    if [ "${CHKSUM}" -eq "0" ]
    then
        source ${CHOPS_SRC_DIR}/source/c_src/afio/import_afio.sh
        source ${CHOPS_SRC_DIR}/source/c_src/dfio/import_dfio.sh
        source ${CHOPS_SRC_DIR}/source/c_src/mk4util/import_mk4util.sh
        source ${CHOPS_SRC_DIR}/source/c_src/vex/import_vex.sh
        source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffcontrol/import_ffcontrol.sh
        source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffcore/import_ffcore.sh
        source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffio/import_ffio.sh
        source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffmath/import_ffmath.sh
        source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffplot/import_ffplot.sh
        source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffsearch/import_ffsearch.sh
        source ${CHOPS_SRC_DIR}/source/c_src/applications/fourfit/import_fourfit.sh
        source ${CHOPS_SRC_DIR}/source/c_src/applications/alist/import_alist.sh
        source ${CHOPS_SRC_DIR}/source/c_src/applications/aedit/import_aedit.sh
    else
        (source ${CHOPS_SRC_DIR}/source/c_src/afio/import_afio.sh --checksum-only)
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/afio/import_afio.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/dfio/import_dfio.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/mk4util/import_mk4util.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/vex/import_vex.sh --checksum-only)
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffcontrol/import_ffcontrol.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffcore/import_ffcore.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffio/import_ffio.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffmath/import_ffmath.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffplot/import_ffplot.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi
        (source ${CHOPS_SRC_DIR}/source/c_src/fourfit_libs/ffsearch/import_ffsearch.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi

        (source ${CHOPS_SRC_DIR}/source/c_src/applications/fourfit/import_fourfit.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi

        (source ${CHOPS_SRC_DIR}/source/c_src/applications/alist/import_alist.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi

        (source ${CHOPS_SRC_DIR}/source/c_src/applications/aedit/import_aedit.sh --checksum-only )
        tmp_val=$?
        if [ $tmp_val ]
        then
            ret_val=1
        fi

    fi

else

    echo "Please define the variables CHOPS_SRC_DIR and HOPS_SRC_DIR or cd into the chops directory before running this script."
    ret_val=2

fi

$return ${ret_val}
