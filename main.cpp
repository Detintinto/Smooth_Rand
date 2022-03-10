#include <stdio.h>
#include <time.h>
#include "Map.h"
#include <random>
#include <conio.h>
#include <string>

#define RAND_ARR_QUANTITY 10
#define RAND_ARR_MAX_SIZE 10
#define RAND_ARR_MIN_SIZE 0

int* rough_rand_arr(int* rand_arr, int size, int max, int min);
int* smooth_rand_arr(int* arr, int size, unsigned long long max, unsigned long long min, int dif, int same_value_value);
void print_arr(int* arr, int size);
float cut_off_decimals(float num, int up_until);
std::string cut_off_zeros(float num, int up_until);

int main(int argc, char* argv[]) {

    int dif = 0;
    int sum_dif;
    int width, height;
    std::string title;
    time_t timer = 0;
    int svv = 10;

    if (argc == 1) {
        printf("Type in the value of same_value_value...");
        int scanned_items = scanf_s("%d", &svv);
        if (scanned_items == 0) {
            svv = 10;
        }
    }
    else if (argc == 2) {
        svv = atoi(argv[1]);
        if (svv == 0) {
            perror("The value must be a non-zero integer as well as it being greater than 0...\n");
        }
    }

    printf("Type in the width...");
    scanf_s("%d", &width);
    printf("Type in the height...");
    scanf_s("%d", &height);

Reset:
    sum_dif = 0;
    timer = clock();
    Map* map = init_Map(width, height);



    int* rand_arr = new int[map->width];
    rand_arr = smooth_rand_arr(rand_arr, map->width, map->height-map->height*0.1, 0, dif, svv);
    
    for (int i = 0; i < map->width-1; i++) {
        sum_dif += rand_arr[i + 1] - rand_arr[i];
    }

    for (int i = 0; i < map->height * map->width; i++) {
        map->map_buffer[i].Char.AsciiChar = ' ';
        map->map_buffer[i].Attributes = 0;
        if (i < 3 * map->width && rand() % 100 < 5) {
            map->map_buffer[i].Char.AsciiChar = '*';
            map->map_buffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;    
        }
    }

    for (int x = 0; x < map->width; x++) {
        for (int h = 0; h < rand_arr[x]; h++) {
            map->map_buffer[x + (map->height-h-1) * map->width].Char.AsciiChar = '#';
            map->map_buffer[x + (map->height -h-1) * map->width].Attributes = FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_GREEN | BACKGROUND_RED;
        }
    }

    for (int x = 0; x < map->width; x++) {
        for (int y = 0; y < map->height; y++) {
            if (map->map_buffer[x+ y * map->width].Char.AsciiChar == '#')
                continue;

            
        }
    }

    title = "Dif:" + std::to_string(dif);
    title += "; SVV: " + std::to_string(svv);
    title += "; Mean Dif: " + cut_off_zeros(cut_off_decimals((float)sum_dif/(float)map->width, 2), 3);

    timer = clock() - timer;
    title += "; Delay: " + std::to_string(timer);
    SetConsoleTitleA(title.c_str());

    print_Map(map);
    end_Map(map);

    delete[] rand_arr;
    while (1) {
        char input = _getch();
        if (input == '+') {
            if (dif != svv) {
                dif++;
                goto Reset;
            }
        }
        else if (input == '-') {
            if (dif != 0) {
                dif--;
                goto Reset;
            }
        }
        else if (input == '=') {
            goto Reset;
        }
        else if (input == '0') {
            break;
        }
    }
    return 0;
}

float cut_off_decimals(float num, int up_until) {
    
    num *= pow(10, up_until);
    num = (int)num;
    num = (float)num;
    num /= pow(10, up_until);

    return num;
}

std::string cut_off_zeros(float num, int up_until) {
    std::string temp = "";

    if (num < 0) {
        temp += "-";
        num *= -1;
    }
    else
        temp += "+";

    int x = num;
    temp += std::to_string(x);
    
    temp += ".";
    
    num = num - x;
    num *= pow(10, up_until);
    x = num;
    temp += std::to_string(x);

    return temp;
}

int* rough_rand_arr(int* rand_arr, int size, int max, int min) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        rand_arr[i] = (rand() % (max + 1)) + min;
        while (rand_arr[i] > max) { rand_arr[i]--; }
    }
    return rand_arr;
}
void print_arr(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        for (int x = 0; x < arr[i]; x++) {
            printf("#");
        }
        printf("\n");
    }
}
int* smooth_rand_arr(int* arr, int size, unsigned long long max, unsigned long long min, int dif, int same_value_value) {
    srand(time(0));

    // Gives the initial non-biased random values, which will be the base for all the other values.
    arr[0] = (rand() % (max - min)) + min;
    //printf("RAV: %d\n\n", arr[0]);

    int* probability_arr = new int[max-min];
    int  sum = 0;
    int  rand_val = 0;
    
    for (unsigned long long i = 1; i < size; i++) {

        // Fill probability array with values
        probability_arr[arr[i-1]-min] = same_value_value;
        for (int y = arr[i - 1] - 1 - min; y >= 0; y--) {
            probability_arr[y] = probability_arr[y + 1] - dif;
        }
        for (int y = arr[i - 1] + 1 - min; y < max - min; y++) {
            probability_arr[y] = probability_arr[y - 1] - dif;
        }


        // Sets values < 0 to 0 and gets x in rand() % x
        sum = 0;
        for (unsigned long long x = 0; x < max - min; x++) {
            if (probability_arr[x] <= 0) {
                probability_arr[x] = 0;
            }
            sum += probability_arr[x];
        }

        rand_val = (rand() % (sum))+1;
        sum = 0;

        int index = 0;
        for (int x = 0; x < max - min; x++) {
            if (index < rand_val && rand_val <= index + probability_arr[x]) {
                arr[i] = x;
                break;
            }
            index+=probability_arr[x];
        }
        sum = 0;
    }
    delete[] probability_arr;
    return arr;
}