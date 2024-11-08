#!/bin/sh

#must run this script as: source ./chopsenv.sh

if [ -n "$CHOPSSYS" ]
    then
        export OLD_CHOPSSYS=$CHOPSSYS
        OLD_PATH=$OLD_CHOPSSYS/bin:
        OLD_LDLIBPATH=$OLD_CHOPSSYS/lib:
        OLD_CMAKE_PREF=$OLD_CHOPSSYS:
fi

if [ -n "$CHOPSSYSPY" ]
    then
        export OLD_CHOPSSYSPY=$CHOPSSYSPY
fi

if [ -n "$CHOPS_INSTALL" ]
    then
        export OLD_CHOPS_INSTALL=$CHOPS_INSTALL
        OLD_PATH=$OLD_CHOPS_INSTALL/bin:
        OLD_LDLIBPATH=$OLD_CHOPS_INSTALL/lib:
        OLD_CMAKE_PREF=$OLD_CHOPS_INSTALL:
fi

export CHOPS_INSTALL=@CMAKE_INSTALL_PREFIX@

if [ $# -eq 0 ]
  then
    CHOPSSYS=$CHOPS_INSTALL
  else
    CHOPSSYS=`readlink -f $1`
fi

export CHOPSSYS
export CHOPSSYSPY=@CMAKE_INSTALL_PREFIX@/@PYTHON_SITE_PREFIX@

#TODO: make the 'TEXT' variable have a less generic name
export TEXT=$CHOPS_INSTALL/share/chops/text

export PATH=$CHOPS_INSTALL/bin:${PATH//${OLD_PATH}/}
export LD_LIBRARY_PATH=$CHOPS_INSTALL/lib:${LD_LIBRARY_PATH//${OLD_LDLIBPATH}/}
export CMAKE_PREFIX_PATH=$CHOPS_INSTALL:${CMAKE_PREFIX_PATH//${OLD_CMAKE_PREF}/}
export TESTDATADIR=@DATA_INSTALL_DIR@/testdata
export DATADIR=@DATA_INSTALL_DIR@/testdata
export DEF_CONTROL=/dev/null
export PYTHONPATH=${CHOPSSYSPY}:${PYTHONPATH//${OLD_CHOPSSYSPY}}

#make tests verbose
export testverb=1

echo -e "CHOPS install directory set to ${CHOPS_INSTALL}"

return 0
