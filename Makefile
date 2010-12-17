# The source, documentation and compiled model are in separate subdirectories.
SRC_DIR = src
DOC_DIR = doc
BUILD_DIR = build

# The basename of the main C++ source file.
MAIN = guyton92
# The name of the binary for the compiled model.
BINARY = $(BUILD_DIR)/$(MAIN)

# The program depends on the following C++ modules.
MODULES = $(MAIN) params read_params vars read_vars debug utils read_exp
# Automatically add all sub-modules of the model.
MODULES += $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/module_*.cpp))
# Automatically add all model experiments.
MODULES += $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/exp_*.cpp))
# Automatically add all model instruments.
MODULES += $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/instr_*.cpp))
# Automatically add all notification filters.
MODULES += $(patsubst $(SRC_DIR)/%.cpp,%,$(wildcard $(SRC_DIR)/filter_*.cpp))

CPP_FILES = $(MODULES:%=$(SRC_DIR)/%.cpp)
HDR_FILES = $(MODULES:%=$(SRC_DIR)/%.h)
SRC_FILES = $(HDR_FILES) $(CPP_FILES)

# The following files are automatically generated.
TMP_FILES = $(addprefix $(SRC_DIR)/,params.h params.cpp vars.h vars.cpp)

# The configuration file for doxygen.
DOXY_FILE = $(SRC_DIR)/Doxyfile

# The file containing the main page of the doxygen documentation.
MAIN_PAGE = $(SRC_DIR)/mainpage.h

# Search for the g++ and clang++ compilers. Return "ERROR" if neither exists.
CXX := $(shell which g++ || which clang++ || echo ERROR)

# Print an error message and terminate if no compiler was found.
ifneq (,$(findstring ERROR,$(CXX)))
    $(error ERROR: unable to find g++ or clang++)
endif

# The flags for the C++ compiler: no optimisations and lots of warnings.
CXXFLAGS := -O0 -std=c++98 -Wall -Wextra -Wno-unused-parameter

# Search for doxygen. Return "ERROR" if it does not exist.
DOXYGEN := $(shell which doxygen || echo ERROR)

# Print an error message and terminate if doxygen was not found.
ifneq (,$(findstring ERROR,$(DOXYGEN)))
    $(warning WARNING: unable to find doxygen, cannot build documentation.)
endif

#
# Targets
#

# The default target is to build the model binary and the documentation.
all: model docs

# Provide "model" as a separate target that builds the model binary.
model: $(BINARY)

# The output binary depends on all source files.
$(BINARY): $(SRC_FILES)
	@echo "  [Compiling]"
	@if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	@$(CXX) $(CXXFLAGS) -o $@ $(CPP_FILES)

# Provide "docs" as a separate target.
docs: $(DOC_DIR)/index.html

# The documentation depends on the source and doxygen configuration file.
$(DOC_DIR)/index.html: $(SRC_FILES) $(DOXY_FILE) $(MAIN_PAGE)
	@echo "  [Documentation]"
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
	-@rm -f $(BINARY)
	-@rm -rf $(DOC_DIR)/*
