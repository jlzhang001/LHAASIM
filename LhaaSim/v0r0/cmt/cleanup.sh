tempfile=`${CMTROOT}/mgr/cmt build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt -quiet cleanup -sh -pack=LhaaSim -version=v0r0 -path=/projet/auger/Diane/LHAASIM $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

