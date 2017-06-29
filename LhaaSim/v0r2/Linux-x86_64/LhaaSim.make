#-- start of make_header -----------------

#====================================
#  Application LhaaSim
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

cmt_LhaaSim_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_LhaaSim_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_LhaaSim

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_LhaaSim = /tmp/CMT_$(LhaaSim_tag)_LhaaSim.make$(cmt_lock_pid)
else
cmt_local_tagfile_LhaaSim = $(LhaaSim_tag)_LhaaSim.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_LhaaSim = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_LhaaSim = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_LhaaSim)

ifdef cmt_LhaaSim_has_target_tag

ifdef READONLY
cmt_final_setup_LhaaSim = /tmp/CMT_LhaaSim_LhaaSimsetup.make
cmt_local_LhaaSim_makefile = /tmp/CMT_LhaaSim$(cmt_lock_pid).make
else
cmt_final_setup_LhaaSim = $(bin)LhaaSim_LhaaSimsetup.make
cmt_local_LhaaSim_makefile = $(bin)LhaaSim.make
endif

else

ifdef READONLY
cmt_final_setup_LhaaSim = /tmp/CMT_LhaaSimsetup.make
cmt_local_LhaaSim_makefile = /tmp/CMT_LhaaSim$(cmt_lock_pid).make
else
cmt_final_setup_LhaaSim = $(bin)LhaaSimsetup.make
cmt_local_LhaaSim_makefile = $(bin)LhaaSim.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

LhaaSim ::


ifdef READONLY
LhaaSim ::
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
	@echo 'LhaaSim'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = LhaaSim/
LhaaSim::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

LhaaSim :: dirs  $(bin)LhaaSim${application_suffix}
	@/bin/echo "------> LhaaSim ok"

#-- end of application_header
#-- start of application

$(bin)LhaaSim${application_suffix} :: $(bin)LhaaSim.o $(use_stamps) $(LhaaSimstamps) requirements $(use_requirements)
	$(link_echo) $@
	$(link_silent) cd $(bin); $(cpplink) -o LhaaSim.exe.new $(bin)LhaaSim.o $(use_linkopts) $(LhaaSimlinkopts); mv -f LhaaSim.exe.new LhaaSim${application_suffix}

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
LhaaSiminstallname = LhaaSim${application_suffix}

LhaaSim :: LhaaSiminstall

install :: LhaaSiminstall

LhaaSiminstall :: $(install_dir)/$(LhaaSiminstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(LhaaSiminstallname) :: $(bin)$(LhaaSiminstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(LhaaSiminstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

##LhaaSimclean :: LhaaSimuninstall

uninstall :: LhaaSimuninstall

LhaaSimuninstall ::
	@echo "------> (LhaaSim.make) Removing installed files"
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(LhaaSiminstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"; \
	fi

#-- end of application
#-- start of dependency ------------------

$(bin)LhaaSim_dependencies.make :: dirs

ifndef QUICK
$(bin)LhaaSim_dependencies.make :: $(src)LhaaSim.cc  requirements $(use_requirements) $(cmt_final_setup_LhaaSim)
	@echo "------> (LhaaSim.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) LhaaSim -all_sources $${args}
endif

#$(LhaaSim_dependencies)

-include $(bin)LhaaSim_dependencies.make

#-- end of dependency -------------------
#-- start of cpp ------

$(bin)$(binobj)LhaaSim.o : $(LhaaSim_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)LhaaSim.o $(use_pp_cppflags) $(LhaaSim_pp_cppflags) $(app_LhaaSim_pp_cppflags) $(LhaaSim_pp_cppflags) $(use_cppflags) $(LhaaSim_cppflags) $(app_LhaaSim_cppflags) $(LhaaSim_cppflags) $(LhaaSim_cc_cppflags)  $(src)LhaaSim.cc

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: LhaaSimclean
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




LhaaSimclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	@echo "------> (LhaaSim.make) Removing application files"
	-$(cleanup_echo) LhaaSim${application_suffix}
	-@cd $(bin); /bin/rm -f LhaaSim${application_suffix}
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)LhaaSim.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf LhaaSim_deps LhaaSim_dependencies.make
#-- end of cleanup_objects ------
