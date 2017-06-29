#-- start of make_header -----------------

#====================================
#  Library Utils
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

cmt_Utils_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Utils_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_Utils

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Utils = /tmp/CMT_$(Convert_tag)_Utils.make$(cmt_lock_pid)
else
cmt_local_tagfile_Utils = $(Convert_tag)_Utils.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Utils = /tmp/CMT_$(Convert_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_Utils = $(Convert_tag).make
endif

endif

-include $(cmt_local_tagfile_Utils)

ifdef cmt_Utils_has_target_tag

ifdef READONLY
cmt_final_setup_Utils = /tmp/CMT_Convert_Utilssetup.make
cmt_local_Utils_makefile = /tmp/CMT_Utils$(cmt_lock_pid).make
else
cmt_final_setup_Utils = $(bin)Convert_Utilssetup.make
cmt_local_Utils_makefile = $(bin)Utils.make
endif

else

ifdef READONLY
cmt_final_setup_Utils = /tmp/CMT_Convertsetup.make
cmt_local_Utils_makefile = /tmp/CMT_Utils$(cmt_lock_pid).make
else
cmt_final_setup_Utils = $(bin)Convertsetup.make
cmt_local_Utils_makefile = $(bin)Utils.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_Convertsetup.make
else
cmt_final_setup = $(bin)Convertsetup.make
endif

Utils ::


ifdef READONLY
Utils ::
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
	@echo 'Utils'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = Utils/
Utils::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

Utilslibname   = $(bin)$(library_prefix)Utils$(library_suffix)
Utilslib       = $(Utilslibname).a
Utilsstamp     = $(bin)Utils.stamp
Utilsshstamp   = $(bin)Utils.shstamp

Utils :: dirs  UtilsLIB
	@/bin/echo "------> Utils ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

UtilsLIB :: $(Utilslib) $(Utilsshstamp)
	@/bin/echo "------> Utils : library ok"

$(Utilslib) :: $(bin)Utils.o $(bin)utilities.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(Utilslib) $?
	$(lib_silent) $(ranlib) $(Utilslib)
	$(lib_silent) cat /dev/null >$(Utilsstamp)

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

$(Utilslibname).$(shlibsuffix) :: $(Utilslib) requirements $(use_requirements) $(Utilsstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" Utils $(Utils_shlibflags)

$(Utilsshstamp) :: $(Utilslibname).$(shlibsuffix)
	@if test -f $(Utilslibname).$(shlibsuffix) ; then cat /dev/null >$(Utilsshstamp) ; fi

Utilsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)Utils.o $(bin)utilities.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf Utils_deps Utils_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
Utilsinstallname = $(library_prefix)Utils$(library_suffix).$(shlibsuffix)

Utils :: Utilsinstall

install :: Utilsinstall

Utilsinstall :: $(install_dir)/$(Utilsinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(Utilsinstallname) :: $(bin)$(Utilsinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Utilsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi

##Utilsclean :: Utilsuninstall

uninstall :: Utilsuninstall

Utilsuninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Utilsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi


#-- end of libary -----------------------
#-- start of dependency ------------------

$(bin)Utils_dependencies.make :: dirs

ifndef QUICK
$(bin)Utils_dependencies.make :: $(src)Utils.cc $(src)utilities.cc  requirements $(use_requirements) $(cmt_final_setup_Utils)
	@echo "------> (Utils.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) Utils -all_sources $${args}
endif

#$(Utils_dependencies)

-include $(bin)Utils_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Utils.o : $(Utils_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Utils.o $(use_pp_cppflags) $(Utils_pp_cppflags) $(lib_Utils_pp_cppflags) $(Utils_pp_cppflags) $(use_cppflags) $(Utils_cppflags) $(lib_Utils_cppflags) $(Utils_cppflags) $(Utils_cc_cppflags)  $(src)Utils.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)utilities.o : $(utilities_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)utilities.o $(use_pp_cppflags) $(Utils_pp_cppflags) $(lib_Utils_pp_cppflags) $(utilities_pp_cppflags) $(use_cppflags) $(Utils_cppflags) $(lib_Utils_cppflags) $(utilities_cppflags) $(utilities_cc_cppflags)  $(src)utilities.cc

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: Utilsclean
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




Utilsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)Utils$(library_suffix).a $(binobj)$(library_prefix)Utils$(library_suffix).s? $(binobj)Utils.stamp $(binobj)Utils.shstamp
#-- end of cleanup_library ---------------

