# ----------------------
# Dynamic Options System
# ----------------------

DYNOS_APPDATA_DIR := $(APPDATA)/Llennpie/Pluto
DYNOS_INPUT_DIR := ./dynos
DYNOS_OUTPUT_DIR := $(DYNOS_APPDATA_DIR)/dynos
DYNOS_PACKS_DIR := $(DYNOS_APPDATA_DIR)/dynos/packs
DYNOS_INIT := \
    mkdir -p "$(DYNOS_INPUT_DIR)"; \
    mkdir -p "$(DYNOS_OUTPUT_DIR)"; \
    mkdir -p "$(DYNOS_PACKS_DIR)"; \
    cp -f -r "$(DYNOS_INPUT_DIR)" "$(DYNOS_APPDATA_DIR)" 2>/dev/null || true ;

DYNOS_DO := $(shell $(call DYNOS_INIT))
INCLUDE_CFLAGS += -DDYNOS
