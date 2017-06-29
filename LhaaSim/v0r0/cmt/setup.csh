# echo "Setting LhaaSim v0r0 in /projet/auger/Diane/LHAASIM"

setenv CMTROOT /home/deligny/CDAS/CMT/v1r8cdas
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt -quiet setup -csh -pack=LhaaSim -version=v0r0 -path=/projet/auger/Diane/LHAASIM $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}
