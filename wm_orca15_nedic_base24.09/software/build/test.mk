# testing rules

TESTING_FLAGS = -fdiagnostics-color=always -fanalyzer -Wstrict-prototypes -Wno-deprecated -Wno-type-limits -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wunreachable-code -Wconversion -Wsign-conversion -Wswitch-enum -Wdouble-promotion -Wsign-compare -Wno-unused-parameter -Wunreachable-code -Wformat=2 -Wnull-dereference -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wstrict-prototypes -Wold-style-definition -O0 $(UDEFS) -MMD -MP -MF $(TESTING_DEPDIR)/$*.d
#COVERAGE_FLAGS

# ADD HERE:
# ALL_EXE_DEP += $(TESTING_EXECDIR)/testing_app_main.exe
ALL_EXE_DEP += $(TESTING_EXECDIR)/testing_app_soak.exe
ALL_EXE_DEP += $(TESTING_EXECDIR)/testing_app_wash.exe
ALL_EXE_DEP += $(TESTING_EXECDIR)/testing_app_rinse.exe
ALL_EXE_DEP += $(TESTING_EXECDIR)/testing_app_spin.exe
# ALL_EXE_DEP += $(TESTING_EXECDIR)/testing_app_steam.exe
# ALL_EXE_DEP += $(TESTING_EXECDIR)/testing_app_dry.exe
# ALL_EXE_DEP += $(TESTING_EXECDIR)/testing_app_heater.exe

UPDATE_EXE_DEP = $(TESTING_EXECDIR)/testing_app_main_update.exe

testing_all: compile_all link_all run_all
testing_update : compile_all link_update run_update

compile_all: testing_begin $(TESTING_OBJS)

link_all: $(TESTING_OBJS) $(ALL_EXE_DEP)
link_update: $(TESTING_OBJS) $(UPDATE_EXE_DEP)

testing_format:
	@clang-format -style=file:.clang-format -i $(TESTING_SRC)

testing_cppcheck:
	@cppcheck --enable=all --addon=misra_cli.json --inline-suppr --error-exitcode=1 -f --suppress=missingInclude --suppress=unmatchedSuppression --suppress=unusedFunction --template=gcc --std=c11 $(TESTING_SRC)

testing_compile_commands: $(TESTING_OBJS) $(ALL_EXE_DEP)
	@echo
	@echo  "\033[0;33m[Generating compile_commands.json]\033[0m"
	@$(REMOVE) compile_commands.json
	@compiledb -n make link_all

testing_begin:
	@echo  "\033[0;33mGCC Version \033[0m  "
	@gcc --version

# ADD HERE:
run_all: $(TESTING_OBJS) $(ALL_EXE_DEP)
# @echo
# @echo
# @echo  "\033[0;32mRunning app_main Test Cases :  \033[0m"
# @ $(TESTING_EXECDIR)/testing_app_main.exe -v
# @echo
# @echo
# @echo  "\033[0;32m Running app_dry Test Cases :  \033[0m"
# @ $(TESTING_EXECDIR)/testing_app_dry.exe -v
# @echo
# @echo
# @echo  "\033[0;32m Running app_heater Test Cases :  \033[0m"
# @ $(TESTING_EXECDIR)/testing_app_heater.exe -v
	@echo
	@echo
	@echo  "\033[0;32m Running app_wash Test Cases :  \033[0m"
	@ $(TESTING_EXECDIR)/testing_app_wash.exe -v
	@echo
	@echo
	@echo  "\033[0;32m Running app_soak Test Cases :  \033[0m"
	@ $(TESTING_EXECDIR)/testing_app_soak.exe -v
	@echo
	@echo
	@echo  "\033[0;32m Running app_rinse Test Cases :  \033[0m"
	@ $(TESTING_EXECDIR)/testing_app_rinse.exe -v
	@echo
	@echo
	@echo  "\033[0;32m Running app_spin Test Cases :  \033[0m"
	@ $(TESTING_EXECDIR)/testing_app_spin.exe -v
# @echo
# @echo
# @echo  "\033[0;32m Running app_steam Test Cases :  \033[0m"
# @ $(TESTING_EXECDIR)/testing_app_steam.exe -v
	@echo
	@echo
	@echo  "\033[0;32mAll Tests : OK\033[0m"
	@echo


run_update: $(TESTING_OBJS) $(UPDATE_EXE_DEP)
	@echo
	@echo  "\033[0;32m Running app_normal_mode_update Test Cases :  \033[0m"
	@$(TESTING_EXECDIR)/testing_app_main_update.exe -v

# ADD HERE:
$(TESTING_EXECDIR)/testing_app_main.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_no_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(COVERAGE_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DAPP_MAIN -DTESTING -DUNITY_OUTPUT_COLOR $^ -o $@

$(TESTING_EXECDIR)/testing_app_soak.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_no_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(COVERAGE_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DAPP_SOAK -DTESTING -DUNITY_OUTPUT_COLOR $^ -o $@

$(TESTING_EXECDIR)/testing_app_wash.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_no_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(COVERAGE_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DAPP_WASH -DTESTING -DUNITY_OUTPUT_COLOR $^ -o $@

$(TESTING_EXECDIR)/testing_app_rinse.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_no_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(COVERAGE_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DAPP_RINSE -DTESTING -DUNITY_OUTPUT_COLOR $^ -o $@

$(TESTING_EXECDIR)/testing_app_spin.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_no_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(COVERAGE_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DAPP_SPIN -DTESTING -DUNITY_OUTPUT_COLOR $^ -o $@

$(TESTING_EXECDIR)/testing_app_steam.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_no_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(COVERAGE_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DAPP_STEAM -DTESTING -DUNITY_OUTPUT_COLOR $^ -o $@

$(TESTING_EXECDIR)/testing_app_dry.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_no_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(COVERAGE_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DAPP_DRY -DTESTING -DUNITY_OUTPUT_COLOR $^ -o $@


$(TESTING_EXECDIR)/testing_app_heater.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_no_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(COVERAGE_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DDRY_HEATER -DTESTING -DUNITY_OUTPUT_COLOR $^ -o $@


$(TESTING_EXECDIR)/testing_app_main_update.exe: $(TESTING_OBJS) $(TESTING_ROOT_SRC_DIR)/test_update.c $(TESTING_ROOT_SRC_DIR)/test_main.c
	@echo  $(MSG_LINKING) $@
	@gcc $(TESTING_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DAPP_MAIN -DTESTING -DLOG_ENABLE -DUNITY_OUTPUT_COLOR $^ -o $@


# pattern

$(TESTING_OBJDIR)/%.o : %.c
	@echo  $(MSG_COMPILING) $<
	@gcc $(TESTING_FLAGS) $(patsubst %,-I%,$(TESTINGINCDIRS)) $(patsubst %,-I%,$(EXTRAINCDIRS)) -DTESTING -DUNITY_OUTPUT_COLOR -c -o $@ $<


testing_clean:
	@echo $(MSG_CLEANING)
	@$(REMOVE) $(TESTING_OBJDIR)/*.o
	@$(REMOVE) $(TESTING_DEPDIR)/*.d
	@$(REMOVE) $(TESTING_OBJDIR)/*.gcda
	@$(REMOVE) $(TESTING_OBJDIR)/*.gcno
	@$(REMOVE) $(TESTING_EXECDIR)/*.exe
	@$(REMOVE) $(TESTING_EXECDIR)/*.gcda
	@$(REMOVE) $(TESTING_EXECDIR)/*.gcno


include $(wildcard $(TESTING_DEPFILES))
