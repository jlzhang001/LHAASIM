#-- start of make_header -----------------

#====================================
#  Library Event
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

cmt_Event_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Event_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_Event

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Event = /tmp/CMT_$(LhaaSim_tag)_Event.make$(cmt_lock_pid)
else
cmt_local_tagfile_Event = $(LhaaSim_tag)_Event.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Event = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_Event = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_Event)

ifdef cmt_Event_has_target_tag

ifdef READONLY
cmt_final_setup_Event = /tmp/CMT_LhaaSim_Eventsetup.make
cmt_local_Event_makefile = /tmp/CMT_Event$(cmt_lock_pid).make
else
cmt_final_setup_Event = $(bin)LhaaSim_Eventsetup.make
cmt_local_Event_makefile = $(bin)Event.make
endif

else

ifdef READONLY
cmt_final_setup_Event = /tmp/CMT_LhaaSimsetup.make
cmt_local_Event_makefile = /tmp/CMT_Event$(cmt_lock_pid).make
else
cmt_final_setup_Event = $(bin)LhaaSimsetup.make
cmt_local_Event_makefile = $(bin)Event.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

Event ::


ifdef READONLY
Event ::
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
	@echo 'Event'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = Event/
Event::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

Eventlibname   = $(bin)$(library_prefix)Event$(library_suffix)
Eventlib       = $(Eventlibname).a
Eventstamp     = $(bin)Event.stamp
Eventshstamp   = $(bin)Event.shstamp

Event :: dirs  EventLIB
	@/bin/echo "------> Event ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

EventLIB :: $(Eventlib) $(Eventshstamp)
	@/bin/echo "------> Event : library ok"

$(Eventlib) :: $(bin)Event.o $(bin)Station.o $(bin)Array.o $(bin)Particle.o $(bin)EventDict.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(Eventlib) $?
	$(lib_silent) $(ranlib) $(Eventlib)
	$(lib_silent) cat /dev/null >$(Eventstamp)

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

$(Eventlibname).$(shlibsuffix) :: $(Eventlib) requirements $(use_requirements) $(Eventstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" Event $(Event_shlibflags)

$(Eventshstamp) :: $(Eventlibname).$(shlibsuffix)
	@if test -f $(Eventlibname).$(shlibsuffix) ; then cat /dev/null >$(Eventshstamp) ; fi

Eventclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)Event.o $(bin)Station.o $(bin)Array.o $(bin)Particle.o $(bin)EventDict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf Event_deps Event_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
Eventinstallname = $(library_prefix)Event$(library_suffix).$(shlibsuffix)

Event :: Eventinstall

install :: Eventinstall

Eventinstall :: $(install_dir)/$(Eventinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(Eventinstallname) :: $(bin)$(Eventinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Eventinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi

##Eventclean :: Eventuninstall

uninstall :: Eventuninstall

Eventuninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Eventinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi


#-- end of libary -----------------------
#-- start of dependency ------------------

$(bin)Event_dependencies.make :: dirs

ifndef QUICK
$(bin)Event_dependencies.make :: $(src)Event.cc $(src)Station.cc $(src)Array.cc $(src)Particle.cc $(src)EventDict.cc  requirements $(use_requirements) $(cmt_final_setup_Event)
	@echo "------> (Event.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) Event -all_sources $${args}
endif

#$(Event_dependencies)

-include $(bin)Event_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Event.o : $(Event_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Event.o $(use_pp_cppflags) $(Event_pp_cppflags) $(lib_Event_pp_cppflags) $(Event_pp_cppflags) $(use_cppflags) $(Event_cppflags) $(lib_Event_cppflags) $(Event_cppflags) $(Event_cc_cppflags)  $(src)Event.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Station.o : $(Station_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Station.o $(use_pp_cppflags) $(Event_pp_cppflags) $(lib_Event_pp_cppflags) $(Station_pp_cppflags) $(use_cppflags) $(Event_cppflags) $(lib_Event_cppflags) $(Station_cppflags) $(Station_cc_cppflags)  $(src)Station.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Array.o : $(Array_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Array.o $(use_pp_cppflags) $(Event_pp_cppflags) $(lib_Event_pp_cppflags) $(Array_pp_cppflags) $(use_cppflags) $(Event_cppflags) $(lib_Event_cppflags) $(Array_cppflags) $(Array_cc_cppflags)  $(src)Array.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Particle.o : $(Particle_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Particle.o $(use_pp_cppflags) $(Event_pp_cppflags) $(lib_Event_pp_cppflags) $(Particle_pp_cppflags) $(use_cppflags) $(Event_cppflags) $(lib_Event_cppflags) $(Particle_cppflags) $(Particle_cc_cppflags)  $(src)Particle.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)EventDict.o : $(EventDict_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)EventDict.o $(use_pp_cppflags) $(Event_pp_cppflags) $(lib_Event_pp_cppflags) $(EventDict_pp_cppflags) $(use_cppflags) $(Event_cppflags) $(lib_Event_cppflags) $(EventDict_cppflags) $(EventDict_cc_cppflags)  $(src)EventDict.cc

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: Eventclean
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




Eventclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)Event$(library_suffix).a $(binobj)$(library_prefix)Event$(library_suffix).s? $(binobj)Event.stamp $(binobj)Event.shstamp
#-- end of cleanup_library ---------------

