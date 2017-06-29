set tempfile=`${CMTROOT}/mgr/cmt build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt -quiet cleanup -csh -pack=LhaaSim -version=v0r0 -path=/projet/auger/Diane/LHAASIM $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

