echo name:$1
rm include/$1.h
rm src/$1.cxx
rm include/$1PubSubTypes.h
rm src/$1PubSubTypes.cxx
fastrtpsgen idl/$1.idl
mv $1.h include/$1.h
mv $1.cxx src/$1.cxx
mv $1PubSubTypes.h include/$1PubSubTypes.h
mv $1PubSubTypes.cxx src/$1PubSubTypes.cxx
