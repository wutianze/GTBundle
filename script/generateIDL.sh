#usage: in GTPATH: ./script/generateIDL.sh SerCli
echo name:$1
rm $GTPATH/include/$1.h
rm $GTPATH/src/$1.cxx
rm $GTPATH/include/$1PubSubTypes.h
rm $GTPATH/src/$1PubSubTypes.cxx
fastrtpsgen $GTPATH/idl/$1.idl
mv $1.h $GTPATH/include/$1.h
mv $1.cxx $GTPATH/src/$1.cxx
mv $1PubSubTypes.h $GTPATH/include/$1PubSubTypes.h
mv $1PubSubTypes.cxx $GTPATH/src/$1PubSubTypes.cxx
