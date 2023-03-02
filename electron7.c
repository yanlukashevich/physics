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
    float angle;
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
            (atom + 101*wiersz+kolumna) -> radius = 3;
        }
    }
}


struct electron_data spawn_electron()
{
    struct electron_data electron;
    srand(time(NULL));
    
    // Pozycja elektronu
    electron.radius = 0;
    electron.coordinate_x = 0.0;
    electron.coordinate_y = rand()%21+40;

    // Predkosc elektronu
    electron.x_velocity = 0;
    electron.y_velocity = rand()%6-10;

    // Kat poruszania sie
    electron.angle = atan2(electron.y_velocity, electron.x_velocity);
    
    return electron;
}


struct electron_data one_cycle(struct electron_data E, struct atom_data * atom, float boost, FILE *file)
{
    // Przesuniecie
    float dx = cos(E.angle);
    float dy = sin(E.angle);

    // Ruch
    E.x_velocity += 0.001 * boost;
    E.coordinate_x = E.coordinate_x + dx + E.x_velocity;
    E.coordinate_y = E.coordinate_y + dy;

    // Check if in bounds
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

    ///////////////////////// COLLISION HANDLER
    struct vector_data wektor_dwoch_srodkow;
    float dlugosc_wektora;
    int i;

    for(i=0; i<1110; i++)
    {
        // Wektor miedzy srodkami
        wektor_dwoch_srodkow.x = (atom+i)->coordinate_x - E.coordinate_x;    // Od E do atom
        wektor_dwoch_srodkow.y = (atom+i)->coordinate_y - E.coordinate_y;    // Od E do atom
        dlugosc_wektora = sqrt(wektor_dwoch_srodkow.x*wektor_dwoch_srodkow.x + wektor_dwoch_srodkow.y*wektor_dwoch_srodkow.y);

        /*
        // Korekcja elektronu jesli wleci do srodka atomu
        float collision_point_x = ((E.coordinate_x * (atom+i)->radius) + ((atom+i)->coordinate_x * E.radius)) / (E.radius + (atom+i)->radius);
        float collision_point_y = ((E.coordinate_y * (atom+i)->radius) + ((atom+i)->coordinate_y * E.radius)) / (E.radius + (atom+i)->radius);

        E.coordinate_x = collision_point_x - ((atom+i)->radius * wektor_dwoch_srodkow.x / dlugosc_wektora);
        E.coordinate_y = collision_point_y - ((atom+i)->radius * wektor_dwoch_srodkow.y / dlugosc_wektora);
        */

        // Kolizja
        if (dlugosc_wektora <= (atom+i)->radius)
        {
            printf("COLLISION\n");
            float dx_c = wektor_dwoch_srodkow.x; // pozioma odleglosc od srodka atomu
            float dy_c = wektor_dwoch_srodkow.y; // pionowa odleglosc od srodka atomu

            // Kat odbicia
            double theta = atan2(dy_c, dx_c);
            double phi = 2 * theta - E.angle;
            
            // update angle of travel
            E.angle = phi;
        }
    }

    return E;
}


/*
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
*/

/*
void fancy_result(float TOTAL_AVG_velocity, float boost)
{
    printf("\n>>> sr. predkosc");
    printf("\n--- %f\n", TOTAL_AVG_velocity);
    printf("\n>>> stosunek predkosci do przyspieszenia (V/a)");
    printf("\n--- %f", TOTAL_AVG_velocity/boost); printf("\n\n");
}
*/

//////////////////////////// MAIN ////////////////////////////
//////////////////////////// MAIN ////////////////////////////
//////////////////////////// MAIN ////////////////////////////
int main()
{
    float avg_velocity, boost;
    struct electron_data electron;
    struct atom_data atom[1111];
    create_atom_net(atom);

    ////////////////////////////////////////////////////////////////

    FILE *file = NULL;
    file = fopen("wykres.txt", "w+");
    if(file == NULL) {perror("\nERROR OCCURED >>"); return(13);}

    ////////////////////////////////////////////////////////////////

    printf("\nProgram symuluje losowy ruch elektronu w kawalku przewodu w zaleznosci przyspieszenia wywolanego przez napiecie");
    printf("\n\nPodaj przespieszenie.");
    printf("\n(W granicach 1 - 50)");
    printf("\na = "); scanf("%f", &boost);


    electron = spawn_electron();
    int cycle_num = 1;
    while(electron.coordinate_x < 1000)
    {
        cycle_num++;
        electron = one_cycle(electron, atom, boost, file);
        fprintf(file, "%f %f\n", electron.coordinate_x, electron.coordinate_y);
    }

    fclose(file);
    // avg_velocity = (1000.0/cycle_num);     // V = s/t
    // fancy_result(avg_velocity, boost);
        
    return 0;
}
