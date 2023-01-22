#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


struct electron
{
    float x;
    float y;
    float x_veleocity;
    float y_veleocity;
};

struct XY
{
    float x;
    float y;
};

// tworzy elektron w XY (0, 0~100) z V = (-10~+10)
struct electron create_electron(void)
{
    struct electron ele;
    srand(time(NULL));
    
    ele.x = 0.0;
    ele.y = rand()%101;
    ele.x_veleocity = rand()%20-10;
    ele.y_veleocity = rand()%20-10;
    
    /* printf("x = %f\n", ele.x);
    printf("y = %f\n", ele.y);
    printf("x_veleocity = %f\n", ele.x_veleocity);
    printf("y_veleocity = %f\n", ele.y_veleocity); */
    return ele;
}


//oblicza współrzędne i prędkość electrona po jednym cyklu
//boost to przespieszenie elektronu wzdlóż współrzędnej X. Tą stałą trzeba dopasować
//W naszych obliczeniach ona równa a = E*e/m
struct electron one_cycle(struct electron something, float boost)
{
    something.x_veleocity = something.x_veleocity + boost;
    
    something.x = something.x + something.x_veleocity;
    something.y = something.y + something.y_veleocity;

    return something;
    // printf("x = %f  y = %f  x_veleocity = %f  y_veleocity = %f\n", something.x, something.y, something.x_veleocity, something.y_veleocity);
}



//Tworzy siatke atomow 50 x 6 (300)
void create_atoms(struct XY * atom){
    int kolumna, wiersz, n;
    for(wiersz=0; wiersz<6 ;wiersz++)
    {
        for(kolumna=0; kolumna<50; kolumna++){
            n++;
            (atom+50*wiersz+kolumna) -> x=20*kolumna;
            (atom+50*wiersz+kolumna) -> y=20*wiersz;
        }
    }
    // for(k=0; k<300; k++){
    // printf("number atomu=%d  x=%f y=%f\n",k , (atom+k)->x, (atom+k)->y);
    // }
}


//zeruje prędkość x_veleocity jesli elektron zderza się z atomem i kontroluje że by nie wylatał z pola y(0;100)
//dla sprawdzenia udeżenia elektronu wykorzystuje się wzór (x-a)^2 + (x-b)^2 <= r^2
//cord_x = (x-a)^2
//cord_y = (y-a)^2
void collision_check(struct electron * ele, struct XY * atom){
    float cord_x, cord_y;
    float r2 = 25;
    int i = 0;
    
    if(ele->y > 100){
        ele->y=ele->y-100;
    }
    if(ele->y < 0){
        ele->y=ele->y+100;
    }
    for(i=0;i<300;i++){
        cord_x=(ele->x - (atom+i)->x)*(ele->x - (atom+i)->x);
        cord_y=(ele->y - (atom+i)->y)*(ele->y - (atom+i)->y);
        
        if((cord_x+cord_y)<r2){
            ele->x_veleocity=0;
            //printf("\n<<kolizja>>\nelek (%f, %f)\natom (%f, %f)\n", ele->x, ele->y, (atom+i)->x, (atom+i)->y);
        }
    }
}





// ignore
void fancy_result(float TOTAL_AVG_SPEED, float boost)
{
    fflush(stdout);
    printf("\n\n>>> TOTAL AVERAGE VELOCITY = %f", TOTAL_AVG_SPEED); fflush(stdout); sleep(1);
    printf("\n>>> SPEED-TO-VOLTAGE RATIO = %f", TOTAL_AVG_SPEED/boost); fflush(stdout); sleep(1); printf("\n\n");
}


 
//////////////////////////// MAIN ////////////////////////////
//////////////////////////// MAIN ////////////////////////////
///////////////// Wymiary pola X-1000 Y-100 //////////////////
int main()
{
    int cycle_num, nth_electron, num_of_electrons, LOOP_Nth_TIME;
    float avg_speed, sum_of_avg_speeds = 0, TOTAL_AVG_SPEED=0, boost;
    struct electron ele;
    struct XY atom[300];
    create_atoms(atom);

    printf("\nProgram symuluje losowy ruch elektronu w kawalku przewodu w zaleznosci od napiecia");
    printf("\noraz oblicza stosunek predkosci elektronu do tego napiecia");
    printf("\n[[Expected constant ratio, regardless of input data]].");
    printf("\n\nPodaj przespieszenie wywolane przez napiecie [m/s] = "); scanf("%f", &boost);

        
        printf("Podaj ilosc powtorzen symulacji = "); scanf("%d", &num_of_electrons);
        for(nth_electron = 0;     nth_electron < num_of_electrons;     nth_electron++)
        {
            ele = create_electron();
            cycle_num = 0;

            while(ele.x < 1000)
            {
                cycle_num++;
                ele = one_cycle(ele, boost);
                collision_check(&ele, atom);
            }
            avg_speed = (1000.0/cycle_num);     // V = s/t
            sum_of_avg_speeds = sum_of_avg_speeds + avg_speed;
            
            printf("\n[%d] Average velocity = %f", nth_electron+1, avg_speed); fflush(stdout); sleep(1);
        }
        TOTAL_AVG_SPEED = (sum_of_avg_speeds / (num_of_electrons*1.0));
        fancy_result(TOTAL_AVG_SPEED, boost);
        
    return 0;
}


