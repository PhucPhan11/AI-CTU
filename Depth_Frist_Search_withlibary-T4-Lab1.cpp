#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stack>

using namespace std;

#define tankcapacity_X 9 //sua chua binh X
#define tankcapacity_Y 4 //suc chua binh Y 
#define Empty 0
#define goal 6 //muc tien luong nuoc can dong duoc
#define Maxlength 100 //Stack

//hang chuoi den in ra ten hanh dong
const char* action[] = {"Frist State","pour water pull","pour water full Y",
                        "pour water empty X", "pour water empty Y", 
                        "pour water X to Y","pour water Y to X"};

//khai bao cau truc trang thai
typedef struct
{
    int x; //luong nuoc trong binh x
    int y; //luong nuoc trong bình y
}State;

//khoi tao trang thai binh X = 0 va Y = 0
void MakeNullState(State *state){
    state->x = 0;
    state->y = 0;
}

//in trang thai
void PrintState(State state){
    printf("\n   X: %d ---- Y: %d", state.x, state.y);
}

//ham kiem tra trang thai muc tieu
int Goalcheck(State state){
    return (state.x==goal || state.y==goal);
}

//lam day nuoc binh X
int PourWaterFullX(State cur_state, State *result){
    if(cur_state.x < tankcapacity_X){
        result->x = tankcapacity_X;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}

//lam day nuoc binh y
int PourWaterFullY(State cur_state, State *result){
    if(cur_state.y < tankcapacity_Y){
        result->y = tankcapacity_Y;
        result->x = cur_state.x;
        return 1;
    }
    return 0;
}

//ham lam rong nuoc trong binh X
int PourWaterEmptyX(State cur_state, State *result){
    if(cur_state.x > 0){
        result->x = Empty;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}

//ham lam rong nuoc trong binh Y
int PourWaterEmptyY(State cur_state, State *result){
    if(cur_state.y > 0){
        result->y = Empty;
        result->x = cur_state.x;
        return 1;
    }
    return 0;
}

//chuyen nuoc tu binh X sang binh Y
int PourWaterXY (State cur_state, State *result){
    if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
        result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), Empty);
        result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
        return 1;
    }
    return 0;
}

//chuyen nuoc tu binh Y sang binh X
int PourWaterYX (State cur_state, State *result){
    if(cur_state.y > 0 && cur_state.x < tankcapacity_X){
        result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), Empty);
        result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
        return 1;
    }
    return 0;
}

//goi cac phep toan tren trang thai
int call_operator(State cur_state, State *result, int option){
    switch (option){
        case 1: return PourWaterFullX(cur_state, result);
        case 2: return PourWaterFullY(cur_state, result);
        case 3: return PourWaterEmptyX(cur_state, result);
        case 4: return PourWaterEmptyY(cur_state, result);
        case 5: return PourWaterXY(cur_state, result);
        case 6: return PourWaterYX(cur_state, result);
        default: printf("Error call operators");
            return 0;
    }
}

int CompareStates(State state1, State state2){
    if(state1.x == state2.x && state1.y == state2.y){
        return true;
    }
    return false;
}

/*--------------------hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh*/

//khai bao cau truc nut (dinh) de dung cay tim kiem
typedef struct Node{
    State state;
    struct Node* Parent;
    int no_function;    
}Node;

//tim trang thai trong stack open/close
int FindState(State state, stack<Node*> openStack){
    while(!openStack.empty()){
        if(CompareStates(openStack.top()->state, state)){
            return 1;
        }
        openStack.pop();
    }
    return 0;
    
}

//thuat toan DFS
Node* DFS_Algorithm(State state){
    //khai bao hai ngan xep open va close
    stack<Node*> OpenDFS;
    stack<Node*> CloseDFS;

    //tao nut trang thai cha
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    OpenDFS.push(root);

    while(!OpenDFS.empty()){
        //lay mot dinh trong ngan xep
        Node* node = OpenDFS.top();
        OpenDFS.pop();
        CloseDFS.push(node);

        //kiem tra xem dinh day ra co phai trang thai goal?
        if(Goalcheck(node->state)){
            return node;
        }    

        int opt;

        //goi cac phep toan tren trang thai
        for(opt = 1; opt <= 6; opt++){
            State newstate;
            if(call_operator(node->state, &newstate, opt)){
                //neu trang thai moi sinh ra da ton tai thi bo qua
                if(FindState(newstate, CloseDFS) || FindState(newstate, OpenDFS)){
                    continue;
                }

                //neu trang thai moi chua ton tai thi them vao ngan xep
                Node* newnode = (Node*)malloc(sizeof(Node));
                newnode->state = newstate;
                newnode->Parent = node;
                newnode->no_function = opt;
                OpenDFS.push(newnode);
            }
        }
    }
    
    return NULL;

}

//in ket qua chuyen nuoc de dat goal
void PrintWaysToGetGoal(Node* node){
    stack<Node*> stackprint;

    //duyet nguoc ve nut parent
    while(node->Parent != NULL){
        stackprint.push(node);
        node = node->Parent;
    }
    stackprint.push(node);

    //in ra thu tu hanh dong chuyen nuoc
    int no_action = 0;
    while(!stackprint.empty()){
        printf("\nAction %d: %s", no_action, action[stackprint.top()->no_function]);
        PrintState(stackprint.top()->state);
        stackprint.pop();
        no_action++;
    }
}


int main(){
    State cur_state = {0, 0};
    Node* p = DFS_Algorithm(cur_state);
    PrintWaysToGetGoal(p);

    system("pause");
    return 0;
}