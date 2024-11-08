{
split ($0, name, ".")
position = length (name[1]) + 16
print "vhelp: ", sprintf ("%-12s %s", name[1], substr ($0, position))}
