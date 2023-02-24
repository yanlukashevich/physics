#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>


struct electron_data
{
    float coordinate_x;
    float coordinate_y;
    float x_velocity;
    float y_velocity;
    float radius;
};

struct atom_data
{
    float coordinate_x;
    float coordinate_y;
    float radius;
};

struct vector_data
{
    float x;
    float y;
};


//Tworzy siatke 1111 atomow. Odstepy miedzy atomami = 10. Poczatek w (0,0). Promien orbity atomu = 3.
void create_atom_net(struct atom_data * atom)
{
    int kolumna, wiersz;
    for(wiersz=0; wiersz <= 10; wiersz++)
    {
        for(kolumna=0; kolumna <= 100; kolumna++)
        {
            (atom + 101*wiersz+kolumna) -> coordinate_x = 10*kolumna;
            (atom + 101*wiersz+kolumna) -> coordinate_y = 10*wiersz;
            (atom + 101*wiersz+kolumna) -> radius = 1;
        }
    }
}


struct electron_data spawn_electron()
{
    struct electron_data electron;
    srand(time(NULL));
    
    electron.coordinate_x = 0.0;
    electron.coordinate_y = rand()%101;

    // Set starting velocity (0-10)
    electron.x_velocity = rand()%101;
    electron.y_velocity = rand()%11;
    
    return electron;
}


struct electron_data one_cycle(struct electron_data E, float boost, FILE *file)
{
    // dt - time step
    float dt = 0.01;
    // Increase (only) X velocity
    E.x_velocity = E.x_velocity + boost * dt;
    // Update position
    E.coordinate_x = E.coordinate_x + E.x_velocity * dt;
    E.coordinate_y = E.coordinate_y + E.y_velocity * dt;
    // Check if position is in boundries
    if(E.coordinate_y >= 100)
    {
        E.coordinate_y = 99;
        E.y_velocity = -E.y_velocity;
    }
    if(E.coordinate_y <= 0)
    {
        E.coordinate_y = 1;
        E.y_velocity = -E.y_velocity;
    }
    return E;
}


// Zachowuje kierunek wektora i zmienia jego dlugosc na '1'
struct vector_data normalize_vector(struct vector_data vector)
{
    // Vector of lenght 1, maintaining the same direction
    struct vector_data normalized_vector;
    float vector_length = sqrt((vector.x*vector.x) + (vector.y*vector.y));

    normalized_vector.x = vector.x / vector_length;
    normalized_vector.y = vector.y / vector_length;

    return normalized_vector;
}


// Oblicza iloczyn skalarny
float iloczyn_skalarny(struct vector_data vector_A, struct vector_data vector_B)
{
    float iloczyn = (vector_A.x * vector_B.x) + (vector_A.y * vector_B.y);
    return iloczyn;
}


struct electron_data collision_handler(struct electron_data E, struct atom_data * atom)
{
    int i;
    for(i=0; i<=1110; i++)
    {
        float odleglosc_srodkow = sqrt(((atom+i)->coordinate_x - E.coordinate_x)*((atom+i)->coordinate_x - E.coordinate_x)
                                     + ((atom+i)->coordinate_y - E.coordinate_y)*((atom+i)->coordinate_y - E.coordinate_y));

        if(odleglosc_srodkow <= (atom+i)->radius)
        {
            float correction_distance = (atom+i)->radius - odleglosc_srodkow;
            float angle = atan2((atom+i)->coordinate_y - E.coordinate_y, (atom+i)->coordinate_x - E.coordinate_x);
            // printf("\nANGLE = %f\n\n", angle);
            E.coordinate_x += cos(angle) * correction_distance;
            E.coordinate_y += sin(angle) * correction_distance;

            // Wektor prostopadly do (x, y) to (-y, x) (Tangent Vector)
            struct vector_data T_vector;
            T_vector.y = -(E.coordinate_x - (atom+i)->coordinate_x);
            T_vector.x =   E.coordinate_y - (atom+i)->coordinate_y;

            // Wektor prostopadly o dlugosci '1'
            struct vector_data T_normalized;
            T_normalized = normalize_vector(T_vector);

            // Same as the Electron's velocity, because the atom is stationary (V=0)
            struct vector_data relative_velocity;
            relative_velocity.x = E.x_velocity - 0;
            relative_velocity.y = E.y_velocity - 0;

            float length_of_velocity_parallel_to_tangent = iloczyn_skalarny(relative_velocity, T_normalized);

            struct vector_data velocity_perpendicular_to_tangent;
            velocity_perpendicular_to_tangent.x = T_normalized.x * length_of_velocity_parallel_to_tangent;
            velocity_perpendicular_to_tangent.y = T_normalized.y * length_of_velocity_parallel_to_tangent;

            E.x_velocity = E.x_velocity - 2 * velocity_perpendicular_to_tangent.x;
            E.y_velocity = E.y_velocity - 2 * velocity_perpendicular_to_tangent.y;
            // printf("\nX VEL = %f\n\nY VEL = %f\n\n", E.x_velocity, E.y_velocity);
            
            return E;
        }
        else;
    }
    return E;
}
    // If necessary, correct electron-atom intersecting and bounce
    






/*
//zeruje prędkość x_velocity jesli elektron zderza się z atomem i kontroluje że by nie wylatał z pola y(0;100)
//dla sprawdzenia udeżenia elektronu wykorzystuje się wzór (x-a)^2 + (x-b)^2 <= r^2
//cord_x = (x-a)^2
//cord_y = (y-a)^2
void collision_check(struct electron_data * ele, struct atom_net * atom){
    float cord_x, cord_y;
    float r2 = 25;
    int i = 0;
    
    if(ele->coordinate_y > 100)
    {
        ele->y_velocity = ele->y_velocity*(-1);
    }
    if(ele->coordinate_y < 0)
    {
        ele->y_velocity = ele->y_velocity*(-1);
    }
    for(i=0;i<300;i++)
    {
        cord_x=(ele->coordinate_x - (atom+i)->coordinate_x)*(ele->x - (atom+i)->x);
        cord_y=(ele->coordinate_y - (atom+i)->coordinate_y)*(ele->y - (atom+i)->y);
        
        if((cord_x+cord_y)<r2)
        {
            ele->x_velocity=0;
            //printf("colizja ele.x=%f ele.y=%f  XY.x=%f XY.y=%f\n", ele->x, ele->y, (atom+i)->x, (atom+i)->y);
        }
    }
}
*/

// ignore
void fancy_result(float TOTAL_AVG_SPEED, float boost)
{
    printf("\n>>> sr. predkosc");
    printf("\n--- %f\n", TOTAL_AVG_SPEED);
    printf("\n>>> stosunek predkosci do przyspieszenia (V/a)");
    printf("\n--- %f", TOTAL_AVG_SPEED/boost); printf("\n\n");
}



 
//////////////////////////// MAIN ////////////////////////////
//////////////////////////// MAIN ////////////////////////////
//////////////////////////// MAIN ////////////////////////////
int main()
{
    int cycle_num;
    float avg_speed, boost;
    struct electron_data electron;
    struct atom_data atom[1200];
    create_atom_net(atom);

    ////////////////////////////////////////////////////////////////

    FILE *file = NULL;
    file = fopen("wykres.txt", "w+");
    if(file == NULL) {perror("\nERROR OCCURED >>"); return(13);}

    printf("\nProgram symuluje losowy ruch elektronu w kawalku przewodu w zaleznosci od napiecia");
    printf("\noraz oblicza stosunek predkosci elektronu do przyspieszenia.");
    printf("\n\nPodaj przespieszenie wywolane przez napiecie [m/s2]");
    printf("\n(W granicach 50 - 200)");
    printf("\na = "); scanf("%f", &boost);

        
    electron = spawn_electron();
    cycle_num = 1;
    while(electron.coordinate_x < 1000)
    {
        cycle_num++;
        electron = one_cycle(electron, boost, file);
        electron = collision_handler(electron, atom);
        fprintf(file, "%f %f\n", electron.coordinate_x, electron.coordinate_y);
    }
    fclose(file);
    avg_speed = (1000.0/cycle_num);     // V = s/t
    fancy_result(avg_speed, boost);
        
    return 0;
}
