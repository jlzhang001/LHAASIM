#-- start of make_header -----------------

#====================================
#  Library ShowerIO
#
#   Generated Wed Jun 14 21:39:32 2017  by martraire
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_ShowerIO_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_ShowerIO_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_ShowerIO

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_ShowerIO = /tmp/CMT_$(Convert_tag)_ShowerIO.make$(cmt_lock_pid)
else
cmt_local_tagfile_ShowerIO = $(Convert_tag)_ShowerIO.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_ShowerIO = /tmp/CMT_$(Convert_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_ShowerIO = $(Convert_tag).make
endif

endif

-include $(cmt_local_tagfile_ShowerIO)

ifdef cmt_ShowerIO_has_target_tag

ifdef READONLY
cmt_final_setup_ShowerIO = /tmp/CMT_Convert_ShowerIOsetup.make
cmt_local_ShowerIO_makefile = /tmp/CMT_ShowerIO$(cmt_lock_pid).make
else
cmt_final_setup_ShowerIO = $(bin)Convert_ShowerIOsetup.make
cmt_local_ShowerIO_makefile = $(bin)ShowerIO.make
endif

else

ifdef READONLY
cmt_final_setup_ShowerIO = /tmp/CMT_Convertsetup.make
cmt_local_ShowerIO_makefile = /tmp/CMT_ShowerIO$(cmt_lock_pid).make
else
cmt_final_setup_ShowerIO = $(bin)Convertsetup.make
cmt_local_ShowerIO_makefile = $(bin)ShowerIO.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_Convertsetup.make
else
cmt_final_setup = $(bin)Convertsetup.make
endif

ShowerIO ::


ifdef READONLY
ShowerIO ::
	@echo tags=$(tags)
	@echo cmt_local_tagfile=$(cmt_local_tagfile)
endif


dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	@echo 'ShowerIO'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = ShowerIO/
ShowerIO::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

ShowerIOlibname   = $(bin)$(library_prefix)ShowerIO$(library_suffix)
ShowerIOlib       = $(ShowerIOlibname).a
ShowerIOstamp     = $(bin)ShowerIO.stamp
ShowerIOshstamp   = $(bin)ShowerIO.shstamp

ShowerIO :: dirs  ShowerIOLIB
	@/bin/echo "------> ShowerIO ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

ShowerIOLIB :: $(ShowerIOlib) $(ShowerIOshstamp)
	@/bin/echo "------> ShowerIO : library ok"

$(ShowerIOlib) :: $(bin)IOCorsika.o $(bin)RotToAugerFr.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(ShowerIOlib) $?
	$(lib_silent) $(ranlib) $(ShowerIOlib)
	$(lib_silent) cat /dev/null >$(ShowerIOstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(ShowerIOlibname).$(shlibsuffix) :: $(ShowerIOlib) requirements $(use_requirements) $(ShowerIOstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" ShowerIO $(ShowerIO_shlibflags)

$(ShowerIOshstamp) :: $(ShowerIOlibname).$(shlibsuffix)
	@if test -f $(ShowerIOlibname).$(shlibsuffix) ; then cat /dev/null >$(ShowerIOshstamp) ; fi

ShowerIOclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)IOCorsika.o $(bin)RotToAugerFr.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf ShowerIO_deps ShowerIO_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
ShowerIOinstallname = $(library_prefix)ShowerIO$(library_suffix).$(shlibsuffix)

ShowerIO :: ShowerIOinstall

install :: ShowerIOinstall

ShowerIOinstall :: $(install_dir)/$(ShowerIOinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(ShowerIOinstallname) :: $(bin)$(ShowerIOinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(ShowerIOinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi

##ShowerIOclean :: ShowerIOuninstall

uninstall :: ShowerIOuninstall

ShowerIOuninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(ShowerIOinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi


#-- end of libary -----------------------
#-- start of dependency ------------------

$(bin)ShowerIO_dependencies.make :: dirs

ifndef QUICK
$(bin)ShowerIO_dependencies.make :: $(src)IOCorsika.cc $(src)RotToAugerFr.cc  requirements $(use_requirements) $(cmt_final_setup_ShowerIO)
	@echo "------> (ShowerIO.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) ShowerIO -all_sources $${args}
endif

#$(ShowerIO_dependencies)

-include $(bin)ShowerIO_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)IOCorsika.o : $(IOCorsika_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)IOCorsika.o $(use_pp_cppflags) $(ShowerIO_pp_cppflags) $(lib_ShowerIO_pp_cppflags) $(IOCorsika_pp_cppflags) $(use_cppflags) $(ShowerIO_cppflags) $(lib_ShowerIO_cppflags) $(IOCorsika_cppflags) $(IOCorsika_cc_cppflags)  $(src)IOCorsika.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)RotToAugerFr.o : $(RotToAugerFr_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)RotToAugerFr.o $(use_pp_cppflags) $(ShowerIO_pp_cppflags) $(lib_ShowerIO_pp_cppflags) $(RotToAugerFr_pp_cppflags) $(use_cppflags) $(ShowerIO_cppflags) $(lib_ShowerIO_cppflags) $(RotToAugerFr_cppflags) $(RotToAugerFr_cc_cppflags)  $(src)RotToAugerFr.cc

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: ShowerIOclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	@echo "You should provide a target for this file  $@"
#	@exit 0
endif

#ifdef PEDANTIC
#.DEFAULT::
#	@echo "You should provide a target for this file  $@"
#	@exit 1
#endif




ShowerIOclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)ShowerIO$(library_suffix).a $(binobj)$(library_prefix)ShowerIO$(library_suffix).s? $(binobj)ShowerIO.stamp $(binobj)ShowerIO.shstamp
#-- end of cleanup_library ---------------

