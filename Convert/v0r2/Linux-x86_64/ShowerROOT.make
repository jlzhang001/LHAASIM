#-- start of make_header -----------------

#====================================
#  Library ShowerROOT
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

cmt_ShowerROOT_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_ShowerROOT_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_ShowerROOT

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_ShowerROOT = /tmp/CMT_$(Convert_tag)_ShowerROOT.make$(cmt_lock_pid)
else
cmt_local_tagfile_ShowerROOT = $(Convert_tag)_ShowerROOT.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_ShowerROOT = /tmp/CMT_$(Convert_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_ShowerROOT = $(Convert_tag).make
endif

endif

-include $(cmt_local_tagfile_ShowerROOT)

ifdef cmt_ShowerROOT_has_target_tag

ifdef READONLY
cmt_final_setup_ShowerROOT = /tmp/CMT_Convert_ShowerROOTsetup.make
cmt_local_ShowerROOT_makefile = /tmp/CMT_ShowerROOT$(cmt_lock_pid).make
else
cmt_final_setup_ShowerROOT = $(bin)Convert_ShowerROOTsetup.make
cmt_local_ShowerROOT_makefile = $(bin)ShowerROOT.make
endif

else

ifdef READONLY
cmt_final_setup_ShowerROOT = /tmp/CMT_Convertsetup.make
cmt_local_ShowerROOT_makefile = /tmp/CMT_ShowerROOT$(cmt_lock_pid).make
else
cmt_final_setup_ShowerROOT = $(bin)Convertsetup.make
cmt_local_ShowerROOT_makefile = $(bin)ShowerROOT.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_Convertsetup.make
else
cmt_final_setup = $(bin)Convertsetup.make
endif

ShowerROOT ::


ifdef READONLY
ShowerROOT ::
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
	@echo 'ShowerROOT'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = ShowerROOT/
ShowerROOT::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

ShowerROOTlibname   = $(bin)$(library_prefix)ShowerROOT$(library_suffix)
ShowerROOTlib       = $(ShowerROOTlibname).a
ShowerROOTstamp     = $(bin)ShowerROOT.stamp
ShowerROOTshstamp   = $(bin)ShowerROOT.shstamp

ShowerROOT :: dirs  ShowerROOTLIB
	@/bin/echo "------> ShowerROOT ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

ShowerROOTLIB :: $(ShowerROOTlib) $(ShowerROOTshstamp)
	@/bin/echo "------> ShowerROOT : library ok"

$(ShowerROOTlib) :: $(bin)Header_ROOT.o $(bin)HeaderCorsika_ROOT.o $(bin)PartGrnd_ROOT.o $(bin)Shower_ROOT.o $(bin)ShowerGrnd_ROOT.o $(bin)ShowerProf_ROOT.o $(bin)ShProperties.o $(bin)ShowerROOTDict.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(ShowerROOTlib) $?
	$(lib_silent) $(ranlib) $(ShowerROOTlib)
	$(lib_silent) cat /dev/null >$(ShowerROOTstamp)

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

$(ShowerROOTlibname).$(shlibsuffix) :: $(ShowerROOTlib) requirements $(use_requirements) $(ShowerROOTstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" ShowerROOT $(ShowerROOT_shlibflags)

$(ShowerROOTshstamp) :: $(ShowerROOTlibname).$(shlibsuffix)
	@if test -f $(ShowerROOTlibname).$(shlibsuffix) ; then cat /dev/null >$(ShowerROOTshstamp) ; fi

ShowerROOTclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)Header_ROOT.o $(bin)HeaderCorsika_ROOT.o $(bin)PartGrnd_ROOT.o $(bin)Shower_ROOT.o $(bin)ShowerGrnd_ROOT.o $(bin)ShowerProf_ROOT.o $(bin)ShProperties.o $(bin)ShowerROOTDict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf ShowerROOT_deps ShowerROOT_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
ShowerROOTinstallname = $(library_prefix)ShowerROOT$(library_suffix).$(shlibsuffix)

ShowerROOT :: ShowerROOTinstall

install :: ShowerROOTinstall

ShowerROOTinstall :: $(install_dir)/$(ShowerROOTinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(ShowerROOTinstallname) :: $(bin)$(ShowerROOTinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(ShowerROOTinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi

##ShowerROOTclean :: ShowerROOTuninstall

uninstall :: ShowerROOTuninstall

ShowerROOTuninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(ShowerROOTinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)" ; \
	fi


#-- end of libary -----------------------
#-- start of dependency ------------------

$(bin)ShowerROOT_dependencies.make :: dirs

ifndef QUICK
$(bin)ShowerROOT_dependencies.make :: $(src)Header_ROOT.cc $(src)HeaderCorsika_ROOT.cc $(src)PartGrnd_ROOT.cc $(src)Shower_ROOT.cc $(src)ShowerGrnd_ROOT.cc $(src)ShowerProf_ROOT.cc $(src)ShProperties.cc $(src)ShowerROOTDict.cc  requirements $(use_requirements) $(cmt_final_setup_ShowerROOT)
	@echo "------> (ShowerROOT.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) ShowerROOT -all_sources $${args}
endif

#$(ShowerROOT_dependencies)

-include $(bin)ShowerROOT_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Header_ROOT.o : $(Header_ROOT_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Header_ROOT.o $(use_pp_cppflags) $(ShowerROOT_pp_cppflags) $(lib_ShowerROOT_pp_cppflags) $(Header_ROOT_pp_cppflags) $(use_cppflags) $(ShowerROOT_cppflags) $(lib_ShowerROOT_cppflags) $(Header_ROOT_cppflags) $(Header_ROOT_cc_cppflags)  $(src)Header_ROOT.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)HeaderCorsika_ROOT.o : $(HeaderCorsika_ROOT_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)HeaderCorsika_ROOT.o $(use_pp_cppflags) $(ShowerROOT_pp_cppflags) $(lib_ShowerROOT_pp_cppflags) $(HeaderCorsika_ROOT_pp_cppflags) $(use_cppflags) $(ShowerROOT_cppflags) $(lib_ShowerROOT_cppflags) $(HeaderCorsika_ROOT_cppflags) $(HeaderCorsika_ROOT_cc_cppflags)  $(src)HeaderCorsika_ROOT.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)PartGrnd_ROOT.o : $(PartGrnd_ROOT_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)PartGrnd_ROOT.o $(use_pp_cppflags) $(ShowerROOT_pp_cppflags) $(lib_ShowerROOT_pp_cppflags) $(PartGrnd_ROOT_pp_cppflags) $(use_cppflags) $(ShowerROOT_cppflags) $(lib_ShowerROOT_cppflags) $(PartGrnd_ROOT_cppflags) $(PartGrnd_ROOT_cc_cppflags)  $(src)PartGrnd_ROOT.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)Shower_ROOT.o : $(Shower_ROOT_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)Shower_ROOT.o $(use_pp_cppflags) $(ShowerROOT_pp_cppflags) $(lib_ShowerROOT_pp_cppflags) $(Shower_ROOT_pp_cppflags) $(use_cppflags) $(ShowerROOT_cppflags) $(lib_ShowerROOT_cppflags) $(Shower_ROOT_cppflags) $(Shower_ROOT_cc_cppflags)  $(src)Shower_ROOT.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)ShowerGrnd_ROOT.o : $(ShowerGrnd_ROOT_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)ShowerGrnd_ROOT.o $(use_pp_cppflags) $(ShowerROOT_pp_cppflags) $(lib_ShowerROOT_pp_cppflags) $(ShowerGrnd_ROOT_pp_cppflags) $(use_cppflags) $(ShowerROOT_cppflags) $(lib_ShowerROOT_cppflags) $(ShowerGrnd_ROOT_cppflags) $(ShowerGrnd_ROOT_cc_cppflags)  $(src)ShowerGrnd_ROOT.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)ShowerProf_ROOT.o : $(ShowerProf_ROOT_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)ShowerProf_ROOT.o $(use_pp_cppflags) $(ShowerROOT_pp_cppflags) $(lib_ShowerROOT_pp_cppflags) $(ShowerProf_ROOT_pp_cppflags) $(use_cppflags) $(ShowerROOT_cppflags) $(lib_ShowerROOT_cppflags) $(ShowerProf_ROOT_cppflags) $(ShowerProf_ROOT_cc_cppflags)  $(src)ShowerProf_ROOT.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)ShProperties.o : $(ShProperties_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)ShProperties.o $(use_pp_cppflags) $(ShowerROOT_pp_cppflags) $(lib_ShowerROOT_pp_cppflags) $(ShProperties_pp_cppflags) $(use_cppflags) $(ShowerROOT_cppflags) $(lib_ShowerROOT_cppflags) $(ShProperties_cppflags) $(ShProperties_cc_cppflags)  $(src)ShProperties.cc

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)ShowerROOTDict.o : $(ShowerROOTDict_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)ShowerROOTDict.o $(use_pp_cppflags) $(ShowerROOT_pp_cppflags) $(lib_ShowerROOT_pp_cppflags) $(ShowerROOTDict_pp_cppflags) $(use_cppflags) $(ShowerROOT_cppflags) $(lib_ShowerROOT_cppflags) $(ShowerROOTDict_cppflags) $(ShowerROOTDict_cc_cppflags)  $(src)ShowerROOTDict.cc

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: ShowerROOTclean
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




ShowerROOTclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)ShowerROOT$(library_suffix).a $(binobj)$(library_prefix)ShowerROOT$(library_suffix).s? $(binobj)ShowerROOT.stamp $(binobj)ShowerROOT.shstamp
#-- end of cleanup_library ---------------

