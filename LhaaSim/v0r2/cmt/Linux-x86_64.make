CMTPATH=/home/martraire:/home/martraire/Work/LHAASIM:/home/martraire/CDAS/trunk:/home/martraire/CDAS/trunk/IoFd
CMT_tag=$(tag)
CMTROOT=/home/martraire/CMT/v1r18p20060606
CMT_root=/home/martraire/CMT/v1r18p20060606
CMTVERSION=v1r18p20060606
CMT_cmtpath=/home/martraire
CMT_project=home
CMT_project_release=martraire
cmt_hardware_query_command=uname -m
cmt_hardware=`$(cmt_hardware_query_command)`
cmt_system_version_query_command=${CMTROOT}/mgr/cmt_linux_version.sh | ${CMTROOT}/mgr/cmt_filter_version.sh
cmt_system_version=`$(cmt_system_version_query_command)`
cmt_compiler_version_query_command=${CMTROOT}/mgr/cmt_gcc_version.sh | ${CMTROOT}/mgr/cmt_filter3_version.sh
cmt_compiler_version=`$(cmt_compiler_version_query_command)`
PATH=/home/martraire/CMT/v1r18p20060606/${CMTBIN}:/home/martraire/bin:/home/martraire/.local/bin:/CERN/root/bin:/CERN/root/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
CLASSPATH=/home/martraire/CMT/v1r18p20060606/java
debug_option=-g
cc=gcc
cflags= -fPIC -D__LINUX64__ 
ccomp=$(cc) -c $(includes) $(cdebugflags) $(cflags) $(pp_cflags)
clink=$(cc) $(clinkflags) $(cdebugflags)
ppcmd=-I
preproc=c++ -MD -c 
cpp=c++
cppflags=-pipe   -W -Wall -Wwrite-strings -Wpointer-arith   -Wall $(shell ${ROOTBIN}/root-config --cflags) -DROOT_${ROOTVERSION} -Wno-non-virtual-dtor  -Wno-deprecated -Wno-long-long  -DOPTIMIZED_VERSION -O  -fPIC -D__LINUX64__ 
pp_cppflags=-D_GNU_SOURCE
cppcomp=$(cpp) -c $(includes) $(cppdebugflags) $(cppflags) $(pp_cppflags)
cpplink=$(cpp) $(cpplinkflags) $(cppdebugflags)
for=g77
fflags=$(debug_option)
fcomp=$(for) -c $(fincludes) $(fflags) $(pp_fflags)
flink=$(for) $(flinkflags)
javacomp=javac -classpath $(src):$(CLASSPATH) 
javacopy=cp
jar=jar
X11_cflags=-I/usr/include
Xm_cflags=-I/usr/include
X_linkopts=-L/usr/X11R6/lib -lXm -lXt -lXext -lX11 -lm
lex=lex $(lexflags)
yaccflags= -l -d 
yacc=yacc $(yaccflags)
ar=ar r
ranlib=ranlib
make_shlib=${CMTROOT}/mgr/cmt_make_shlib_common.sh extract
shlibsuffix=so
shlibbuilder=g++ $(cmt_installarea_linkopts) 
shlibflags=-shared
symlink=/bin/ln -fs 
symunlink=/bin/rm -f 
library_install_command=${symlink}
build_library_links=$(cmtexe) build library_links -quiet -tag=$(tags)
remove_library_links=$(cmtexe) remove library_links -quiet -tag=$(tags)
cmtexe=${CMTROOT}/${CMTBIN}/cmt.exe
build_prototype=$(cmtexe) build prototype
build_dependencies=$(cmtexe) -quiet -tag=$(tags) build dependencies
build_triggers=$(cmtexe) build triggers
implied_library_prefix=-l
SHELL=/bin/sh
q="
src=../src/
doc=../doc/
inc=../src/
mgr=../cmt/
application_suffix=.exe
library_prefix=lib
lock_command=chmod -R a-w ../*
unlock_command=chmod -R g+w ../*
MAKEFLAGS= --no-print-directory 
gmake_hosts=lx1 rsplus lxtest as7 dxplus ax7 hp2 aleph hp1 hpplus papou1-fe atlas
make_hosts=virgo-control1 rio0a vmpc38a
everywhere=hosts
install_command=cp 
uninstall_command=/bin/rm -f 
cmt_installarea_command=ln -fs 
cmt_uninstallarea_command=/bin/rm -f 
cmt_install_area_command=$(cmt_installarea_command)
cmt_uninstall_area_command=$(cmt_uninstallarea_command)
cmt_install_action=$(CMTROOT)/mgr/cmt_install_action.sh
cmt_installdir_action=$(CMTROOT)/mgr/cmt_installdir_action.sh
cmt_uninstall_action=$(CMTROOT)/mgr/cmt_uninstall_action.sh
cmt_uninstalldir_action=$(CMTROOT)/mgr/cmt_uninstalldir_action.sh
mkdir=mkdir
cmt_cvs_protocol_level=v1r1
cmt_installarea_prefix=InstallArea
CMT_PATH_remove_regexp=/[^/]*/
CMT_PATH_remove_share_regexp=/share/
NEWCMTCONFIG=x86_64-debianstretch/sid-/src/configure
LhaaSim_tag=$(tag)
LHAASIMROOT=/home/martraire/Work/LHAASIM/LhaaSim/v0r2
LhaaSim_root=/home/martraire/Work/LHAASIM/LhaaSim/v0r2
LHAASIMVERSION=v0r2
LhaaSim_cmtpath=/home/martraire
LhaaSim_offset=Work/LHAASIM
LhaaSim_project=home
LhaaSim_project_release=martraire
Convert_tag=$(tag)
CONVERTROOT=/home/martraire/Work/LHAASIM/Convert/v0r2
Convert_root=/home/martraire/Work/LHAASIM/Convert/v0r2
CONVERTVERSION=v0r2
Convert_cmtpath=/home/martraire
Convert_offset=Work/LHAASIM
Convert_project=home
Convert_project_release=martraire
RootW_tag=$(tag)
ROOTWROOT=/home/martraire/CDAS/trunk/RootW/v2r8
RootW_root=/home/martraire/CDAS/trunk/RootW/v2r8
ROOTWVERSION=v2r8
RootW_cmtpath=/home/martraire
RootW_offset=CDAS/trunk
RootW_project=home
RootW_project_release=martraire
ROOTBIN=${ROOTSYS}/bin
ROOTEXEC=/bin
ROOTLIBS=$(shell ${ROOTBIN}/root-config --libs)
ROOTGLIBS=$(shell ${ROOTBIN}/root-config --glibs)
ROOTVERSION=$(shell ${ROOTBIN}/root-config --version | sed 's/[\./]/_/g')
RootW_linkopts= -L$(shell ${ROOTBIN}/root-config --libdir) ${ROOTLIBS}  -Wl,-rpath $(shell ${ROOTBIN}/root-config --libdir) 
ROOTSYS=$(shell ${ROOTWROOT}/cmt/evalrootsys.sh)
Convert_linkopts= -L$(CONVERTROOT)/$(Convert_tag) -lShowerROOT -lShowerIO -lUtils -Wl,--no-as-needed -Wl,-rpath $(CONVERTROOT)/$(Convert_tag)  -lgfortran 
Convert_cppflags=-DBETA_VERSION -DVERSION="0" -DREVISION="0"
LhaaSim_cppflags= -g 
LhaaSim_linkopts=-L$(LHAASIMROOT)/$(LhaaSim_tag) -Wl,--no-as-needed -Wl,-rpath $(LHAASIMROOT)/$(LhaaSim_tag)  -lEvent -lTools -lConfig  -lAna 
tag=Linux-x86_64
package=LhaaSim
version=v0r2
PACKAGE_ROOT=$(LHAASIMROOT)
srcdir=../src
bin=../$(LhaaSim_tag)/
javabin=../classes/
mgrdir=cmt
project=home
cmt_installarea_paths= $(cmt_installarea_prefix)/$(CMTCONFIG)/bin
use_linkopts= $(cmt_installarea_linkopts)   $(LhaaSim_linkopts)  $(Convert_linkopts)  $(RootW_linkopts) 
trunk_installarea_prefix=$(cmt_installarea_prefix)
trunk_installarea_prefix_remove=$(trunk_installarea_prefix)
LD_LIBRARY_PATH=/CERN/root/lib
CDAS_installarea_prefix=$(cmt_installarea_prefix)
CDAS_installarea_prefix_remove=$(CDAS_installarea_prefix)
Work_installarea_prefix=$(cmt_installarea_prefix)
Work_installarea_prefix_remove=$(Work_installarea_prefix)
home_installarea_prefix=$(cmt_installarea_prefix)
home_installarea_prefix_remove=$(home_installarea_prefix)
use_requirements=requirements $(CMTROOT)/mgr/requirements $(CONVERTROOT)/cmt/requirements $(ROOTWROOT)/cmt/requirements 
use_includes= $(ppcmd)"$(Convert_root)/src" $(ppcmd)"$(RootW_root)/src" 
use_fincludes= $(use_includes)
use_stamps= $(LhaaSim_stamps)  $(Convert_stamps)  $(RootW_stamps) 
use_cflags=  $(LhaaSim_cflags)  $(Convert_cflags)  $(RootW_cflags) 
use_pp_cflags=  $(LhaaSim_pp_cflags)  $(Convert_pp_cflags)  $(RootW_pp_cflags) 
use_cppflags=  $(LhaaSim_cppflags)  $(Convert_cppflags)  $(RootW_cppflags) 
use_pp_cppflags=  $(LhaaSim_pp_cppflags)  $(Convert_pp_cppflags)  $(RootW_pp_cppflags) 
use_fflags=  $(LhaaSim_fflags)  $(Convert_fflags)  $(RootW_fflags) 
use_pp_fflags=  $(LhaaSim_pp_fflags)  $(Convert_pp_fflags)  $(RootW_pp_fflags) 
use_libraries= $(Convert_libraries)  $(RootW_libraries) 
includes= $(ppcmd)"$(srcdir)" $(use_includes)
fincludes= $(includes)
Event_use_linkopts=  $(LhaaSim_linkopts)  $(Convert_linkopts)  $(RootW_linkopts) 
Tools_use_linkopts=  $(LhaaSim_linkopts)  $(Convert_linkopts)  $(RootW_linkopts) 
Config_use_linkopts=  $(LhaaSim_linkopts)  $(Convert_linkopts)  $(RootW_linkopts) 
Ana_use_linkopts=  $(LhaaSim_linkopts)  $(Convert_linkopts)  $(RootW_linkopts) 
LhaaSim_use_linkopts=  $(LhaaSim_linkopts)  $(Convert_linkopts)  $(RootW_linkopts) 
AnaEvent_use_linkopts=  $(LhaaSim_linkopts)  $(Convert_linkopts)  $(RootW_linkopts) 
anamuons_use_linkopts=  $(LhaaSim_linkopts)  $(Convert_linkopts)  $(RootW_linkopts) 
constituents= EventDict AnaDict Event Tools Config Ana LhaaSim AnaEvent anamuons 
all_constituents= $(constituents)
constituentsclean= anamuonsclean AnaEventclean LhaaSimclean Anaclean Configclean Toolsclean Eventclean AnaDictclean EventDictclean 
all_constituentsclean= $(constituentsclean)
cmt_actions_constituents= make 
cmt_actions_constituentsclean= makeclean 
