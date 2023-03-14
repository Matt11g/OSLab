#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "thread.h"
#include "thread-sync.h"

#define MAXN 10000
int T, N, M;
char A[MAXN + 1], B[MAXN + 1];
int dp[MAXN][MAXN];
int result;

#define DP(x, y) (((x) >= 0 && (y) >= 0) ? dp[x][y] : 0)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX3(x, y, z) MAX(MAX(x, y), z)

spinlock_t lk = SPIN_INIT();

void Tworker(int id) {
  if (id != 1) {
    // This is a serial implementation
    // Only one worker needs to be activated
    return;
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      // Always try to make DP code more readable
      int skip_a = DP(i - 1, j);
      int skip_b = DP(i, j - 1);
      int take_both = DP(i - 1, j - 1) + (A[i] == B[j]);
      dp[i][j] = MAX3(skip_a, skip_b, take_both);
    }
  }

  result = dp[N - 1][M - 1];
}

int R;
void plcs_worker() {
	//int len = MAX(N, M), wid = MIN(N, M);
	spin_lock(&lk);
  for (int i = 0; i <= R; i++) {
      spin_unlock(&lk);
		  int j = R - i;
      if (0 <= i && i < N && 0 <= j && j < M) {
      int skip_a = DP(i - 1, j);
      int skip_b = DP(i, j - 1);
      int take_both = DP(i - 1, j - 1) + (A[i] == B[j]);
      dp[i][j] = MAX3(skip_a, skip_b, take_both);
			spin_lock(&lk);
		}
	}
	spin_unlock(&lk);
}

int main(int argc, char *argv[]) {
  // No need to change
  assert(scanf("%s%s", A, B) == 2);
  N = strlen(A);
  M = strlen(B);
  T = !argv[1] ? 1 : atoi(argv[1]);

  // Add preprocessing code here

  for (int i = 0; i < T; i++) {
    create(plcs_worker);
  }
  join();  // Wait for all workers
  

  /*for (int round = 0; round < N + M - 1; round++) {
	// 1. 计算出本轮能够计算的单元格
	  R = round;
	  //int nr = (round < n) ? round : (2 * n - 1 - round);
	// 2. 将任务分配给线程执行
	  for (int i = 0; i < T; i++) {
      create(plcs_worker);
		}
	// 3. 等待线程执行完毕
	  join();
	}*/
  printf("%d\n", dp[N - 1][M - 1]);
}
