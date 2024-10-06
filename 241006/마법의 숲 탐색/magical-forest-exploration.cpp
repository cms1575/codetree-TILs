#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;
#define X first
#define Y second
int dx[4] = { -1,0,1,0 };
int dy[4] = {  0,1,0,-1};
int R, C, K;
int board[73][70];
bool vis[73][70];

struct golem {
	int ci;
	int di;
	pair<int, int> Center;
};
golem G[1001];

void init_board() { // initialize board & vis
	for (int i = 0;i<73; i++) {
		for (int j = 0; j < 70; j++) {
			board[i][j] = 0;
			vis[i][j] = false;
		}
	}
}

bool start_ok() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j<C; j++) {// 이게 문법에 맞나>?
				if (board[i][j] != 0) {
					//cout << "start_failed, initialize board again";
					return false;
				}
		}
	}
	return true;
}

void print_board() {
	cout << '\n';
	cout << '\n';
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < C; j++) {
			cout << board[i][j];
		}
		cout << '\n';
	}
	cout << "--------------------\n";
	for (int i = 3; i < R+3; i++) {
		for (int j = 0; j < C; j++) {
			cout << board[i][j];
		}
		cout << '\n';
	}
}


int main() {
	cin >> R >> C >> K;
	int res = 0;
	for (int i = 1; i < K+1; i++) {
		cin >> G[i].ci >> G[i].di;
		G[i].Center.X = 1; 
		G[i].Center.Y = G[i].ci - 1;
	}

	init_board();

	for (int i = 1; i < K+1; i++) {
		// 3방향 쭉 밑으로 내려오기, 처음에는 항상 3열은 비어있는 상태이다.
		while (true) {
			//밑 확인
			pair<int, int> curr = G[i].Center;
			if (curr.X + 2 < R + 3 && board[curr.X + 1][curr.Y - 1] == 0 && board[curr.X + 2][curr.Y] == 0 && board[curr.X + 1][curr.Y + 1] == 0) {
				G[i].Center.X++;
				continue;
			}

			if (curr.Y - 2 >=0 && curr.X + 2 < R + 3 && board[curr.X - 1][curr.Y - 1] == 0 && board[curr.X ][curr.Y-2] == 0 && board[curr.X + 1][curr.Y - 1] == 0 && board[curr.X + 1][curr.Y - 2] == 0 && board[curr.X + 2][curr.Y - 1] == 0) {
				G[i].Center.Y-=1; G[i].Center.X++; G[i].di -= 1;				
				if (G[i].di < 0) { G[i].di = 3; }
				continue;
			}

			if (curr.Y + 2 <C && curr.X + 2 < R + 3&& board[curr.X - 1][curr.Y + 1] == 0 && board[curr.X ][curr.Y +2] == 0 && board[curr.X + 1][curr.Y + 1] == 0 && board[curr.X + 1][curr.Y +2] == 0 && board[curr.X +2][curr.Y+1] == 0 ) {
				G[i].Center.Y += 1; G[i].Center.X++; G[i].di += 1;
				G[i].di %= 4;
				continue;
			}
			break;
		}

		//쭉 밑으로 내려온 다음 출구 방향 표시, 보드 색칠 하기
		pair<int, int> curr = G[i].Center;

		board[curr.X][curr.Y] = i;
		for (int j = 0; j < 4; j++) {
			int nx = curr.X + dx[j];
			int ny = curr.Y + dy[j];
			board[nx][ny] = i;
		}
		board[curr.X + dx[G[i].di]][curr.Y + dy[G[i].di]] =-1*i;


		//print_board();
		//끝까지 밑으로 내려온 다음 골렘이 북쪽 경계선을 전부 벗어났는지 확인
		if (start_ok() == false) {
			init_board(); continue;
		}

		//전부 벗어났을 경우 누적 알고리즘 진행
		queue<pair<int, int>> Q;
		int highest = 0; // 제일 남쪽은 제일 행의 크기가 큰거 

		Q.push({ G[i].Center.X,G[i].Center.Y });
		vis[G[i].Center.X][G[i].Center.Y] = true;
		highest = G[i].Center.X;

		while (!Q.empty()) {
			pair<int, int> curr = Q.front(); Q.pop();
			for (int k = 0; k < 4; k++) {
				int nx = curr.X + dx[k];
				int ny = curr.Y + dy[k];
				if (nx < 0 || nx >= R + 3 || ny < 0 || ny >= C) continue;
				if (vis[nx][ny] == true || board[nx][ny] == 0) continue;

				if (board[curr.X][curr.Y] < 0) {
					//do nothing
				}
				else {
					if ((board[nx][ny] != board[curr.X][curr.Y]) && (board[nx][ny] != board[curr.X][curr.Y] * (- 1))) {
						continue;
					}

				}


				vis[nx][ny] = true;
				if (nx > highest) { highest = nx; }
				Q.push({ nx,ny });

			}



		}
		highest-=2; // 1부터 시작하는거 맞추기
		res += highest;
		//cout << "\n accumed " << highest << endl;
		for (int i = 0; i < 73; i++) {
			for (int j = 0; j < 70; j++) {
				
				vis[i][j] = false;
			}
		}


	}
	cout << res;

}