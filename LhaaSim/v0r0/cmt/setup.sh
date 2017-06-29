# echo "Setting LhaaSim v0r0 in /projet/auger/Diane/LHAASIM"

CMTROOT=/home/deligny/CDAS/CMT/v1r8cdas; export CMTROOT
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt -quiet setup -sh -pack=LhaaSim -version=v0r0 -path=/projet/auger/Diane/LHAASIM $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}
