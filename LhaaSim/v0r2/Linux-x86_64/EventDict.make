#-- start of make_header -----------------

#====================================
#  Document EventDict
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

cmt_EventDict_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_EventDict_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_EventDict

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_EventDict = /tmp/CMT_$(LhaaSim_tag)_EventDict.make$(cmt_lock_pid)
else
cmt_local_tagfile_EventDict = $(LhaaSim_tag)_EventDict.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LhaaSim_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_EventDict = /tmp/CMT_$(LhaaSim_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_EventDict = $(LhaaSim_tag).make
endif

endif

-include $(cmt_local_tagfile_EventDict)

ifdef cmt_EventDict_has_target_tag

ifdef READONLY
cmt_final_setup_EventDict = /tmp/CMT_LhaaSim_EventDictsetup.make
cmt_local_EventDict_makefile = /tmp/CMT_EventDict$(cmt_lock_pid).make
else
cmt_final_setup_EventDict = $(bin)LhaaSim_EventDictsetup.make
cmt_local_EventDict_makefile = $(bin)EventDict.make
endif

else

ifdef READONLY
cmt_final_setup_EventDict = /tmp/CMT_LhaaSimsetup.make
cmt_local_EventDict_makefile = /tmp/CMT_EventDict$(cmt_lock_pid).make
else
cmt_final_setup_EventDict = $(bin)LhaaSimsetup.make
cmt_local_EventDict_makefile = $(bin)EventDict.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LhaaSimsetup.make
else
cmt_final_setup = $(bin)LhaaSimsetup.make
endif

EventDict ::


ifdef READONLY
EventDict ::
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
	@echo 'EventDict'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = EventDict/
EventDict::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-------------------------------------------

src     = ../src/

EventDict_output = $(src)
  
EventDict :: $(src)/EventDict

$(src)/EventDict :: $(EventDict_output)Event.h $(EventDict_output)Station.h $(EventDict_output)Array.h $(EventDict_output)Particle.h $(EventDict_output)LhaaSimLinkDef.h
	cd $(src); ${ROOTBIN}/rootcint -f EventDict.cc -c ${CINTOPT} ${CINT_use_includes} $(EventDict_output)Event.h $(EventDict_output)Station.h $(EventDict_output)Array.h $(EventDict_output)Particle.h $(EventDict_output)LhaaSimLinkDef.h; touch EventDict
	@/bin/echo "---> EventDict ok"
		 
EventDictclean ::
	/bin/rm -f .rootsys
	cd $(src); /bin/rm -f EventDict EventDict.h; echo >EventDict.cc
 
#-------------------------------------------                                    
Event_h_dependencies = ../src/Event.h
Station_h_dependencies = ../src/Station.h
Array_h_dependencies = ../src/Array.h
Particle_h_dependencies = ../src/Particle.h
LhaaSimLinkDef_h_dependencies = ../src/LhaaSimLinkDef.h
#-- start of cleanup_header --------------

clean :: EventDictclean
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




EventDictclean ::
#-- end of cleanup_header ---------------
