# echo "Setting Convert v0r2 in /home/martraire/Work/LHAASIM"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /home/martraire/CMT/v1r18p20060606
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=Convert -version=v0r2 -path=/home/martraire/Work/LHAASIM  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

