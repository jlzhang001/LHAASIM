#!/bin/sh
DESTDIR=EasySim
# basic checks
[ -d $DESTDIR ] && echo "destination dir '$DESTDIR' already exists"
[ -d $DESTDIR ] && exit 0
[ -d ../src ] || echo "Can't find source dir"
[ -d ../src ] || exit 0
# mkdirs
mkdir $DESTDIR
mkdir $DESTDIR/src $DESTDIR/data $DESTDIR/bin
# getting EasySim software (widthout eventual dicts)
cp ../src/*.cc ../src/*.h $DESTDIR/src
cp ../data/EASYSIM_README.txt $DESTDIR
rm -f ../src/*Dict*
# adding package specific stuff
cp default.inp $DESTDIR/data
cp PACK_README $DESTDIR/README
cp Ascii.cc $DESTDIR/src
# adding Makefiles for compilation
cp Makefile_Main $DESTDIR/Makefile
cp Makefile_Src $DESTDIR/src/Makefile
cp Makefile_Data $DESTDIR/data/Makefile
# adding dependancies
cd $DESTDIR/src
makedepend -Y. -w500 *.cc 2> /dev/null
cat Makefile | awk '{print $0; if ($1=="EventDict.o:") {$1="EventDict.cc:"; print $0}; if ($1=="AnaDict.o:") {$1="AnaDict.cc:"; print $0};}' > Makefile.bak && mv Makefile.bak Makefile
cd ../..
# proper rights
find $DESTDIR -type d -exec chmod 755 {} \;
find $DESTDIR -type f -exec chmod 644 {} \;
