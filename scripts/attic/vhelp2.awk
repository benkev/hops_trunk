BEGIN { usection = toupper (section)
    printit = 0
    if (index ("SYNTAX", usection) != 0) {start="SYNTAX";stop="OPTION"}
    if (index ("OPTION", usection) != 0) {start="OPTION";stop="ARGUMENTS"}
    if (index ("ARGUMENTS", usection) != 0) {start="ARGUMENTS";stop="ENVIRONMENT"}
    if (index ("ENVIRONMENT", usection) != 0) {start="ENVIRONMENT";stop="DESCRIPTION"}
    if (index ("DESCRIPTION", usection) != 0) {start="DESCRIPTION";stop="LIBRARIES"}
    if (index ("UPDATE", usection) != 0) {start="UPDATE";stop="The end ..."}}

    {gsub ("\t", "        ")}
    {if (index ($0, start) > 0) printit=1}
    {if (index ($0, stop) > 0) printit=0}
    {if (printit > 0) printf ("vhelp: %s\n", $0)}
