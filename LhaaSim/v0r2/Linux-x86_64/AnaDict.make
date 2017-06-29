#-- start of make_header -----------------

#====================================
#  Document AnaDict
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

cmt_AnaDict_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_AnaDict_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_AnaDict

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_AnaDict = /tmp/CMT_$(LhaaSim_tag)_AnaDict.make$(cmt_lock_pid)
else
cmt_local_tagfile_AnaDict = $(LhaaSim_tag)_AnaDict.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_AnaDict = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_AnaDict = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_AnaDict)

ifdef cmt_AnaDict_has_target_tag

ifdef READONLY
cmt_final_setup_AnaDict = /tmp/CMT_LhaaSim_AnaDictsetup.make
cmt_local_AnaDict_makefile = /tmp/CMT_AnaDict$(cmt_lock_pid).make
else
cmt_final_setup_AnaDict = $(bin)LhaaSim_AnaDictsetup.make
cmt_local_AnaDict_makefile = $(bin)AnaDict.make
endif

else

ifdef READONLY
cmt_final_setup_AnaDict = /tmp/CMT_LhaaSimsetup.make
cmt_local_AnaDict_makefile = /tmp/CMT_AnaDict$(cmt_lock_pid).make
else
cmt_final_setup_AnaDict = $(bin)LhaaSimsetup.make
cmt_local_AnaDict_makefile = $(bin)AnaDict.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

AnaDict ::


ifdef READONLY
AnaDict ::
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
	@echo 'AnaDict'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = AnaDict/
AnaDict::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-------------------------------------------

src     = ../src/

AnaDict_output = $(src)
  
AnaDict :: $(src)/AnaDict

$(src)/AnaDict :: $(AnaDict_output)Analyze.h $(AnaDict_output)AnaLinkDef.h
	cd $(src); ${ROOTBIN}/rootcint -f AnaDict.cc -c ${CINTOPT} ${CINT_use_includes} $(AnaDict_output)Analyze.h $(AnaDict_output)AnaLinkDef.h; touch AnaDict
	@/bin/echo "---> AnaDict ok"
		 
AnaDictclean ::
	/bin/rm -f .rootsys
	cd $(src); /bin/rm -f AnaDict AnaDict.h; echo >AnaDict.cc
 
#-------------------------------------------                                    
Analyze_h_dependencies = ../src/Analyze.h
AnaLinkDef_h_dependencies = ../src/AnaLinkDef.h
#-- start of cleanup_header --------------

clean :: AnaDictclean
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




AnaDictclean ::
#-- end of cleanup_header ---------------
