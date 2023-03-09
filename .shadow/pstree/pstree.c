#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

void display_version() {
  printf("pstree (PSmisc) 23.4\nCopyright (C) 1993-2020 Werner Almesberger and Craig Small\n\nPSmisc comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it under	the terms of the GNU General Public License.\nFor more information about these matters, see the files named COPYING.\n");
}

static bool spids = false, nsort = false, ver = false;
static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"show-pids"   , no_argument, NULL, 'p'},
    {"numeric-sort", no_argument, NULL, 'n'},
    {"version"     , no_argument, NULL, 'V'},
		{0             , 0          , NULL,  0 },
	};
	int o;
	while ( (o = getopt_long(argc, argv, "pnV", table, NULL)) != -1){
    switch (o) {
      case 'p': spids = true; break;
			case 'n': nsort = true; break;
			case 'V': ver = true; break;
			default:
				printf("-p, --show-pids\n");
				printf("-n, --numeric-sort\n");
				printf("-p, --version\n");
				assert(0);
		}
	}
	return 0;
}

typedef struct node{
  pid_t pid, ppid;
  char comm[64];
  struct node *son, *bro; // NULL!!
} Node;
Node nodes[1024]; //TODO
int nr_n = 0;

int comp(const void *a, const void *b) {
  return (((Node *)a)->pid - ((Node *)b)->pid);
	//qsort(a, n, sizeof(Node), comp);
}

static void getdir() {
	nodes[0].pid = 0; nodes[0].ppid = 0;
	nodes[0].son = NULL; nodes[0].bro = NULL;
  //printf("pid = %d\n", getpid());

	DIR *dirp = opendir("/proc");
	assert(NULL != dirp);
	struct dirent *entry = NULL;
	while ((entry = readdir(dirp))) {
    if (entry->d_type == DT_DIR){
      //printf("%s ", entry->d_name);
			char *ch = entry->d_name;
			pid_t pid = 0;
			bool ispid = true;
			while (*ch){
        if (*ch < '0' || *ch > '9'){
          ispid = false;
					break;
				}
        pid = pid * 10 + *ch - '0';
				ch++;
			}
      if (ispid) {
				//printf("%d ", pid);// getpid
			  char fname[32] = {'\0'};//, comm[64] = {'\0'};
			  char state; //pid_t occu;//, ppid;
			  sprintf(fname, "/proc/%d/stat", pid);
        FILE *fp = fopen(fname, "r");
			  if (fp) {
          // fscanf, fgets, ...
					nr_n ++;
				  fscanf(fp, "%d (%s %c %d", &(nodes[nr_n].pid), nodes[nr_n].comm, &state, &(nodes[nr_n].ppid));
					for (int i = 63; i >= 0; i--){
            if (nodes[nr_n].comm[i] == ')'){
              nodes[nr_n].comm[i] = '\0';
							break;
						}
					}
				  if (nodes[nr_n].ppid == 0) printf("pid = %d, comm = %s, ppid = %d\n", nodes[nr_n].pid, nodes[nr_n].comm, nodes[nr_n].ppid);
					//ppid == 0 -> root -> create a node with pid 0
					fclose(fp);
			  }
			  else {
          assert(0);
			  }
			}
		}
	}
	closedir(dirp);
	if (nsort) qsort(nodes, nr_n + 1, sizeof(Node), comp);
	//for (int i = 0; i <= nr_n; i++){
	//	printf("pid = %d, comm = %s, ppid = %d\n", nodes[i].pid, nodes[i].comm, nodes[i].ppid);
	//}
	//printf("nr_n = %d\n", nr_n);
}

static void insert_node (pid_t ppid, int idx) {
	int pidx = -1;
  for (int i = 0; i <= nr_n; i++) {
    if (nodes[i].pid == ppid){
      pidx = i;
			break;
		}
	}
	assert (-1 != pidx);
	if (nodes[pidx].son != NULL) {
		Node *p = nodes[pidx].son;
		while (p->bro != NULL) p = p->bro;
		p->bro = &(nodes[idx]);
	}
	else {
    nodes[pidx].son = &(nodes[idx]);
	}
}
static void build_tree() {
  //printf("the tree is built\n");
	for (int i = 1; i <= nr_n; i++) {
		nodes[i].son = NULL;
    nodes[i].bro = NULL;
	}
	for (int i = 1; i <= nr_n; i++) {
		insert_node(nodes[i].ppid, i);
	}
}

bool line[1024]; //TODO
static void print_tree(Node *cur, int len) {//len: position of start
	//printf("the tree is printed\n");
  if (NULL == cur) return;
  //while (cur) {
  //  
	//}
	printf("%s", cur->comm);
	int pidLen = 0;
	if (spids){
		printf("(%d)", cur->pid);
		pidLen += 2;
		int temp = cur->pid;
    while (temp) {//temp != 0
      temp /= 10;
			pidLen++;
		}
	}
	if (cur->son) {
		if (cur->son->bro) {
			printf("\u2500\u252c\u2500");
      line[len + strlen(cur->comm) + pidLen + 1] = 1;
		}
		else{
			printf("\u2500\u2500\u2500");
      line[len + strlen(cur->comm) + pidLen + 1] = 0;
		}
		print_tree(cur->son, len + strlen(cur->comm) + pidLen + 3);
	}
  if (cur->bro) {
    printf("\n");
		for (int i = 0; i < len - 2; i++){
      if (line[i]) printf("\u2502");
			else printf(" ");
		}
		if (cur->bro->bro) printf("\u251c");
		else {
			printf("\u2514");
			line[len - 2] = 0;
		}
		printf("\u2500");
    print_tree(cur->bro, len);
	}
	//else {
  //  line[len]
	//}
}

int main(int argc, char *argv[]) {
  /*for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);*/
	parse_args(argc, argv);
  if (ver){
    display_version();
		return 0;
	}
	// TODO
	getdir();
	build_tree();
	//assert(nodes[0].son->bro == NULL);
	print_tree(nodes[0], 0);
  return 0;
}
