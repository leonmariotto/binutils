#include "elftool_getopt.h"
#include <stdio.h>
#include <string.h>

static char *flags_list[] = {"--help",
                             "-h",
                             "--config",
                             "-C",
                             "--prog-header",
                             "-P",
                             "--elf-header",
                             "-E",
                             "--section-header",
                             "-S",
                             "--symbols",
                             "-s",
                             "--hexdump",
                             "-X",
                             "--nm",
                             "--test",
                             "--outfile",
                             "-o",
                             NULL};
static char *flags_help[] = {
    "--help: display this help",
    "--config|-C: display current config",
    "--elf-header|-E: display ELF header summary",
    "--prog-header|-P: display program table header entry",
    "--section-header|-S: display section table header entry",
    "--symbols|-s: display symbols table header entry",
    "--hexdump|-X: hexdump all",
    "--nm: nm-like output",
    "--test: run length test on bins : this should be loong (disactived)",
    "--outfile|-o <outfile>: write loaded binary (disactived)",
    NULL};

static int elftool_getopt_outfile(elftool_opt_t *opt, char *arg) {
  int r = 0;

  opt->out = true;
  if (arg) {
    opt->outfile = arg;
  } else {
    r = -1;
    fprintf(stderr, "-o: missing file name\n");
    for (int i = 0; flags_help[i]; i++) {
      printf("%s\n", flags_help[i]);
    }
  }
  return (r);
}
static int elftool_getopt_nm(elftool_opt_t *opt, char *arg) {
  (void)arg;
  opt->nm = true;
  return (0);
}
static int elftool_getopt_help(elftool_opt_t *opt, char *arg) {
  (void)arg;
  (void)opt;
  for (int i = 0; flags_help[i]; i++) {
    printf("%s\n", flags_help[i]);
  }
  return (1);
}
static int elftool_getopt_config(elftool_opt_t *opt, char *arg) {
  (void)arg;
  opt->config = true;
  return (0);
}
static int elftool_getopt_test(elftool_opt_t *opt, char *arg) {
  (void)arg;
  // TODO
  // opt->test = true;
  (void)opt;
  return (0);
}

static int elftool_getopt_ehdr(elftool_opt_t *opt, char *arg) {
  (void)arg;
  opt->ehdr = true;
  return (0);
}
static int elftool_getopt_phdr(elftool_opt_t *opt, char *arg) {
  (void)arg;
  opt->phdr = true;
  return (0);
}
static int elftool_getopt_section(elftool_opt_t *opt, char *arg) {
  (void)arg;
  opt->shdr = true;
  return (0);
}
static int elftool_getopt_hexdump(elftool_opt_t *opt, char *arg) {
  (void)arg;
  opt->hexdump = true;
  return (0);
}

static int elftool_getopt_symbols(elftool_opt_t *opt, char *arg) {
  (void)arg;
  opt->syms = true;
  return (0);
}

static int (*flags_handler[])(elftool_opt_t *, char *) = {
    elftool_getopt_help,    elftool_getopt_help,    elftool_getopt_config,
    elftool_getopt_config,  elftool_getopt_phdr,    elftool_getopt_phdr,
    elftool_getopt_ehdr,    elftool_getopt_ehdr,    elftool_getopt_section,
    elftool_getopt_section, elftool_getopt_symbols, elftool_getopt_symbols,
    elftool_getopt_hexdump, elftool_getopt_hexdump, elftool_getopt_nm,
    elftool_getopt_test,
#define START_2ARG_IDX 16
    elftool_getopt_outfile, elftool_getopt_outfile, NULL,
};

int elftool_getopt(int ac, char **av, elftool_opt_t *opt) {
  int i = 1;
  int r = 0;
  bool b = true;

  while (b == true && r == 0 && i < ac) {
    b = false;
    for (int j = 0; r == 0 && flags_list[j]; j++) {
      if (!strcmp(flags_list[j], av[i])) {
        r = (flags_handler[j])(opt, (i + 1 != ac ? av[i + 1] : NULL));
        if (j >= START_2ARG_IDX) {
          i += 2;
        } else {
          i++;
        }
        b = true;
        break;
      }
    }
  }
  if (r == 0) {
    // no binary given, default to a.out
    if (i == ac) {
      ft_lstpush(&opt->bins, ft_lstnew("./a.out", strlen("./a.out") + 1));
    } else {
      while (i < ac) {
        ft_lstpush(&opt->bins, ft_lstnew(av[i], strlen(av[i]) + 1));
        i++;
      }
    }
  }
  return (r);
}

void elftool_printopt(elftool_opt_t *opt) {
  t_list *head = opt->bins;

  printf("elf-header=%d\n", opt->ehdr);
  printf("program-header=%d\n", opt->phdr);
  printf("section=%d\n", opt->shdr);
  printf("hexdump=%d\n", opt->hexdump);
  while (head) {
    printf("bin={%s}\n", (char *)head->content);
    head = head->next;
  }
}
