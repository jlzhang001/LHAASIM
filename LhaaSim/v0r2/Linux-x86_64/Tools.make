#-- start of make_header -----------------

#====================================
#  Library Tools
#
#   Generated Tue Jun 20 18:59:21 2017  by martraire
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_Tools_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Tools_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_Tools

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Tools = /tmp/CMT_$(LhaaSim_tag)_Tools.make$(cmt_lock_pid)
else
cmt_local_tagfile_Tools = $(LhaaSim_tag)_Tools.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Tools = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_Tools = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_Tools)

ifdef cmt_Tools_has_target_tag

ifdef READONLY
cmt_final_setup_Tools = /tmp/CMT_LhaaSim_Toolssetup.make
cmt_local_Tools_makefile = /tmp/CMT_Tools$(cmt_lock_pid).make
else
cmt_final_setup_Tools = $(bin)LhaaSim_Toolssetup.make
cmt_local_Tools_makefile = $(bin)Tools.make
endif

else

ifdef READONLY
cmt_final_setup_Tools = /tmp/CMT_LhaaSimsetup.make
cmt_local_Tools_makefile = /tmp/CMT_Tools$(cmt_lock_pid).make
else
cmt_final_setup_Tools = $(bin)LhaaSimsetup.make
cmt_local_Tools_makefile = $(bin)Tools.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

Tools ::


ifdef READONLY
Tools ::
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
	@echo 'Tools'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = Tools/
Tools::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

Toolslibname   = $(bin)$(library_prefix)Tools$(library_suffix)
Toolslib       = $(Toolslibname).a
Toolsstamp     = $(bin)Tools.stamp
Toolsshstamp   = $(bin)Tools.shstamp

Tools :: dirs  ToolsLIB
	@/bin/echo "------> Tools ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

ToolsLIB :: $(Toolslib) $(Toolsshstamp)
	@/bin/echo "------> Tools : library ok"

$(Toolslib) :: $(bin)ShowerParam.o $(bin)Utils.o $(bin)BuildProcessesTables.o $(bin)FastSimulation.o $(bin)ManageEvent.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(Toolslib) $?
	$(lib_silent) $(ranlib) $(Toolslib)
	$(lib_silent) cat /dev/null >$(Toolsstamp)

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

$(Toolslibname).$(shlibsuffix) :: $(Toolslib) requirements $(use_requirements) $(Toolsstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" Tools $(Tools_shlibflags)

$(Toolsshstamp) :: $(Toolslibname).$(shlibsuffix)
	@if test -f $(Toolslibname).$(shlibsuffix) ; then cat /dev/null >$(Toolsshstamp) ; fi

Toolsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)ShowerParam.o $(bin)Utils.o $(bin)BuildProcessesTables.o $(bin)FastSimulation.o $(bin)ManageEvent.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf Tools_deps Tools_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
Toolsinstallname = $(library_prefix)Tools$(library_suffix).$(shlibsuffix)

Tools :: Toolsinstall

install :: Toolsinstall

Toolsinstall :: $(install_dir)/$(Toolsinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(Toolsinstallname) :: $(bin)$(Toolsinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Toolsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi

##Toolsclean :: Toolsuninstall

uninstall :: Toolsuninstall

Toolsuninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Toolsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi


#-- end of libary -----------------------
#-- start of dependency ------------------

$(bin)Tools_dependencies.make :: dirs

ifndef QUICK
$(bin)Tools_dependencies.make :: $(src)ShowerParam.cc $(src)Utils.cc $(src)BuildProcessesTables.cc $(src)FastSimulation.cc $(src)ManageEvent.cc  requirements $(use_requirements) $(cmt_final_setup_Tools)
	@echo "------> (Tools.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) Tools -all_sources $${args}
endif

#$(Tools_dependencies)

-include $(bin)Tools_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)ShowerParam.o : $(ShowerParam_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)ShowerParam.o $(use_pp_cppflags) $(Tools_pp_cppflags) $(lib_Tools_pp_cppflags) $(ShowerParam_pp_cppflags) $(use_cppflags) $(Tools_cppflags) $(lib_Tools_cppflags) $(ShowerParam_cppflags) $(ShowerParam_cc_cppflags)  $(src)ShowerParam.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Utils.o : $(Utils_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Utils.o $(use_pp_cppflags) $(Tools_pp_cppflags) $(lib_Tools_pp_cppflags) $(Utils_pp_cppflags) $(use_cppflags) $(Tools_cppflags) $(lib_Tools_cppflags) $(Utils_cppflags) $(Utils_cc_cppflags)  $(src)Utils.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)BuildProcessesTables.o : $(BuildProcessesTables_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)BuildProcessesTables.o $(use_pp_cppflags) $(Tools_pp_cppflags) $(lib_Tools_pp_cppflags) $(BuildProcessesTables_pp_cppflags) $(use_cppflags) $(Tools_cppflags) $(lib_Tools_cppflags) $(BuildProcessesTables_cppflags) $(BuildProcessesTables_cc_cppflags)  $(src)BuildProcessesTables.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)FastSimulation.o : $(FastSimulation_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)FastSimulation.o $(use_pp_cppflags) $(Tools_pp_cppflags) $(lib_Tools_pp_cppflags) $(FastSimulation_pp_cppflags) $(use_cppflags) $(Tools_cppflags) $(lib_Tools_cppflags) $(FastSimulation_cppflags) $(FastSimulation_cc_cppflags)  $(src)FastSimulation.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)ManageEvent.o : $(ManageEvent_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)ManageEvent.o $(use_pp_cppflags) $(Tools_pp_cppflags) $(lib_Tools_pp_cppflags) $(ManageEvent_pp_cppflags) $(use_cppflags) $(Tools_cppflags) $(lib_Tools_cppflags) $(ManageEvent_cppflags) $(ManageEvent_cc_cppflags)  $(src)ManageEvent.cc

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: Toolsclean
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




Toolsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)Tools$(library_suffix).a $(binobj)$(library_prefix)Tools$(library_suffix).s? $(binobj)Tools.stamp $(binobj)Tools.shstamp
#-- end of cleanup_library ---------------

