# The source, documentation and compiled model are in separate subdirectories.
SRC_DIR = src
DOC_DIR = doc
BUILD_DIR = build

# The basename of the main C++ source file.
MAIN = guyton92
# The name of the binary for the compiled model.
MAINBIN = $(BUILD_DIR)/$(MAIN)

# The basename of the sensitivity analyser source file.
SENS = sensitivity
# The name of the binary for the sensitivity analyser.
SENSBIN = $(BUILD_DIR)/$(SENS)

# The names of all binaries defined in this Makefile.
BINARIES = $(MAINBIN) $(SENSBIN)

# The C++ modules that define the core of the Guyton model.
CORE = params vars utils
CORE += $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/module_*.cpp))
CORE += $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/model_*.cpp))

# Additional modules that extend the functionality of the Guyton model.
EXPS = $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/exp_*.cpp))
INSTRS = $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/instr_*.cpp))
FILTS = $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/filter_*.cpp))
MISC = guyton92_step debug read_params read_vars read_exp

# The Guyton model depends on the following C++ modules.
MAIN_MODS = $(CORE) $(MAIN) $(EXPS) $(INSTRS) $(FILTS) $(MISC)
# Define variables for the .cpp and .h files.
MAIN_CPP = $(MAIN_MODS:%=$(SRC_DIR)/%.cpp)
MAIN_HDR = $(MAIN_MODS:%=$(SRC_DIR)/%.h)
MAIN_SRC = $(MAIN_HDR) $(MAIN_CPP)

# The sensitivity analyser depends on the following C++ modules.
SENS_MODS = $(CORE) $(SENS)
# Define variables for the .cpp and .h files.
SENS_CPP = $(SENS_MODS:%=$(SRC_DIR)/%.cpp)
SENS_HDR = $(SENS_MODS:%=$(SRC_DIR)/%.h)
SENS_SRC = $(SENS_CPP) $(SENS_HDR)

# The simulation library depends on the following C++ modules.
LIB_NAME = $(BUILD_DIR)/libg92.so
LIB_MODS = $(CORE) $(EXPS) $(INSTRS) $(FILTS) $(MISC)
# Define variables for the .cpp and .h files.
LIB_CPP = $(LIB_MODS:%=$(SRC_DIR)/%.cpp)
LIB_HDR = $(LIB_MODS:%=$(SRC_DIR)/%.h)
LIB_SRC = $(LIB_CPP) $(LIB_HDR)

# The doxygen documentation depends on every source file.
ALL_SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.h)

# The following files are automatically generated.
TMP_FILES = $(addprefix $(SRC_DIR)/,params.h params.cpp vars.h vars.cpp)

# The configuration file for doxygen.
DOXY_FILE = $(SRC_DIR)/Doxyfile

# The file containing the main page of the doxygen documentation.
MAIN_PAGE = $(SRC_DIR)/mainpage.h

# Search for the g++ and clang++ compilers. Return "ERROR" if neither exists.
CXX := $(shell which clang++ || which g++ || echo ERROR)

# Print an error message and terminate if no compiler was found.
ifneq (,$(findstring ERROR,$(CXX)))
    $(error ERROR: unable to find g++ or clang++)
endif

# Allow the echo command to be over-ridden for silent compilation.
ECHO := echo

# The experiment to perform (the default is no experiment).
EXPERIMENT := VANILLA

# The flags for the C++ compiler: no optimisations and lots of warnings.
WARNINGS := -Wall -Wextra -Wno-unused-parameter
CXXFLAGS := -O0 -std=c++98 $(WARNINGS) -D $(EXPERIMENT) -fPIC

# Search for doxygen. Return "ERROR" if it does not exist.
DOXYGEN := $(shell which doxygen || echo ERROR)

# Print an error message and terminate if doxygen was not found.
ifneq (,$(findstring ERROR,$(DOXYGEN)))
    $(warning WARNING: unable to find doxygen, cannot build documentation.)
endif

#
# Targets
#

# The default target is to build the model binaries and the documentation.
all: $(BINARIES) $(LIB_NAME) docs

# Provide "model" as a separate target that builds the model binary.
model: $(MAINBIN)

$(LIB_NAME): $(LOOP_SRC)
	@$(ECHO) "  [Shared library]"
	@if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	@$(CXX) $(CXXFLAGS) -shared $(LIB_CPP) -o $(LIB_NAME)

# Build the Guyton model.
$(MAINBIN): $(MAIN_SRC)
	@$(ECHO) "  [Compiling]"
	@if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	@$(CXX) $(CXXFLAGS) -o $@ $(MAIN_CPP)

# Build the sensitivity analyser.
$(SENSBIN): $(SENS_SRC)
	@$(ECHO) "  [Compiling]"
	@if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	@$(CXX) $(CXXFLAGS) -o $@ $(SENS_CPP)

# Provide "docs" as a separate target.
docs: $(DOC_DIR)/index.html

# The documentation depends on the source and doxygen configuration file.
$(DOC_DIR)/index.html: $(ALL_SRC) $(DOXY_FILE) $(MAIN_PAGE)
	@$(ECHO) "  [Documentation]"
	@if [ ! -d $(DOC_DIR) ]; then mkdir $(DOC_DIR); fi
	@cd $(SRC_DIR) && $(DOXYGEN)

# Inform make that all temporary files are secondary files.
# They are not deleted automatically once the target is built.
.SECONDARY: $(TMP_FILES)

# Mark the phony targets.
.PHONY: model docs clean clobber

# Generate params.h with the script params.sh.
$(SRC_DIR)/params.h: $(addprefix $(SRC_DIR)/,params.sh params.lst params.val)
	@cd $(SRC_DIR) && ./params.sh

# Generate params.cpp with the script params.sh.
$(SRC_DIR)/params.cpp: $(addprefix $(SRC_DIR)/,params.sh params.lst params.val)
	@cd $(SRC_DIR) && ./params.sh

# Generate vars.h with the script vars.sh.
$(SRC_DIR)/vars.h: $(addprefix $(SRC_DIR)/,vars.sh vars.lst vars.val)
	@cd $(SRC_DIR) && ./vars.sh

# Generate vars.cpp with the script vars.sh.
$(SRC_DIR)/vars.cpp: $(addprefix $(SRC_DIR)/,vars.sh vars.lst vars.val)
	@cd $(SRC_DIR) && ./vars.sh

# Remove the temporary files, since they can be regenerated.
clean:
	-@rm -f $(TMP_FILES)

# Remove the temporary files, the model binary and the documentation.
clobber: clean
	-@rm -f $(BINARIES) $(LIB_NAME)
	-@rm -rf $(DOC_DIR)/*
