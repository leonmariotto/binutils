#include "elftool.h"
#include "elftool_dump.h"
#include "elftool_parse.h"
#include "elftool_write.h"
#include "elftool_load.h"
#include "stdio.h"
#include <arpa/inet.h>
#include <byteswap.h>
#include <elf.h>
#include <endian.h>
#include <string.h>

int elftool_injection(elftool_opt_t *opt, elftool_t *bin) {
  int r = 0;

  if (opt->out) {
    printf("Running injection\n");
    // elftool_t_transform transform = {0};
    // transform.code = malloc(333);
    // transform.code_len = 333;
    // transform.type = NM_SECTION_INJECT;
    // elftool_transform(bin, &transform);
    r = elftool_write(bin, opt->outfile);
  }
  return (r);
}

static void elftool_bin_clean(elftool_t *bin) {
  if (bin->shdr) {
    ft_lstdel(&bin->shdr, default_del);
  }
  if (bin->phdr) {
    ft_lstdel(&bin->phdr, default_del);
  }
  if (bin->syms) {
    ft_lstdel(&bin->syms, default_del);
  }
}

void elftool_clean(elftool_opt_t *opt) {
  if (opt->bins) {
    ft_lstdel(&opt->bins, default_del);
  }
}

int elftool(elftool_opt_t *opt) {
  list_t *head;
  int r = 0;
  bool printName = false;
  elftool_t bin = {0};

  if (!opt) {
    r = -1;
  } else {
    head = opt->bins;
    if (!head) {
      r = -1;
    }
  }
  if (r == 0) {
    if (head->next) {
      printName = true;
    }
    while (r == 0 && head) {
      bzero(&bin, sizeof(bin));
      if (printName) {
        printf("\n%s\n", bin.path);
      }
      r = elftool_load(&bin, head->content);
      if (r == 0) {
        r = elftool_parse(&bin);
      }
      if (r == 0) {
        elftool_dump(opt, &bin);
      }
      if (r == 0) {
        elftool_injection(opt, &bin);
      }
      elftool_bin_clean(&bin);
      head = head->next;
    }
  }
  elftool_clean(opt);
  return (r);
}
