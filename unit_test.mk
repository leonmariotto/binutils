# Input variables
UNIT_NAME = nm_bin_parse
# Warning: Do not put "./" before path
# Currently only support a single unit src in gcovr cmdline
UNIT_SRC = src/$(UNIT_NAME).c
UNIT_TEST_SRC = ./unit_test/test_$(UNIT_NAME).c
UNIT_INC_PATH = ./inc ./liblst
# Warning: input lib must be already compiled
UNIT_LIB = liblst/liblst.a
CMOCKA_PATH = ./cmocka

CC := gcc

CFLAGS := -Wall -Wextra -g3

FPROFILE_FLAG = 					\
-pg									\

COVERAGE_FLAG = 					\
--coverage							\

BUILD_DIR = unit_test_$(UNIT_NAME)
OBJ_DIR = $(BUILD_DIR)/obj

SRC := \
	$(UNIT_SRC)\
	$(UNIT_TEST_SRC)\
	$(CMOCKA_PATH)/src/cmocka.c

INCDIR := $(UNIT_INC_PATH) \
	$(CMOCKA_PATH)/include

IFLAGS 	= $(foreach D,$(INCDIR), -I$(D))

UNIT_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(UNIT_SRC))
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
UNIT_GCDAFILE = $(patsubst $(OBJ_DIR)/%.o, $(OBJ_DIR)/%.gcda, $(UNIT_OBJ))

LIB = $(UNIT_LIB)

BIN_NAME = $(BUILD_DIR)/bin/$(UNIT_NAME)_coverage
JUNIT_REPORT = $(BUILD_DIR)/report/$(UNIT_NAME).junit

all: obj bin run coverage
bin: $(BIN_NAME)
obj: $(OBJ)
run: $(UNIT_GCDAFILE) $(JUNIT_REPORT)
coverage: $(BUILD_DIR)/report/html/coverage.html

$(BIN_NAME): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB) $(IFLAGS) -lgcov

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(COVERAGE_FLAG) -o $@ -c $< $(IFLAGS)

$(JUNIT_REPORT) $(UNIT_GCDAFILE)&: $(BIN_NAME)
	@mkdir -p $(dir $(JUNIT_REPORT))
	$(BIN_NAME) > $(JUNIT_REPORT)

$(BUILD_DIR)/report/html/coverage.html: $(UNIT_GCDAFILE)
	@mkdir -p $(dir $@)
	gcovr --object-directory $(OBJ_DIR) -r .  --html --html-details -o $@ --filter "$(UNIT_SRC)"

clean:
	rm -rf $(BUILD_DIR)

re: clean all
