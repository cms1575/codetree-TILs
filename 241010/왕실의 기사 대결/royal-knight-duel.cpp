#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
int L, N, Q;
int board[41][41];
int 기사위치[41][41];
int temp[41][41];


struct knight {
	int r, c, h, w, k;
};
knight k[31];
int damage[31];
int quest[101][2];
int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1,0,-1 };

void move(int index, int d) {
	if (k[index].w <= 0) {
		return;
	}

	//선 테스트
	//vis선언및초기화
	vector<bool> vis(31, false);
	vector<knight> tk(31);
	queue<int> queue;

	//temp 초기화
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			temp[i][j] = 0;
		}
	}

	bool isfirst = true;
	vis[index] = true;
	queue.push(index);
	while (!queue.empty()) {
		int i = queue.front(); queue.pop();
		int nr = k[i].r + dx[d];
		int nc = k[i].c + dy[d];
		int w = k[i].w; int h = k[i].h;
		int count = 0;

		for (int a = nr; a < nr+h; a++) {
			for (int b = nc; b < nc+w; b++) {
				if (a<1 || a>L || b<1 || b>L) {  return; }//cout<<i<<a<<b << "끝에 닿음 \n";
				if (board[a][b] == 2) {  return; }//cout << i<<"장애물에 닿음 \n";
				if (board[a][b] == 1) count++;

				if (기사위치[a][b] != 0 && vis[기사위치[a][b]] == false) {
					vis[기사위치[a][b]] = true;
					queue.push(기사위치[a][b]);


				}

				//temp에 색칠
				temp[a][b] = i; // i는 현재 큐에서 나와서 조사진행중인 기사의 인덱스


			}
		}
		tk[i].r = nr;
		tk[i].c = nc;
		if (isfirst) {
			tk[i].k = k[i].k;
			isfirst = false;
		}
		else {
			tk[i].k = k[i].k - count;
		}
	}//end of while

	//만일 이 라인이 실행된다면, 문제없이 밀어내기 성공한 것임

	//vis안된 나이트들 옮기기(밀어지지않은 나이트들)
	for (int i = 1; i <= N; i++) {
		if (vis[i] == false&&k[i].k>0) {
			for (int a = k[i].r; a < k[i].r + k[i].h; a++) {
				for (int b = k[i].c; b < k[i].c + k[i].w; b++) {
					temp[a][b] = i;
				}
			}
		}
		else {//tk를 k에 반영하기
			k[i].r = tk[i].r;
			k[i].c = tk[i].c;
			damage[i] += (k[i].k - tk[i].k);
			k[i].k = tk[i].k;
			
			if (k[i].k <= 0) {
				for (int a = k[i].r; a < k[i].r + k[i].h; a++) {
					for (int b = k[i].c; b < k[i].c + k[i].w; b++) {
						temp[a][b] = 0;
					}
				}
			}
			
		}
	}


	
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			기사위치[i][j] = temp[i][j];// 여기서 기사위치 변경
			//cout<< 기사위치[i][j];
		}
		//cout << '\n';
	}
	


}
int main() {
	//freopen("test1.txt", "r", stdin);
	cin >> L >> N >> Q;
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> board[i][j];
		}
		
	}
	for (int i = 1; i <= N; i++) {
		cin >> k[i].r >> k[i].c >> k[i].h >> k[i].w >> k[i].k;
		for (int a = 0; a < k[i].h; a++) {
			for (int b = 0; b < k[i].w; b++) {
				기사위치[a + k[i].r][b + k[i].c] = i;
			}
		}
	}
	// 기사위치 출력테스트
	/*cout << "기사초기위치\n";
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cout<<기사위치[i][j];
		}
		cout << endl;
	}
	cout << "-----------\n";*/

	for (int i = 1; i <= Q; i++) {
		cin >> quest[i][0] >> quest[i][1];
	}

	for (int i = 1; i <= Q; i++) {
		move(quest[i][0], quest[i][1]);
	}

	//결과 출력
	int result = 0;
	for (int i = 1; i <= N; i++) {
		if (k[i].k >= 1) {
			result += damage[i];
		}
	}
	cout << result;

}