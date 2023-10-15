#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double* newArray = new double[new_size];
  for(int i = 0; i < length; i++){
      newArray[i] = array[i];
  }
  for(int i = length; i < new_size; i++){
      newArray[i] = 0.0;
  }
  delete[] array;
  return newArray;
}

double* shrink_array(double* array, int length, int new_size) {
    double* newArray = new double[new_size];
    for(int i = 0; i < new_size; i++){
        newArray[i] = array[i];
    }
    delete[] array;
    return newArray;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
    if(current_size >= max_size){
        max_size += 5;
        array = extend_array(array, current_size, max_size);
    }
    array[current_size] = element;
    current_size++;
    return array;
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
    if(max_size - current_size >= 4){
        int new_max = max_size - 5;
        double* newArray = new double[new_max];
        for(int i = 0; i < current_size -1; i++){
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        max_size = new_max;
    }
    current_size--;
    return array;
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;// IMPLEMENT YOUR FUNCTION HERE
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {// IMPLEMENT YOUR FUNCTION HERE
    telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}
void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  for(int i = 0; i < tot_telemetries; i++){
    for(int j = 0; j < telemetries_sizes[i]; j++){
      global_telemetry = append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
    }
  }
  for(int i = 0; i < global_telemetry_current_size - 2; i += 3){
    for(int j = i + 3; j < global_telemetry_current_size; j += 3){
      if(global_telemetry[j] <= global_telemetry[i]){
        std::swap(global_telemetry[i], global_telemetry[j]);
        std::swap(global_telemetry[i+1], global_telemetry[j+1]);
        std::swap(global_telemetry[i+2], global_telemetry[j+2]);
      }
    }
  }
}

//  global_telemetry = new double[3];
//  global_telemetry_current_size = 0;
//  global_telemetry_max_size = 3;
//  int* grrr = new int[tot_telemetries];
//  for(int i = 0; i < tot_telemetries; i++){
//      grrr[i] = 0;
//  }
//  int index = -1;
//  double min_t = 1000000;
//  int number_of_arrays_to_check = tot_telemetries;
//  while(number_of_arrays_to_check > 0){
//      index = -1;
//      min_t = 1000000;
//      for(int i = 0; i < tot_telemetries; i++){
//          if(grrr[i] < telemetries_sizes[i] - 3 && telemetries[i][grrr[i]] < min_t){
//                  min_t = telemetries[i][grrr[i]];
//                  index = i;
//          }
//      }
//      for(int i = 0; i < 3; i++){
//          global_telemetry = append_to_array(telemetries[index][i], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
//      }
//      grrr[index] += 3;
//      if(grrr[index] == telemetries_sizes[index] - 3){
//          number_of_arrays_to_check --;
//      }
//  }
//  delete[] grrr;
//}














