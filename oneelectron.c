#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct electron{
    float x,y,vx,vy;
};

struct atom{
    float x,y;
};

//stwarza electron na początku pola z takimi wartościami x=0 y(0;100) vx(0;10) vy(0;10) 
struct electron create_electron(void){
    struct electron a;
    srand(time(NULL));
    a.x=0;
    a.y=rand()%101;
    a.vx=rand()%20-10;
    a.vy=rand()%20-10;
  //  printf("x = %f  y = %f  vx = %f  vy = %f\n", a.x, a.y, a.vx, a.vy);
    return a;
}

//oblicza współrzędne i prędkość electrona po jednej secundie(jednym cyklu)
//boost to przespieszenie electrona wzdlóż współrzędnej X. Tą stałą trzeba dopasować
//W naszych obliczeniach ona równa a = E*e/m 
void onesecond(struct electron * a, float boost){
    a->x=a->x + a->vx;
    a->y=a->y + a->vy;
    a->vx = a->vx + boost; 
    printf("x = %f  y = %f  vx = %f  vy = %f\n", a->x, a->y, a->vx, a->vy);
}

//wypełnia tablice atomów nadając centry atomów x(0;1000) y(0;100)
void create_atoms(struct atom * atom_arr){
    int i,j,n;
    for(j=0;j<6;j++){
        for(i=0;i<50;i++){
            n=n+1;
            (atom_arr+50*j+i)->x=20*i;
            (atom_arr+50*j+i)->y=20*j;
        }
    }
    /*for(i=0;i<300;i++){
        printf("i=%d  x=%f y=%f\n",i , (atom_arr+i)->x, (atom_arr+i)->y);
    }*/
}



//pole ma rozmiar x(0;1000) y(0;100)
int main(){
    int i, n;
    float boost=5;
    struct electron el;
    struct atom atom_arr[300];

    el=create_electron();
    create_atoms(atom_arr);
    //robi iteracji póki electron nie skonczy droge i kontroluje że by nie wylatał z pola y(0;100)
    for(n=0;;n++){
        onesecond(&el, boost);
        if(el.y > 100){
            el.y=el.y-100;
        }
        if(el.y < 0){
            el.y=el.y+100;
        }
        for(i=0;i<300;i++){
            if(((el.x - (atom_arr+i)->x)*(el.x - (atom_arr+i)->x)+(el.y - (atom_arr+i)->y)*(el.y - (atom_arr+i)->y)) < 25){
                el.vx=0;
            }
        }
        //kończy programm jeśli amain.x>1000
        if(el.x > 1000){
            break;
        }
    }
    printf("mineło t = %d secund", n);

    return 0;
}
