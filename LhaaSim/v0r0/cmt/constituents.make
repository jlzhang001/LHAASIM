all_groups :: all

all :: $(all_constituents)
	@/bin/echo " all ok."

clean :: allclean
	@/bin/echo " clean ok."

allclean :: $(all_constituentsclean)
	@/bin/echo " allclean ok."

$(bin)EventDict.make :: dirs
	@$(cmtexe) build constituent_makefile EventDict -tag=$(tag); \
	  if test ! "$(bin)" = "./"; then /bin/mv -f EventDict.make $(bin); fi

EventDict :: $(EventDict_dependencies) $(bin)EventDict.make
	@echo " starting EventDict"
	@$(MAKE) -f $(bin)EventDict.make build_strategy=keep_makefiles EventDict

EventDictclean :: $(EventDictclean_dependencies) $(bin)EventDict.make
	@echo " starting EventDictclean"
	@$(MAKE) -f $(bin)EventDict.make build_strategy=keep_makefiles EventDictclean



$(bin)AnaDict.make :: dirs
	@$(cmtexe) build constituent_makefile AnaDict -tag=$(tag); \
	  if test ! "$(bin)" = "./"; then /bin/mv -f AnaDict.make $(bin); fi

AnaDict :: $(AnaDict_dependencies) $(bin)AnaDict.make
	@echo " starting AnaDict"
	@$(MAKE) -f $(bin)AnaDict.make build_strategy=keep_makefiles AnaDict

AnaDictclean :: $(AnaDictclean_dependencies) $(bin)AnaDict.make
	@echo " starting AnaDictclean"
	@$(MAKE) -f $(bin)AnaDict.make build_strategy=keep_makefiles AnaDictclean



$(bin)Event.make :: dirs
	@$(cmtexe) build constituent_makefile Event -tag=$(tag); \
	  if test ! "$(bin)" = "./"; then /bin/mv -f Event.make $(bin); fi

Event :: $(Event_dependencies) $(bin)Event.make
	@echo " starting Event"
	@$(MAKE) -f $(bin)Event.make build_strategy=keep_makefiles Event

Eventclean :: $(Eventclean_dependencies) $(bin)Event.make
	@echo " starting Eventclean"
	@$(MAKE) -f $(bin)Event.make build_strategy=keep_makefiles Eventclean



$(bin)Tools.make :: dirs
	@$(cmtexe) build constituent_makefile Tools -tag=$(tag); \
	  if test ! "$(bin)" = "./"; then /bin/mv -f Tools.make $(bin); fi

Tools :: $(Tools_dependencies) $(bin)Tools.make
	@echo " starting Tools"
	@$(MAKE) -f $(bin)Tools.make build_strategy=keep_makefiles Tools

Toolsclean :: $(Toolsclean_dependencies) $(bin)Tools.make
	@echo " starting Toolsclean"
	@$(MAKE) -f $(bin)Tools.make build_strategy=keep_makefiles Toolsclean



$(bin)Config.make :: dirs
	@$(cmtexe) build constituent_makefile Config -tag=$(tag); \
	  if test ! "$(bin)" = "./"; then /bin/mv -f Config.make $(bin); fi

Config :: $(Config_dependencies) $(bin)Config.make
	@echo " starting Config"
	@$(MAKE) -f $(bin)Config.make build_strategy=keep_makefiles Config

Configclean :: $(Configclean_dependencies) $(bin)Config.make
	@echo " starting Configclean"
	@$(MAKE) -f $(bin)Config.make build_strategy=keep_makefiles Configclean



$(bin)Ana.make :: dirs
	@$(cmtexe) build constituent_makefile Ana -tag=$(tag); \
	  if test ! "$(bin)" = "./"; then /bin/mv -f Ana.make $(bin); fi

Ana :: $(Ana_dependencies) $(bin)Ana.make
	@echo " starting Ana"
	@$(MAKE) -f $(bin)Ana.make build_strategy=keep_makefiles Ana

Anaclean :: $(Anaclean_dependencies) $(bin)Ana.make
	@echo " starting Anaclean"
	@$(MAKE) -f $(bin)Ana.make build_strategy=keep_makefiles Anaclean



$(bin)LhaaSim.make :: dirs
	@$(cmtexe) build constituent_makefile LhaaSim -tag=$(tag); \
	  if test ! "$(bin)" = "./"; then /bin/mv -f LhaaSim.make $(bin); fi

LhaaSim :: $(LhaaSim_dependencies) $(bin)LhaaSim.make
	@echo " starting LhaaSim"
	@$(MAKE) -f $(bin)LhaaSim.make build_strategy=keep_makefiles LhaaSim

LhaaSimclean :: $(LhaaSimclean_dependencies) $(bin)LhaaSim.make
	@echo " starting LhaaSimclean"
	@$(MAKE) -f $(bin)LhaaSim.make build_strategy=keep_makefiles LhaaSimclean



$(bin)anamuons.make :: dirs
	@$(cmtexe) build constituent_makefile anamuons -tag=$(tag); \
	  if test ! "$(bin)" = "./"; then /bin/mv -f anamuons.make $(bin); fi

anamuons :: $(anamuons_dependencies) $(bin)anamuons.make
	@echo " starting anamuons"
	@$(MAKE) -f $(bin)anamuons.make build_strategy=keep_makefiles anamuons

anamuonsclean :: $(anamuonsclean_dependencies) $(bin)anamuons.make
	@echo " starting anamuonsclean"
	@$(MAKE) -f $(bin)anamuons.make build_strategy=keep_makefiles anamuonsclean


