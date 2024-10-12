#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define X first
#define Y second
int board[11][11];
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, 1, -1 };
int N, M, K;//N: 미로의 크기, M:참가자의 수, K : 게임 시간
struct participate {
    int r, c;
};

participate p[11];
bool 탈출성공[11];
int 이동거리[11];
int er, ec;

int dis(const int& a, const int& b) {// dis(p[i].r,p[i].c)
    return (a < er ? er - a : a - er) + (b < ec ? ec - b : b - ec);
}

int 정사각형안에사람수(int r,int c, int w) {
    
    //cout << "w" << w << endl;
    
    for ( int a=0; a <  w ; a++) {
        for (int b=0; b < w ; b++) {
            //cout << a << " " << b << endl;
            if (board[a+r][b+c] < 0) {
                return -1;
            }
        }

    }
    return 0;
}
void print_board() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (i == er && j == ec) {
                cout << "e";
                continue;
            }
            cout << board[i][j];//0이라면, 빈 칸을 의미합니다.1이상 9이하의 값을 갖는다면, 벽을 의미하며, 해당 값은 내구도를 뜻합니다.
        }
        cout << '\n';
    }
}

void 최소정사각형찾기(int &r, int &c, int & w) {
    for (w = 2; w <= N; w++) {
        for (r = 1; r <= N - (w - 1); r++) {
            for (c = 1; c <= N - (w - 1); c++) {
                

                if (er < r || er >= r + w || ec < c || ec >= c + w) continue; // 정사각형 안에 출구가 없는 경우
                
                if (정사각형안에사람수(r, c, w) == 0) continue;
                
                return;

            }
        }


    }
    return;
}
void 회전하기(const int& r, const int& c, const int& w) {
    int temp[11][11];
    for (int a=0; a < w; a++) {
        int nc = c + w - 1 - a;
        for (int b=0; b < w; b++) {
            if (board[a + r][b + c] > 0) {
                board[a + r][b + c]--;
            }
            else if (board[a + r][b + c] < 0) {
                for (int i = 1; i <=M; i++) {
                    if (p[i].r == a + r && p[i].c == b + c) {
                        p[i].r = r + b;
                        p[i].c = nc;
                    }
                }

            }
            else if (er == a + r && ec == b + c) {
                er = r + b; ec = nc;
            }

            temp[r + b][nc] = board[a + r][b + c];

        }
    }

    for (int a = 0; a < w; a++) {
        
        for (int b = 0; b < w; b++) {
            board[a + r][b + c] = temp[a + r][b + c];

        }
    }

}
int maze() {
    /*cout << "\n\nmaze첫부분\n";
    for (int i = 1; i <= M; i++) {
        if (탈출성공[i] == true) continue;
        cout << i << "번 참가자 좌표:" << p[i].r << ", " << p[i].c << endl;
    }*/
    //cout  << "출구    좌표:" << er << ", " << ec << endl;
    //print_board();
    for (int i = 1; i <= M; i++) {
        bool moved = false;
        if (탈출성공[i] == true) continue;
        
        for (int j = 0; j < 4; j++) {
            if (moved == true) break;

            int nx = p[i].r + dx[j];
            int ny = p[i].c + dy[j];
            

            if (nx < 1 || nx >N || ny < 1 || ny >N) continue;
            
            if (board[nx][ny] >= 1) continue;
           
            if (dis(p[i].r, p[i].c) <= dis(nx, ny)) continue;
            
            moved = true;
            //움직이고 갱신하기
            board[p[i].r][p[i].c]++;
            if (board[p[i].r][p[i].c] > 0) {
                //cout << "something wrong";
                return -1;
            }

            if (nx == er && ny == ec) {
                //cout << i<<"탈출!\n";
                탈출성공[i] = true;
            }
            else {
                board[nx][ny]--;
                p[i].r = nx; p[i].c = ny;
            }          
            
            이동거리[i]++;
        }

    }//M번 반복문 끝, 이동 && 탈출 여부 확인
    bool all = true;
    for (int i = 1; i <= M; i++) {
        if (탈출성공[i] == false) { all = false; break; }
    }
    if (all) {
        //cout << "전원탈출!";
        return -1;
    }

    //cout << "\nmaze전원 이동후 최소정사각형파악전\n";
    for (int i = 1; i <= M; i++) {
        if (탈출성공[i] == true) continue;
       // cout << i << "번 참가자 좌표:" << p[i].r << ", " << p[i].c << endl;
    }
    //print_board();
    
    int r, c, w;
    최소정사각형찾기(r, c, w);  
    
    //cout << "중심좌표 : (" << r << ", " << c << ") w: " << w << " 가 최소 정사각형이자 최소 사람출구포함입니다.\n";

    //회전
    회전하기(r, c, w);
    //cout << "회전후" << endl;
    //print_board();

    return 0;

}

int main()
{      
    //freopen("test1.txt", "r", stdin); 
    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> board[i][j];//0이라면, 빈 칸을 의미합니다.1이상 9이하의 값을 갖는다면, 벽을 의미하며, 해당 값은 내구도를 뜻합니다.
        }
    }
    for (int i = 1; i <= M; i++) {//참가자의 좌표
        cin >> p[i].r >> p[i].c;
        board[p[i].r][p[i].c]--;
        탈출성공[i] = false;
    }
    cin >> er >> ec; // 출구 좌표 <- 의심


    for (int i = 0; i < K; i++) {
        if(maze()<0) break;//모든 참가자가 미로를 탈출했을 때 게임 종료
    }
    int sum = 0;
    for (int i = 1; i <= N; i++) {
        sum += 이동거리[i];
    }
    cout << sum<<"\n" << er << " " << ec;
    ////K초가 지났거나, 모든 참가자가 미로를 탈출했을 때 게임 종료
}