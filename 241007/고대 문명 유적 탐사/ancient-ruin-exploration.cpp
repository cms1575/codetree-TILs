#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
#define X first
#define Y second
int board[5][5];
int temp[5][5];
bool vis[5][5]; //for vis

int list[300];
int K, M;

int res;
int dx[4] = { 1,0,-1,0 };
int dy[4] = { 0,1,0,-1 };

int rx[8] = { -1,-1,-1,0,1,1,1,0 };
int ry[8] = { -1,0,1,1,1,0,-1,-1 };

struct Tamsa_com {
	int tiles;
	int rotate;
	int c;
	int r;
	int board[5][5];
};

void init_vis() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			vis[i][j] = false;
		}
	}
}



void print_board() {
	cout << '\n';
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << temp[i][j];
		}
		cout << '\n';
	}
	

}

void print_main_board() {
	cout << '\n';
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << board[i][j];
		}
		cout << '\n';
	}


}


void init_temp_board() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			temp[i][j] = board[i][j];
		}
	}
}

int Tamsa() {
	Tamsa_com TC;
	TC.r = 9; TC.r = 9; TC.tiles = 0; TC.rotate = 9;// initialize Tamsa_complete to maximum
	pair<int, int> C;
	// a,b for center loop, d for rotate loop, e for rotate loop
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			C = { a + 1,b + 1 };
			//cout << "\n Center (x,y) = " << C.X << C.Y << endl;
			for (int d = 0; d < 3; d++) {// c=0 90도 회전, c=1 180도 회전, ... so on
				//cout << "Rotate " << 90 * (d + 1) << " degree\n";
				init_temp_board();

				for (int e = 0; e < 8; e++) {
					int nx = C.X + rx[e];
					int ny = C.Y + ry[e];
					
					int tx = C.X + rx[(e + 2*(d+1)) % 8];
					int ty=C.Y+ ry[(e + 2 * (d + 1)) % 8];
					temp[tx][ty] = board[nx][ny];
				}
				//print_board();
				// do bfs to remove dup three tiles

				// 비교 대상 removed_tile 많을수록, d 작을수록, C.Y 작을수록, C.X 작을수록
				
				int removed_tile = 0;
				
				init_vis();
				
				// 하나의 중심 잡고, 그중 하나의 각도 잡은 temp 보드 완전 탐색해서 3개 이상 remove
				
				for (int m = 0; m < 5; m++) {
					for (int n = 0; n < 5; n++) {
						queue<pair<int, int>> Q;
						queue<pair<int, int>> B; // backtracking, 선언이자 빈 큐 만들기
						int count = 1;
						int nx, ny;
						Q.push({ m,n }); B.push({ m,n });
						vis[m][n] = true;
						while (!Q.empty()) {
							pair<int, int> curr = Q.front(); Q.pop();
							for (int o=0; o < 4; o++) {
								nx= curr.X + dx[o];
								ny = curr.Y + dy[o];
								if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) continue;
								if (vis[nx][ny] == true || temp[nx][ny] != temp[curr.X][curr.Y]) continue;
								vis[nx][ny] = true;
								Q.push({ nx,ny });
								B.push({ nx,ny });
								count++;

							}
						}
						if (count >= 3) {
							while (!B.empty()) {
								pair<int, int> curr = B.front(); B.pop();
								temp[curr.X][curr.Y] = 0;
								removed_tile++;
							}
							//cout<<"\n 3회 중복 발생 \n";
							//print_board();
						}
						

					}
				}
				bool exchange = false;

				if (TC.tiles < removed_tile) {
					TC.tiles = removed_tile;
					TC.rotate = d;
					TC.r = C.X; TC.c = C.Y;
					exchange = true;
				}
				else if (TC.tiles == removed_tile) {
					if (TC.rotate > d) {
						TC.rotate = d;
						TC.r = C.X; TC.c = C.Y;
						exchange = true;
					}
					else if (TC.rotate == d) {
						if (TC.c > C.Y) {
							TC.r = C.X; TC.c = C.Y;
							exchange = true;
						}
						else if (TC.c == C.Y) {
							if (TC.r > C.X) {
								TC.r = C.X; 
								exchange = true;
							}
						}

					}

				}
				if (exchange == true) {
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 5; j++) {
							TC.board[i][j] = temp[i][j];
						}
					}
				}

			}
			//cout << "------------------------------\n";




		}
	}
	if (TC.rotate == 9) {
		return -1;
	}
	
	//cout << "Tamsa Completed :\n";
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			board[i][j] = TC.board[i][j];
			//cout << board[i][j];
		}
		//cout << "\n";
	}/*
	cout << "Tiles :" << TC.tiles << endl;
	cout << "Rotates :" << TC.rotate << endl;
	cout << "Rows :" << TC.r << endl;
	cout << "Columns :" << TC.c << endl;*/
	res = TC.tiles;
	return 0;
}

int main() {
	cin >> K >> M;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> board[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		cin >> list[i];
	}
	int index = 0;

	for (int i = 0; i < K; i++) {
		//탐사 진행
		res = 0;
		if (Tamsa() < 0) {
			return 0;
		}
		
		//completed board 
		//cout << "\n리스트 채우기 진행 \n";
		bool again = true;
		
		while (again) {
			for (int j = 0; j < 5; j++) {
				for (int i = 4; i >= 0; i--) {
					if (board[i][j] == 0) {
						board[i][j] = list[index++];
					}
				}
			}

			int removed_tile = 0;

			init_vis();
			again = false;
			// 하나의 중심 잡고, 그중 하나의 각도 잡은 temp 보드 완전 탐색해서 3개 이상 remove

			for (int m = 0; m < 5; m++) {
				for (int n = 0; n < 5; n++) {
					queue<pair<int, int>> Q;
					queue<pair<int, int>> B; // backtracking, 선언이자 빈 큐 만들기
					int count = 1;
					int nx, ny;
					Q.push({ m,n }); B.push({ m,n });
					vis[m][n] = true;
					while (!Q.empty()) {
						pair<int, int> curr = Q.front(); Q.pop();
						for (int o = 0; o < 4; o++) {
							nx = curr.X + dx[o];
							ny = curr.Y + dy[o];
							if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) continue;
							if (vis[nx][ny] == true || board[nx][ny] != board[curr.X][curr.Y]) continue;
							vis[nx][ny] = true;
							Q.push({ nx,ny });
							B.push({ nx,ny });
							count++;

						}
					}
					if (count >= 3) {
						while (!B.empty()) {
							pair<int, int> curr = B.front(); B.pop();
							board[curr.X][curr.Y] = 0;
							removed_tile++;
						}
						//cout << "\n 3회 중복 발생 \n";
						again = true;
						//print_main_board();
					}
					


				}
			}
			//cout << "\n 리스트 중복 찾기에서 제거됨 :" << removed_tile << endl;
			res += removed_tile;

		}
		//탐사 진행 후 리스트에서 채우고 bfs 진행
		if (res != 0) { cout << res << " "; }

		
	}



}