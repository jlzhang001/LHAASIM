#-- start of make_header -----------------

#====================================
#  Library Config
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

cmt_Config_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Config_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_Config

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Config = /tmp/CMT_$(LhaaSim_tag)_Config.make$(cmt_lock_pid)
else
cmt_local_tagfile_Config = $(LhaaSim_tag)_Config.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Config = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_Config = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_Config)

ifdef cmt_Config_has_target_tag

ifdef READONLY
cmt_final_setup_Config = /tmp/CMT_LhaaSim_Configsetup.make
cmt_local_Config_makefile = /tmp/CMT_Config$(cmt_lock_pid).make
else
cmt_final_setup_Config = $(bin)LhaaSim_Configsetup.make
cmt_local_Config_makefile = $(bin)Config.make
endif

else

ifdef READONLY
cmt_final_setup_Config = /tmp/CMT_LhaaSimsetup.make
cmt_local_Config_makefile = /tmp/CMT_Config$(cmt_lock_pid).make
else
cmt_final_setup_Config = $(bin)LhaaSimsetup.make
cmt_local_Config_makefile = $(bin)Config.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

Config ::


ifdef READONLY
Config ::
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
	@echo 'Config'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = Config/
Config::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

Configlibname   = $(bin)$(library_prefix)Config$(library_suffix)
Configlib       = $(Configlibname).a
Configstamp     = $(bin)Config.stamp
Configshstamp   = $(bin)Config.shstamp

Config :: dirs  ConfigLIB
	@/bin/echo "------> Config ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

ConfigLIB :: $(Configlib) $(Configshstamp)
	@/bin/echo "------> Config : library ok"

$(Configlib) :: $(bin)LhaaSimConfig.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(Configlib) $?
	$(lib_silent) $(ranlib) $(Configlib)
	$(lib_silent) cat /dev/null >$(Configstamp)

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

$(Configlibname).$(shlibsuffix) :: $(Configlib) requirements $(use_requirements) $(Configstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" Config $(Config_shlibflags)

$(Configshstamp) :: $(Configlibname).$(shlibsuffix)
	@if test -f $(Configlibname).$(shlibsuffix) ; then cat /dev/null >$(Configshstamp) ; fi

Configclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)LhaaSimConfig.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf Config_deps Config_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
Configinstallname = $(library_prefix)Config$(library_suffix).$(shlibsuffix)

Config :: Configinstall

install :: Configinstall

Configinstall :: $(install_dir)/$(Configinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(Configinstallname) :: $(bin)$(Configinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Configinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi

##Configclean :: Configuninstall

uninstall :: Configuninstall

Configuninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Configinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi


#-- end of libary -----------------------
#-- start of dependency ------------------

$(bin)Config_dependencies.make :: dirs

ifndef QUICK
$(bin)Config_dependencies.make :: $(src)LhaaSimConfig.cc  requirements $(use_requirements) $(cmt_final_setup_Config)
	@echo "------> (Config.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) Config -all_sources $${args}
endif

#$(Config_dependencies)

-include $(bin)Config_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)LhaaSimConfig.o : $(LhaaSimConfig_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)LhaaSimConfig.o $(use_pp_cppflags) $(Config_pp_cppflags) $(lib_Config_pp_cppflags) $(LhaaSimConfig_pp_cppflags) $(use_cppflags) $(Config_cppflags) $(lib_Config_cppflags) $(LhaaSimConfig_cppflags) $(LhaaSimConfig_cc_cppflags)  $(src)LhaaSimConfig.cc

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: Configclean
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




Configclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)Config$(library_suffix).a $(binobj)$(library_prefix)Config$(library_suffix).s? $(binobj)Config.stamp $(binobj)Config.shstamp
#-- end of cleanup_library ---------------

