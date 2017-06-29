#-- start of make_header -----------------

#====================================
#  Application AnaEvent
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

cmt_AnaEvent_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_AnaEvent_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_AnaEvent

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_AnaEvent = /tmp/CMT_$(LhaaSim_tag)_AnaEvent.make$(cmt_lock_pid)
else
cmt_local_tagfile_AnaEvent = $(LhaaSim_tag)_AnaEvent.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_AnaEvent = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_AnaEvent = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_AnaEvent)

ifdef cmt_AnaEvent_has_target_tag

ifdef READONLY
cmt_final_setup_AnaEvent = /tmp/CMT_LhaaSim_AnaEventsetup.make
cmt_local_AnaEvent_makefile = /tmp/CMT_AnaEvent$(cmt_lock_pid).make
else
cmt_final_setup_AnaEvent = $(bin)LhaaSim_AnaEventsetup.make
cmt_local_AnaEvent_makefile = $(bin)AnaEvent.make
endif

else

ifdef READONLY
cmt_final_setup_AnaEvent = /tmp/CMT_LhaaSimsetup.make
cmt_local_AnaEvent_makefile = /tmp/CMT_AnaEvent$(cmt_lock_pid).make
else
cmt_final_setup_AnaEvent = $(bin)LhaaSimsetup.make
cmt_local_AnaEvent_makefile = $(bin)AnaEvent.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

AnaEvent ::


ifdef READONLY
AnaEvent ::
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
	@echo 'AnaEvent'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = AnaEvent/
AnaEvent::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

AnaEvent :: dirs  $(bin)AnaEvent${application_suffix}
	@/bin/echo "------> AnaEvent ok"

#-- end of application_header
#-- start of application

$(bin)AnaEvent${application_suffix} :: $(bin)AnaEvent.o $(use_stamps) $(AnaEventstamps) requirements $(use_requirements)
	$(link_echo) $@
	$(link_silent) cd $(bin); $(cpplink) -o AnaEvent.exe.new $(bin)AnaEvent.o $(use_linkopts) $(AnaEventlinkopts); mv -f AnaEvent.exe.new AnaEvent${application_suffix}

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
AnaEventinstallname = AnaEvent${application_suffix}

AnaEvent :: AnaEventinstall

install :: AnaEventinstall

AnaEventinstall :: $(install_dir)/$(AnaEventinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(AnaEventinstallname) :: $(bin)$(AnaEventinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(AnaEventinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

##AnaEventclean :: AnaEventuninstall

uninstall :: AnaEventuninstall

AnaEventuninstall ::
	@echo "------> (AnaEvent.make) Removing installed files"
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(AnaEventinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

#-- end of application
#-- start of dependency ------------------

$(bin)AnaEvent_dependencies.make :: dirs

ifndef QUICK
$(bin)AnaEvent_dependencies.make :: $(src)AnaEvent.cc  requirements $(use_requirements) $(cmt_final_setup_AnaEvent)
	@echo "------> (AnaEvent.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) AnaEvent -all_sources $${args}
endif

#$(AnaEvent_dependencies)

-include $(bin)AnaEvent_dependencies.make

#-- end of dependency -------------------
#-- start of cpp ------

$(bin)$(binobj)AnaEvent.o : $(AnaEvent_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)AnaEvent.o $(use_pp_cppflags) $(AnaEvent_pp_cppflags) $(app_AnaEvent_pp_cppflags) $(AnaEvent_pp_cppflags) $(use_cppflags) $(AnaEvent_cppflags) $(app_AnaEvent_cppflags) $(AnaEvent_cppflags) $(AnaEvent_cc_cppflags)  $(src)AnaEvent.cc

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: AnaEventclean
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




AnaEventclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	@echo "------> (AnaEvent.make) Removing application files"
	-$(cleanup_echo) AnaEvent${application_suffix}
	-@cd $(bin); /bin/rm -f AnaEvent${application_suffix}
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)AnaEvent.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf AnaEvent_deps AnaEvent_dependencies.make
#-- end of cleanup_objects ------
