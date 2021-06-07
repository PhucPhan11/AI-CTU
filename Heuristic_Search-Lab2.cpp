#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <cmath>

using namespace std;

const int BOARD_ROWS = 3;
const int BOARD_COLS = 3;
const int EMPTY_TILE = 0;
const int MAX_OP = 4;

struct State{
    int num[BOARD_ROWS][BOARD_COLS];
    int empty_row; //luu chi muc dong cua o trong
    int empty_col; //luu chi muc cot cua o trong

};

struct Node{
    State state;
    Node* parent;
    int g; //luu gia tri duong di cua nut goc den nut hien tai
    int h; //luu ket qua uoc luong cua ham heuristic cua nut hien tai den trang thai dich
    int f; // = g + h
};

//dinh nghia phep toan so sanh trong multiset
struct node_cmp{
    bool operator() (Node* a, Node* b){
        return a->f < b->f;
    }
};

//kiem tra 2 mang trang thai xem co giong nhau khong?
bool SameState (State s1, State s2){
    if (s1.empty_col != s2.empty_col || s1.empty_row != s2.empty_row){
        return false;
    }

    for (int row = 0; row < BOARD_ROWS; row++){
        for (int col = 0; col < BOARD_COLS; col++){
            if (s1.num[row][col] != s2.num[row][col]){
                return false;
            }
        }
    }
    return true;
}

//phep toan di chuyen o trong len tren
bool up(State s, State &out){
    int er = s.empty_row, ec = s.empty_col;
    if (er > 0){
        out = s;
        out.empty_col = ec;
        out.empty_row = er - 1;

        out.num[er][ec] = s.num[er - 1][ec];
        out.num[er - 1][ec] = EMPTY_TILE;

        return true;
    }
    return false;
}

//phep toan di chuyen o trong xuong duoi
bool down(State s, State &out){
    int er = s.empty_row, ec = s.empty_col;
    	if (er < 2){
		out = s;
		out.empty_col = ec;
		out.empty_row = er + 1;
		
		out.num[er][ec] = s.num[er + 1][ec];
		out.num[er + 1][ec] = EMPTY_TILE;
		
		return true;
		
	}
	return false;
}

//phep toan di chuyen o trong sang trai
bool left (State s, State &out){
	int er = s.empty_row, ec = s.empty_col;
	if (ec > 0){
		out = s;
		out.empty_col = ec - 1;
		out.empty_row = er;
		
		out.num[er][ec] = s.num[er][ec - 1];
		out.num[er][ec - 1] = EMPTY_TILE;
		
		return true;
		
	}
	return false;
}

//phep toan di chuyen o trong sang phai 
bool right (State s, State &out){
	int er = s.empty_row, ec = s.empty_col;
	if (ec < 2){
		out = s;
		out.empty_col = ec + 1;
		out.empty_row = er;
		
		out.num[er][ec] = s.num[er][ec + 1];
		out.num[er][ec + 1] = EMPTY_TILE;
		
		return true;
	}
	return false;
}

//ham sinh cac phep toan
bool call_operator (State s, State& out, int op_no){
    switch (op_no){
        case 1:
            return up(s, out);
        case 2:
            return down(s, out);
        case 3:
            return left(s, out);
        case 4:
            return right(s, out);
        default:
            return false;
    }
}

//in trang thai s
void PrintState(State s){
    for(int i = 0; i < BOARD_ROWS; i++){
        for(int j = 0; j < BOARD_COLS; j++){
            cout << s.num[i][j] << " ";

        }
        cout << "\n";
    }
}

//kiem tra trang thai co phai la goal?
bool IsGoal(State s, State goal){
    return SameState(s, goal);
}

//ham h1 dem so vi tri sai khac
int h1(State s, State s2){
    int count = 0;
    for(int row = 0; row < BOARD_ROWS; row++){
        for(int col = 0; col < BOARD_COLS; col++){
            if(s.num[row][col] != s2.num[row][col]){
                count++;
            }
        }
    }
    return count;
}

//search phan tu da co trong frontier?
Node* FindNode(State s, multiset<Node *, node_cmp> list){
    for(Node* n: list){
        if(SameState(s, n->state)){
            return n;
        }
    }
    return NULL;
}

//search phan tu da co trong explore?
bool FindState(State s, vector<State> *explored){
    for(State c1: *explored){
        if(SameState(s, c1)){
            return true;
        }
    }
}

//nhap trang thai cho bai toan
State* GetState(){
    State *s = new State();
    for(int row = 0; row < BOARD_ROWS; row++){
        for(int col = 0; row < BOARD_COLS; col++){
            cin >> s->num[row][col];

            if(s->num[row][col] == 0){
                s->empty_row = row;
                s->empty_col = col;
            }
        }
    }
    return s;
}

//giai thuat A*
Node* A_star(State init_state, State goal_state, vector<State> *explored){
    //initial
    Node* root = new Node();
    root->state = init_state;
    root->parent = NULL;
    root->g = 0;
    root->h = h1(init_state, goal_state);
    root->f = root->g + root->h;

    //frontiers la tap Open duoc sap thu tu
    multiset<Node*, node_cmp> frontiers;
    frontiers.insert(root);
    while(!frontiers.empty()){
        Node* node = *frontiers.begin();
        frontiers.erase(frontiers.begin());
        explored->push_back(node->state);
        if(SameState(node->state, goal_state)){
            return node;
        }
        
        //generate states
        for(int op = 1; op <= 4; op++){

            cout << "lololo";
            State new_state;
            if(call_operator(node->state, new_state, op)){
                if(FindState(new_state, explored)){
                    continue;
                }

                Node* n = FindNode(new_state, frontiers);
                if(n == NULL){
                    n = new Node();
                    n->parent = node;
                    n->state = new_state;
                    n->h = h1(new_state, goal_state);
                    PrintState(new_state);
                    
                    n->g = node->g+1;
                    n->f = n->g + n->h;
                    cout << "== Gia tri g: " << n->g << "  == Gia tri f: " << n->f << endl;
                    frontiers.insert(n);
                }

                else{
                    // neu nut con moi tim da thuoc duong bien
                    //kiem tra gia tri f cua nut con co nho hon cac nut da nam trong frontier khong?
                    //neu nho hon gia tri cap nhat lai, neu lon hon thi khong can lam gi ca
                    n->g = node->g+1;
                    n->f = n->g + n->h;
                }
            }
        }
    }
    return NULL;
}

//ham in duong di
void PrintPath(Node* r){
    int i = 0;
    stack <State> q;
    cout << "\nDuong di loi giai\n";
    while(r->parent != NULL){
        q.push(r->state);
        r = r->parent;
        i++;
    }

    q.push(r->state); //them nui goc vao stack
    while(!q.empty()){
        cout << "\nTrang thai thu " << i++ <<endl;
        PrintState(q.top());
        cout << endl;
        q.pop();
    }
}


int main(){
    State s, goal, test;
	s = {{{1,2,5}, {6,0,7}, {3,4,8}}, 1, 1};
	
	goal = {{{0,1,2}, {3,4,5}, {6,7,8}}, 2, 2};
	vector<State> explored;
	Node* p = A_star(s, goal, &explored);
    //PrintPath(p);

    system("pause");
    return 0;
}