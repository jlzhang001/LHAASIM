#-- start of make_header -----------------

#====================================
#  Library Ana
#
#   Generated Wed May 17 11:31:15 2017  by martraire
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_Ana_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Ana_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_Ana

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Ana = /tmp/CMT_$(LhaaSim_tag)_Ana.make$(cmt_lock_pid)
else
cmt_local_tagfile_Ana = $(LhaaSim_tag)_Ana.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Ana = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_Ana = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_Ana)

ifdef cmt_Ana_has_target_tag

ifdef READONLY
cmt_final_setup_Ana = /tmp/CMT_LhaaSim_Anasetup.make
cmt_local_Ana_makefile = /tmp/CMT_Ana$(cmt_lock_pid).make
else
cmt_final_setup_Ana = $(bin)LhaaSim_Anasetup.make
cmt_local_Ana_makefile = $(bin)Ana.make
endif

else

ifdef READONLY
cmt_final_setup_Ana = /tmp/CMT_LhaaSimsetup.make
cmt_local_Ana_makefile = /tmp/CMT_Ana$(cmt_lock_pid).make
else
cmt_final_setup_Ana = $(bin)LhaaSimsetup.make
cmt_local_Ana_makefile = $(bin)Ana.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

Ana ::


ifdef READONLY
Ana ::
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
	@echo 'Ana'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = Ana/
Ana::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

Analibname   = $(bin)$(library_prefix)Ana$(library_suffix)
Analib       = $(Analibname).a
Anastamp     = $(bin)Ana.stamp
Anashstamp   = $(bin)Ana.shstamp

Ana :: dirs  AnaLIB
	@/bin/echo "------> Ana ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

AnaLIB :: $(Analib) $(Anashstamp)
	@/bin/echo "------> Ana : library ok"

$(Analib) :: $(bin)Analyze.o $(bin)AnaDict.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(Analib) $?
	$(lib_silent) $(ranlib) $(Analib)
	$(lib_silent) cat /dev/null >$(Anastamp)

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

$(Analibname).$(shlibsuffix) :: $(Analib) requirements $(use_requirements) $(Anastamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" Ana $(Ana_shlibflags)

$(Anashstamp) :: $(Analibname).$(shlibsuffix)
	@if test -f $(Analibname).$(shlibsuffix) ; then cat /dev/null >$(Anashstamp) ; fi

Anaclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)Analyze.o $(bin)AnaDict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf Ana_deps Ana_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
Anainstallname = $(library_prefix)Ana$(library_suffix).$(shlibsuffix)

Ana :: Anainstall

install :: Anainstall

Anainstall :: $(install_dir)/$(Anainstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(Anainstallname) :: $(bin)$(Anainstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Anainstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi

##Anaclean :: Anauninstall

uninstall :: Anauninstall

Anauninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Anainstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi


#-- end of libary -----------------------
#-- start of dependency ------------------

$(bin)Ana_dependencies.make :: dirs

ifndef QUICK
$(bin)Ana_dependencies.make :: $(src)Analyze.cc $(src)AnaDict.cc  requirements $(use_requirements) $(cmt_final_setup_Ana)
	@echo "------> (Ana.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) Ana -all_sources $${args}
endif

#$(Ana_dependencies)

-include $(bin)Ana_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Analyze.o : $(Analyze_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Analyze.o $(use_pp_cppflags) $(Ana_pp_cppflags) $(lib_Ana_pp_cppflags) $(Analyze_pp_cppflags) $(use_cppflags) $(Ana_cppflags) $(lib_Ana_cppflags) $(Analyze_cppflags) $(Analyze_cc_cppflags)  $(src)Analyze.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)AnaDict.o : $(AnaDict_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)AnaDict.o $(use_pp_cppflags) $(Ana_pp_cppflags) $(lib_Ana_pp_cppflags) $(AnaDict_pp_cppflags) $(use_cppflags) $(Ana_cppflags) $(lib_Ana_cppflags) $(AnaDict_cppflags) $(AnaDict_cc_cppflags)  $(src)AnaDict.cc

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: Anaclean
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




Anaclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)Ana$(library_suffix).a $(binobj)$(library_prefix)Ana$(library_suffix).s? $(binobj)Ana.stamp $(binobj)Ana.shstamp
#-- end of cleanup_library ---------------

