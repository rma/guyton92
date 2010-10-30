# The source, documentation and compiled model are in separate subdirectories.
SRC_DIR = src
DOC_DIR = doc
BUILD_DIR = build

# The basename of the main C++ source file.
MAIN = guyton92
# The name of the binary for the compiled model.
MODEL = $(BUILD_DIR)/$(MAIN)

# The program depends on the following C++ modules.
MODULES = $(MAIN) params read_params vars read_vars
CPP_FILES = $(MODULES:%=$(SRC_DIR)/%.cpp)
HDR_FILES = $(MODULES:%=$(SRC_DIR)/%.h)
SRC_FILES = $(HDR_FILES) $(CPP_FILES)

# The following files are automatically generated.
TMP_FILES = $(addprefix $(SRC_DIR)/,params.h params.cpp vars.h vars.cpp)

# The configuration file for doxygen.
DOXY_FILE = $(SRC_DIR)/Doxyfile

# Search for the g++ compiler. Return "ERROR" if it does not exist.
GCC := $(shell which g++ || echo ERROR)

# Print an error message and terminate if no compiler was found.
ifneq (,$(findstring ERROR,$(GCC)))
    $(error ERROR: unable to find g++)
endif

# The default set of flags for the C++ compiler
GCC_FLAGS := -w -O3

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

# Provide "model" as a separate target.
model: $(MODEL)

# The output program depends on all source files (*.cpp and *.h).
$(MODEL): $(SRC_FILES) $(BUILD_DIR)
	@echo "  [Compiling]"
	@$(GCC) $(GCC_FLAGS) -o $(MODEL) $(CPP_FILES)

# Provide "docs" as a separate target.
docs: $(DOC_DIR)/index.html

# The documentation depends on the source and doxygen configuration file.
$(DOC_DIR)/index.html: $(DOC_DIR) $(SRC_FILES) $(DOXY_FILE)
	@echo "  [Documentation]"
	@cd $(SRC_DIR) && $(DOXYGEN)

# Create the build directory if it does not exist.
$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

# Create the documentation directory if it does not exist.
$(DOC_DIR):
	@mkdir $(DOC_DIR)

# Inform make that all temporary files are secondary files.
# They are not deleted automatically once the target is built.
.SECONDARY: $(TMP_FILES)

# Mark the phony targets.
.PHONY: clean clobber

# Generate params.h with the script params.sh.
$(SRC_DIR)/params.h: $(SRC_DIR)/params.sh $(SRC_DIR)/params.lst
	@cd $(SRC_DIR) && ./params.sh

# Generate params.cpp with the script params.sh.
$(SRC_DIR)/params.cpp: $(SRC_DIR)/params.sh $(SRC_DIR)/params.lst
	@cd $(SRC_DIR) && ./params.sh

# Generate vars.h with the script vars.sh.
$(SRC_DIR)/vars.h: $(SRC_DIR)/vars.sh $(SRC_DIR)/vars.lst
	@cd $(SRC_DIR) && ./vars.sh

# Generate vars.cpp with the script vars.sh.
$(SRC_DIR)/vars.cpp: $(SRC_DIR)/vars.sh $(SRC_DIR)/vars.lst
	@cd $(SRC_DIR) && ./vars.sh

# Remove the temporary files, since they can be regenerated.
clean:
	-@rm -f $(TMP_FILES)

# Remove the temporary files, the model binary and the documentation.
clobber: clean
	-@rm -f $(MODEL)
	-@rm -rf $(DOC_DIR)/*
