#-- start of make_header -----------------

#====================================
#  Document ShowerROOTDict
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

cmt_ShowerROOTDict_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_ShowerROOTDict_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_ShowerROOTDict

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_ShowerROOTDict = /tmp/CMT_$(Convert_tag)_ShowerROOTDict.make$(cmt_lock_pid)
else
cmt_local_tagfile_ShowerROOTDict = $(Convert_tag)_ShowerROOTDict.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

Convert_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_ShowerROOTDict = /tmp/CMT_$(Convert_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_ShowerROOTDict = $(Convert_tag).make
endif

endif

-include $(cmt_local_tagfile_ShowerROOTDict)

ifdef cmt_ShowerROOTDict_has_target_tag

ifdef READONLY
cmt_final_setup_ShowerROOTDict = /tmp/CMT_Convert_ShowerROOTDictsetup.make
cmt_local_ShowerROOTDict_makefile = /tmp/CMT_ShowerROOTDict$(cmt_lock_pid).make
else
cmt_final_setup_ShowerROOTDict = $(bin)Convert_ShowerROOTDictsetup.make
cmt_local_ShowerROOTDict_makefile = $(bin)ShowerROOTDict.make
endif

else

ifdef READONLY
cmt_final_setup_ShowerROOTDict = /tmp/CMT_Convertsetup.make
cmt_local_ShowerROOTDict_makefile = /tmp/CMT_ShowerROOTDict$(cmt_lock_pid).make
else
cmt_final_setup_ShowerROOTDict = $(bin)Convertsetup.make
cmt_local_ShowerROOTDict_makefile = $(bin)ShowerROOTDict.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_Convertsetup.make
else
cmt_final_setup = $(bin)Convertsetup.make
endif

ShowerROOTDict ::


ifdef READONLY
ShowerROOTDict ::
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
	@echo 'ShowerROOTDict'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = ShowerROOTDict/
ShowerROOTDict::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-------------------------------------------

src     = ../src/

ShowerROOTDict_output = $(src)
  
ShowerROOTDict :: $(src)/ShowerROOTDict

$(src)/ShowerROOTDict :: $(ShowerROOTDict_output)Shower_ROOT.h $(ShowerROOTDict_output)ShowerGrnd_ROOT.h $(ShowerROOTDict_output)ShowerProf_ROOT.h $(ShowerROOTDict_output)HeaderCorsika_ROOT.h $(ShowerROOTDict_output)PartGrnd_ROOT.h $(ShowerROOTDict_output)ShowerROOTLinkDef.h
	cd $(src); ${ROOTBIN}/rootcint -f ShowerROOTDict.cc -c ${CINTOPT} ${CINT_use_includes} $(ShowerROOTDict_output)Shower_ROOT.h $(ShowerROOTDict_output)ShowerGrnd_ROOT.h $(ShowerROOTDict_output)ShowerProf_ROOT.h $(ShowerROOTDict_output)HeaderCorsika_ROOT.h $(ShowerROOTDict_output)PartGrnd_ROOT.h $(ShowerROOTDict_output)ShowerROOTLinkDef.h; touch ShowerROOTDict
	@/bin/echo "---> ShowerROOTDict ok"
		 
ShowerROOTDictclean ::
	/bin/rm -f .rootsys
	cd $(src); /bin/rm -f ShowerROOTDict ShowerROOTDict.h; echo >ShowerROOTDict.cc
 
#-------------------------------------------                                    
Shower_ROOT_h_dependencies = ../src/Shower_ROOT.h
ShowerGrnd_ROOT_h_dependencies = ../src/ShowerGrnd_ROOT.h
ShowerProf_ROOT_h_dependencies = ../src/ShowerProf_ROOT.h
HeaderCorsika_ROOT_h_dependencies = ../src/HeaderCorsika_ROOT.h
PartGrnd_ROOT_h_dependencies = ../src/PartGrnd_ROOT.h
ShowerROOTLinkDef_h_dependencies = ../src/ShowerROOTLinkDef.h
#-- start of cleanup_header --------------

clean :: ShowerROOTDictclean
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




ShowerROOTDictclean ::
#-- end of cleanup_header ---------------
