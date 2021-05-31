#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

#define tankcapacity_X 9 //sua chua binh X
#define tankcapacity_Y 4 //suc chua binh Y 
#define empty 0
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
        result->x = empty;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}

//ham lam rong nuoc trong binh Y
int PourWaterEmptyY(State cur_state, State *result){
    if(cur_state.y > 0){
        result->y = empty;
        result->x = cur_state.x;
        return 1;
    }
    return 0;
}

//chuyen nuoc tu binh X sang binh Y
int PourWaterXY (State cur_state, State *result){
    if(cur_state.x > 0 && cur_state.y < tankcapacity_Y){
        result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), empty);
        result->y = min(cur_state.x + cur_state.y, tankcapacity_Y);
        return 1;
    }
    return 0;
}

//chuyen nuoc tu binh Y sang binh X
int PourWaterYX (State cur_state, State *result){
    if(cur_state.y > 0 && cur_state.x < tankcapacity_X){
        result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), empty);
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


int main(){

    State cur_state = {9, 0}, result;
    printf("Start!\n");
    PrintState(cur_state);
    int opt;
    for(opt = 1; opt <= 6; opt++){
        int thuchien = call_operator(cur_state, &result, opt);
        if(thuchien == 1){  //thuc hien thanh cong
            printf("\n Action %s success", action[opt]);
            PrintState(result);
        }
        else
            printf("\n Action %s Fail", action[opt]);
    }

    system("pause");
    return 0;
}

//oke hehe