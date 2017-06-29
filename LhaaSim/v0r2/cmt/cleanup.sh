if test "${CMTROOT}" = ""; then
  CMTROOT=/home/martraire/CMT/v1r18p20060606; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=LhaaSim -version=v0r2 -path=/home/martraire/Work/LHAASIM $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

