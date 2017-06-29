
include $(CMTROOT)/src/Makefile.header

#====================================
#  Library IOAires.cc
#
#   Generated Tue Nov  5 13:01:49 2013  by martraire
#
#====================================

help ::
	@echo 'IOAires.cc'

IOAires.cclib       = $(bin)libIOAires.cc.a
IOAires.ccstamp     = $(bin)IOAires.cc.stamp
IOAires.ccshstamp   = $(bin)IOAires.cc.shstamp

IOAires.cc :: dirs  IOAires.ccLIB
	@/bin/echo "------> IOAires.cc ok"


IOAires.ccLIB :: $(IOAires.cclib) $(IOAires.ccshstamp)
	@/bin/echo "------> IOAires.cc : library ok"

$(IOAires.cclib) :: $(bin)HeaderAires_ROOT.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); $(ar) $(IOAires.cclib) $?
	$(lib_silent) $(ranlib) $(IOAires.cclib)
	$(lib_silent) cat /dev/null >$(IOAires.ccstamp)

$(IOAires.ccshstamp) :: $(IOAires.cclib)
	$(lib_silent) cd $(bin); $(make_shlib) $(tag) IOAires.cc $(IOAires.cc_shlibflags)
	@if test -f $(bin)/libIOAires.cc.$(shlibsuffix) ; then cat /dev/null >$(IOAires.ccshstamp) ; fi

IOAires.ccclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)HeaderAires_ROOT.o




$(bin)IOAires.cc_dependencies.make : $(src)HeaderAires_ROOT.cc cmt_path.make requirements $(use_requirements)
	@echo "Rebuilding $@"; \
           args=`echo $? | sed -e "s#cmt_path.make.*##"`; \
           $(build_dependencies) IOAires.cc $${args}

-include $(bin)IOAires.cc_dependencies.make


$(bin)HeaderAires_ROOT.o : $(HeaderAires_ROOT_cc_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o HeaderAires_ROOT.o $(use_pp_cppflags) $(IOAires.cc_pp_cppflags) $(lib_IOAires.cc_pp_cppflags) $(HeaderAires_ROOT_pp_cppflags) $(use_cppflags) $(IOAires.cc_cppflags) $(lib_IOAires.cc_cppflags) $(HeaderAires_ROOT_cppflags) $(HeaderAires_ROOT_cc_cppflags)  $(src)HeaderAires_ROOT.cc
clean :: IOAires.ccclean
	@cd .

IOAires.ccclean ::
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f libIOAires.cc.a libIOAires.cc.s? IOAires.cc.stamp IOAires.cc.shstamp
