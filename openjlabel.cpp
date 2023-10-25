#define VERSION "1.0.0"

#include <stdlib.h>
#include "libopenjtalk-lang.h"

void usage() {
  fprintf(stderr, "The Japanese Phoneme Label Generation System \"OpenJLabel\"\n");
  fprintf(stderr, "OpenJLabel (https://github.com/KoharuYuzuki/OpenJLabel)\n");
  fprintf(stderr, "Version %s\n", VERSION);
  fprintf(stderr, "\n");
  fprintf(stderr, "OpenJLabel is developed based on LibOpenJTalk.\n");
  fprintf(stderr, "LibOpenJTalk (https://github.com/KoharuYuzuki/LibOpenJTalk)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "  usage:\n");
  fprintf(stderr, "    openjlabel [ options ] [ text ]\n");
  fprintf(stderr, "  options:\n");
  fprintf(stderr, "    -d : dictionary directory\n");
  fprintf(stderr, "    -o : filename of output labels\n");
  fprintf(stderr, "\n");
  exit(0);
}

int main(int argc, char **argv) {
  size_t i;

  /* dictionary directory */
  char *dic_dir = NULL;

  /* output file pointer */
  FILE *labfp = NULL;

  /* text */
  char *text = NULL;

  /* labels */
  char **labels = NULL;

  /* label length */
  int label_len = 0;

  /* usage */
  for (i = 0; i < argc; i++) {
    if ((argv[i][0] == '-') && (argv[i][1] == 'h')) {
      usage();
    }
  }

  /* get dictionary directory */
  for (i = 0; i < argc; i++) {
    if ((argv[i][0] == '-') && (argv[i][1] == 'd')) {
      dic_dir = argv[++i];
      break;
    }
  }
  if (dic_dir == NULL) {
    fprintf(stderr, "Error: Dictionary directory must be specified.\n");
    exit(1);
  }

  /* open output file */
  for (i = 0; i < argc; i++) {
    if ((argv[i][0] == '-') && (argv[i][1] == 'o')) {
      labfp = fopen(argv[++i], "wt");
      break;
    }
  }
  if (labfp == NULL) {
    fprintf(stderr, "Error: Filename of output labels must be specified.\n");
    exit(1);
  }

  /* get text */
  if ((argc >= 2) && (argv[argc - 2][0] != '-')) {
    text = argv[argc - 1];
  }
  if (text == NULL) {
    fprintf(stderr, "Error: Text must be specified.\n");
    exit(1);
  }

  /* initialize openjtalk */
  Open_JTalk *open_jtalk = Open_JTalk_initialize();

  /* load dictionary */
  if (Open_JTalk_load(open_jtalk, dic_dir, NULL) != true) {
    fprintf(stderr, "Error: Dictionary cannot be loaded.\n");
    Open_JTalk_clear(&open_jtalk);
    exit(1);
  }

  /* extract label */
  if (Open_JTalk_extract_label(open_jtalk, text, &labels, &label_len) != true) {
    fprintf(stderr, "Error: Could not extract label.\n");
    Open_JTalk_clear(&open_jtalk);
    exit(1);
  }

  /* output labels */
  for (i = 0; i < label_len; i++) {
    fprintf(labfp, "%s\n", labels[i]);
  }

  /* free memory */
  Open_JTalk_clear(&open_jtalk);

  /* close file */
  if (labfp != NULL) {
    fclose(labfp);
  }

  return 0;
}
