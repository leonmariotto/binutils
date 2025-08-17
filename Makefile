

LIBNAME := libelftool.a
NAME := elftool
NAME_COVERAGE := $(NAME)_coverage
NAME_PROFILING := $(NAME)_profiling

CC := gcc

CFLAGS := -Wall -Wextra -Werror -g3

# CMocka require this
CFLAGS += -DHAVE_SIGNAL_H
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-result
CFLAGS += -Wno-return-type

FANALYZER_FLAG =					\
-O2									\
-flto								\
-fanalyzer							\
-fstack-usage						\

#-fcallgraph-info=su,da				\
#-fdump-analyzer-callgraph			\
#-fdump-analyzer-supergraph			\

FPROFILE_FLAG = 					\
-pg									\

COVERAGE_FLAG = 					\
--coverage							\

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
ASM_DIR = $(BUILD_DIR)/asm
PRE_DIR = $(BUILD_DIR)/preprocessing
CALLGRAPH_DIR = $(BUILD_DIR)/callgraph

SRC := 											\
	src/client/main.c							\
	src/client/elftool.c						\

LIBSRC :=										\
	src/lib/elftool_query.c							\
	src/lib/elftool_load.c							\
	src/lib/elftool_dump.c							\
	src/lib/elftool_getopt.c						\
	src/lib/elftool_parse.c							\
	src/lib/elftool_transform.c						\
	src/lib/elftool_write.c							\

INCDIR := inc/				\
		  	libft/			\
			cmocka/include	\
			liblst/	\

IFLAGS 	= $(foreach D,$(INCDIR), -I$(D))

LIBOBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIBSRC))

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
OBJ_COVERAGE = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.cov.o, $(SRC))
OBJ_PROFILING = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.profiling.o, $(SRC))
ASM = $(patsubst $(SRC_DIR)/%.c, $(ASM_DIR)/%.S, $(SRC))
PRE = $(patsubst $(SRC_DIR)/%.c, $(PRE_DIR)/%.i, $(SRC))

CALLGRAPHS_DOTFILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.callgraph.dot, $(SRC))
SUPERGRAPHS_DOTFILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.supergraph.dot, $(SRC))
SUPERGRAPHS_EG_DOTFILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.c.supergraph-eg.dot, $(SRC))
GCDAFILES = $(patsubst $(OBJ_DIR)/%.cov.o, $(OBJ_DIR)/%.cov.gcda, $(OBJ))

MISRA_REPORT_FILE = $(BUILD_DIR)/misra_report.txt

LIBLST = liblst/liblst.a

LIB = $(LIBLST)

all: prep asm obj callgraphs misra lib bin

asm: $(ASM)
prep: $(PRE)
obj: $(OBJ)
coverage: $(OBJ_COVERAGE) $(NAME_COVERAGE) $(NAME_PROFILING)
profiling: $(OBJ_PROFILING)
callgraphs: $(CALLGRAPH_DIR)/callgraph.svg
misra: $(MISRA_REPORT_FILE)
lib: $(LIBNAME)
bin: $(NAME)

CGRAPH_FILE = $(NAME).wpa.000i.cgraph

$(LIBLST):
	$(MAKE) -C liblst/

$(CGRAPH_FILE): $(NAME)

$(LIBNAME): $(LIBOBJ)
	ar rc $@ $(LIBOBJ)
	ranlib $@

$(NAME): $(LIBNAME) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB) $(LIBNAME) $(IFLAGS) -fdump-ipa-cgraph

$(NAME_COVERAGE): $(OBJ_COVERAGE)
	$(CC) $(CFLAGS) -o $@ $(OBJ_COVERAGE) $(LIB) $(IFLAGS) -lgcov

$(NAME_PROFILING): $(OBJ_PROFILING)
	$(CC) $(CFLAGS) -o $@ $(OBJ_PROFILING) $(LIB) $(IFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(FANALYZER_FLAG) -o $@ -c $< $(IFLAGS)

$(OBJ_DIR)/%.cov.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(COVERAGE_FLAG) -o $@ -c $< $(IFLAGS)

$(OBJ_DIR)/%.profiling.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(FPROFILE_FLAG) -o $@ -c $< $(IFLAGS)

$(PRE_DIR)/%.i: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(IFLAGS)

$(ASM_DIR)/%.S: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< $(IFLAGS)

$(CALLGRAPH_DIR)/callgraph.svg: $(CGRAPH_FILE)
	@mkdir -p $(dir $@)
	cd gcc_graph_tool && uv sync && . .venv/bin/activate && cd - && \
	./gcc_graph_tool/gcc_graph_tool.py -y ./gcc_graph_tool/yml/callgraph.yml -i $< -o $@

$(CALLGRAPH_DIR)/%.supergraph.svg: $(OBJ_DIR)/%.c.supergraph.dot
	@mkdir -p $(dir $@)
	dot -Tsvg -o $@ $<

$(CALLGRAPH_DIR)/%.supergraph-eg.svg: $(OBJ_DIR)/%.c.supergraph-eg.dot
	@mkdir -p $(dir $@)
	dot -Tsvg -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(NAME) $(LIBNAME) $(NAME_COVERAGE) $(NAME_PROFILING)
	rm -f *.cgraph
	rm -rf unit_test_*

$(MISRA_REPORT_FILE): $(SRC) $(wildcard $(INC_DIR)/*.h)
	cppcheck --template="{file}:{line}:{column}: {id}:{severity}:{message}" --output-file=$@ --addon=misra/misra.json $(SRC) $(wildcard $(INC_DIR)/*.h)

re: clean all
