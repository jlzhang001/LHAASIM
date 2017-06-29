#-- start of make_header -----------------

#====================================
#  Application CorsToRoot
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

cmt_CorsToRoot_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_CorsToRoot_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_CorsToRoot

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_CorsToRoot = /tmp/CMT_$(Convert_tag)_CorsToRoot.make$(cmt_lock_pid)
else
cmt_local_tagfile_CorsToRoot = $(Convert_tag)_CorsToRoot.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_CorsToRoot = /tmp/CMT_$(Convert_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_CorsToRoot = $(Convert_tag).make
endif

endif

-include $(cmt_local_tagfile_CorsToRoot)

ifdef cmt_CorsToRoot_has_target_tag

ifdef READONLY
cmt_final_setup_CorsToRoot = /tmp/CMT_Convert_CorsToRootsetup.make
cmt_local_CorsToRoot_makefile = /tmp/CMT_CorsToRoot$(cmt_lock_pid).make
else
cmt_final_setup_CorsToRoot = $(bin)Convert_CorsToRootsetup.make
cmt_local_CorsToRoot_makefile = $(bin)CorsToRoot.make
endif

else

ifdef READONLY
cmt_final_setup_CorsToRoot = /tmp/CMT_Convertsetup.make
cmt_local_CorsToRoot_makefile = /tmp/CMT_CorsToRoot$(cmt_lock_pid).make
else
cmt_final_setup_CorsToRoot = $(bin)Convertsetup.make
cmt_local_CorsToRoot_makefile = $(bin)CorsToRoot.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_Convertsetup.make
else
cmt_final_setup = $(bin)Convertsetup.make
endif

CorsToRoot ::


ifdef READONLY
CorsToRoot ::
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
	@echo 'CorsToRoot'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = CorsToRoot/
CorsToRoot::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

CorsToRoot :: dirs  $(bin)CorsToRoot${application_suffix}
	@/bin/echo "------> CorsToRoot ok"

#-- end of application_header
#-- start of application

$(bin)CorsToRoot${application_suffix} :: $(bin)CorsToRoot.o $(bin)globalvar.o $(use_stamps) $(CorsToRootstamps) requirements $(use_requirements)
	$(link_echo) $@
	$(link_silent) cd $(bin); $(cpplink) -o CorsToRoot.exe.new $(bin)CorsToRoot.o $(bin)globalvar.o $(use_linkopts) $(CorsToRootlinkopts); mv -f CorsToRoot.exe.new CorsToRoot${application_suffix}

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
CorsToRootinstallname = CorsToRoot${application_suffix}

CorsToRoot :: CorsToRootinstall

install :: CorsToRootinstall

CorsToRootinstall :: $(install_dir)/$(CorsToRootinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(CorsToRootinstallname) :: $(bin)$(CorsToRootinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(CorsToRootinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

##CorsToRootclean :: CorsToRootuninstall

uninstall :: CorsToRootuninstall

CorsToRootuninstall ::
	@echo "------> (CorsToRoot.make) Removing installed files"
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(CorsToRootinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

#-- end of application
#-- start of dependency ------------------

$(bin)CorsToRoot_dependencies.make :: dirs

ifndef QUICK
$(bin)CorsToRoot_dependencies.make :: $(src)CorsToRoot.cc $(src)globalvar.cc  requirements $(use_requirements) $(cmt_final_setup_CorsToRoot)
	@echo "------> (CorsToRoot.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) CorsToRoot -all_sources $${args}
endif

#$(CorsToRoot_dependencies)

-include $(bin)CorsToRoot_dependencies.make

#-- end of dependency -------------------
#-- start of cpp ------

$(bin)$(binobj)CorsToRoot.o : $(CorsToRoot_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)CorsToRoot.o $(use_pp_cppflags) $(CorsToRoot_pp_cppflags) $(app_CorsToRoot_pp_cppflags) $(CorsToRoot_pp_cppflags) $(use_cppflags) $(CorsToRoot_cppflags) $(app_CorsToRoot_cppflags) $(CorsToRoot_cppflags) $(CorsToRoot_cc_cppflags)  $(src)CorsToRoot.cc

#-- end of cpp ------
#-- start of cpp ------

$(bin)$(binobj)globalvar.o : $(globalvar_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)globalvar.o $(use_pp_cppflags) $(CorsToRoot_pp_cppflags) $(app_CorsToRoot_pp_cppflags) $(globalvar_pp_cppflags) $(use_cppflags) $(CorsToRoot_cppflags) $(app_CorsToRoot_cppflags) $(globalvar_cppflags) $(globalvar_cc_cppflags)  $(src)globalvar.cc

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: CorsToRootclean
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




CorsToRootclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	@echo "------> (CorsToRoot.make) Removing application files"
	-$(cleanup_echo) CorsToRoot${application_suffix}
	-@cd $(bin); /bin/rm -f CorsToRoot${application_suffix}
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)CorsToRoot.o $(bin)globalvar.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf CorsToRoot_deps CorsToRoot_dependencies.make
#-- end of cleanup_objects ------
