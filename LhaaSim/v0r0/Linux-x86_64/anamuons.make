#-- start of make_header -----------------

#====================================
#  Application anamuons
#
#   Generated Wed May 17 11:31:16 2017  by martraire
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_anamuons_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_anamuons_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_anamuons

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_anamuons = /tmp/CMT_$(LhaaSim_tag)_anamuons.make$(cmt_lock_pid)
else
cmt_local_tagfile_anamuons = $(LhaaSim_tag)_anamuons.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_anamuons = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_anamuons = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_anamuons)

ifdef cmt_anamuons_has_target_tag

ifdef READONLY
cmt_final_setup_anamuons = /tmp/CMT_LhaaSim_anamuonssetup.make
cmt_local_anamuons_makefile = /tmp/CMT_anamuons$(cmt_lock_pid).make
else
cmt_final_setup_anamuons = $(bin)LhaaSim_anamuonssetup.make
cmt_local_anamuons_makefile = $(bin)anamuons.make
endif

else

ifdef READONLY
cmt_final_setup_anamuons = /tmp/CMT_LhaaSimsetup.make
cmt_local_anamuons_makefile = /tmp/CMT_anamuons$(cmt_lock_pid).make
else
cmt_final_setup_anamuons = $(bin)LhaaSimsetup.make
cmt_local_anamuons_makefile = $(bin)anamuons.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

anamuons ::


ifdef READONLY
anamuons ::
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
	@echo 'anamuons'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = anamuons/
anamuons::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

anamuons :: dirs  $(bin)anamuons${application_suffix}
	@/bin/echo "------> anamuons ok"

#-- end of application_header
#-- start of application

$(bin)anamuons${application_suffix} :: $(bin)anamuons.o $(use_stamps) $(anamuonsstamps) requirements $(use_requirements)
	$(link_echo) $@
	$(link_silent) cd $(bin); $(cpplink) -o anamuons.exe.new $(bin)anamuons.o $(use_linkopts) $(anamuonslinkopts); mv -f anamuons.exe.new anamuons${application_suffix}

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
anamuonsinstallname = anamuons${application_suffix}

anamuons :: anamuonsinstall

install :: anamuonsinstall

anamuonsinstall :: $(install_dir)/$(anamuonsinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(anamuonsinstallname) :: $(bin)$(anamuonsinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(anamuonsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

##anamuonsclean :: anamuonsuninstall

uninstall :: anamuonsuninstall

anamuonsuninstall ::
	@echo "------> (anamuons.make) Removing installed files"
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(anamuonsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

#-- end of application
#-- start of dependency ------------------

$(bin)anamuons_dependencies.make :: dirs

ifndef QUICK
$(bin)anamuons_dependencies.make :: $(src)anamuons.cc  requirements $(use_requirements) $(cmt_final_setup_anamuons)
	@echo "------> (anamuons.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) anamuons -all_sources $${args}
endif

#$(anamuons_dependencies)

-include $(bin)anamuons_dependencies.make

#-- end of dependency -------------------
#-- start of cpp ------

$(bin)$(binobj)anamuons.o : $(anamuons_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)anamuons.o $(use_pp_cppflags) $(anamuons_pp_cppflags) $(app_anamuons_pp_cppflags) $(anamuons_pp_cppflags) $(use_cppflags) $(anamuons_cppflags) $(app_anamuons_cppflags) $(anamuons_cppflags) $(anamuons_cc_cppflags)  $(src)anamuons.cc

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: anamuonsclean
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




anamuonsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	@echo "------> (anamuons.make) Removing application files"
	-$(cleanup_echo) anamuons${application_suffix}
	-@cd $(bin); /bin/rm -f anamuons${application_suffix}
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)anamuons.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf anamuons_deps anamuons_dependencies.make
#-- end of cleanup_objects ------
