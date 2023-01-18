#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


struct electron{
    float x,y,vx,vy;
};

struct atom{
    float x,y;
};

//stwarza electron na początku pola z takimi wartościami x=0 y(0;100) vx(-10;10) vy(-10;10) 
struct electron create_electron(void){
    struct electron el;
    srand(time(NULL));
    el.x=0;
    el.y=rand()%101;
    el.vx=rand()%20-10;
    el.vy=rand()%20-10;
    //printf("x = %f  y = %f  vx = %f  vy = %f\n", el.x, el.y, el.vx, el.vy);
    return el;
}

//oblicza współrzędne i prędkość electrona po jednej secundie(jednym cyklu)
//boost to przespieszenie electrona wzdlóż współrzędnej X. Tą stałą trzeba dopasować
//W naszych obliczeniach ona równa a = E*e/m 
void onesecond(struct electron *el, float boost){
    el->vx = el->vx + boost; //najpierw zmieniamy prędkość a potem liczymy kordynaty 
    el->x=el->x + el->vx;
    el->y=el->y + el->vy; 
    //printf("x = %f  y = %f  vx = %f  vy = %f\n", el->x, el->y, el->vx, el->vy);
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
    //for(i=0;i<300;i++){
    //    printf("number atomu=%d  x=%f y=%f\n",i , (atom_arr+i)->x, (atom_arr+i)->y);
    //}
}
//zeruje prędkość vx jeżeli electron zdaża się z atomem i kontroluje że by nie wylatał z pola y(0;100)
//dla sprawdzenia udeżenia electrona wykorzystuje się wzór (x-a)^2+(x-b)^2<r^2
//wspx=(x-a)^2 wspy=(y-a)^2 
void collision_check(struct electron * el, struct atom * atom_arr){
    int i=0;
    float wspx, wspy, r2;
    r2=25;
    if(el->y > 100){
        el->y=el->y-100;
    }
    if(el->y < 0){
        el->y=el->y+100;
    }
    for(i=0;i<300;i++){
        wspx=(el->x - (atom_arr+i)->x)*(el->x - (atom_arr+i)->x);
        wspy=(el->y - (atom_arr+i)->y)*(el->y - (atom_arr+i)->y);
        if((wspx+wspy)<r2){
            el->vx=0;
            //printf("colizja el.x=%f el.y=%f  atom.x=%f atom.y=%f\n", el->x, el->y, (atom_arr+i)->x, (atom_arr+i)->y);
        }
    }
}

//pole ma rozmiar x(0;1000) y(0;100)
//avsp - avarage speed 
int main(){
    int n=0, i;
    float boost;
    float avavsp=0, sumsp=0;
    struct electron el;
    struct atom atom_arr[300];
    printf("wpisz przespieszenie = ");
    scanf("%f", &boost);
    for(i=0;i<100;i++){
        n=0;
        sleep(1);   
        create_atoms(atom_arr);
        el=create_electron();
        while(el.x < 1000){
            n++;
            onesecond(&el, boost);
            collision_check(&el, atom_arr);
        }
        sumsp=sumsp+((float)1000/n);
        //printf("sum speed = %f\n", sumsp);
        }
    avavsp=sumsp/(1.0*i);
    printf("average average speed = %f\n", avavsp);
    return 0;
}