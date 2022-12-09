
#include <digitalWriteFast.h>
#include <Arduino.h>

// pin constants
const int x1 = 23;
const int x2 = 6;
const int x3 = 27;
const int x4 = 34;
const int x5 = 36;
const int x6 = 38;
const int x7 = 40;
const int x8 = 42;

const int y1 = 29;
const int y2 = 31;
const int y3 = 33;
const int y4 = 44;
const int y5 = 46;
const int y6 = 48;
const int y7 = 50;
const int y8 = 52;

// setup info
const int num_x_receivers = 8;
const int num_y_receivers = 8;
const int num_samples = 100;

// define struct for photoresistors
struct Receiver {
  char axis;
  int index;
  float location;
  int pin;
};

Receiver x_receivers[num_x_receivers];
Receiver y_receivers[num_y_receivers];

// define struct to store kick result
struct Result {
  float x_pos;
  float y_pos;
  float velocity;
};

// define struct for sampled data
struct Sample {
  int x_samples[num_x_receivers];
  int y_samples[num_y_receivers];
  float time_val;
};

// create array of Samples
Sample samples[num_samples];

// create helper functions
int sort_desc(const void *cmp1, const void *cmp2)
{
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
  return a > b ? -1 : (a < b ? 1 : 0);
}

void assign_val(int n, int curr_x_samples[], int curr_y_samples[], float curr_time ) {
  //Serial.println(curr_y_samples[0]);
  samples[n] = Sample{*curr_x_samples, *curr_y_samples, 0.0};
  samples[n].time_val = curr_time;
  for (int i = 0; i < num_x_receivers; i++) {
    samples[n].x_samples[i] = curr_x_samples[i];
  }
  for (int j = 0; j < num_y_receivers; j++) {
    samples[n].y_samples[j] = curr_y_samples[j];
  }
}

// perform calculations from sampled data
Result getResults(Sample samples[], int n, Receiver x_receivers[], Receiver y_receivers[]) {

  Result ret;

  float time_x1;
  float time_x2;
  float time_x3 = -1;
  float time_y1;
  float time_y2;
  float time_y3 = -1;

  int x_inds[] = { -1, -1, -1};
  int y_inds[] = { -1, -1, -1};

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < num_x_receivers; j++) {
      if (samples[i].x_samples[j] == 1) {
        if (x_inds[0] == -1) {
          x_inds[0] = j;
        }
        else if (x_inds[0] != j && x_inds[1] == -1) {
          x_inds[1] = j;
        }
        else if (x_inds[0] != j && x_inds[1] != j && x_inds[2] == -1) {
          x_inds[2] = j;
        }
      }
    }
    for (int j = 0; j < num_y_receivers; j++) {
      if (samples[i].y_samples[j] == 1) {
        if (y_inds[0] == -1) {
          y_inds[0] = j;
        }
        else if (y_inds[0] != j && y_inds[1] == -1) {
          y_inds[1] = j;
        }
        else if (y_inds[0] != j && y_inds[1] != j && y_inds[2] == -1) {
          y_inds[2] = j;
        }
      }
    }
  }

  qsort(x_inds, sizeof(x_inds) / sizeof(x_inds[0]), sizeof(x_inds[0]), sort_desc);
  qsort(y_inds, sizeof(y_inds) / sizeof(y_inds[0]), sizeof(y_inds[0]), sort_desc);

  int x_ind1 = x_inds[0];
  int x_ind2 = x_inds[1];
  int x_ind3 = x_inds[2];

  int y_ind1 = y_inds[0];
  int y_ind2 = y_inds[1];
  int y_ind3 = y_inds[2];

  Serial.println("X indices: " + String(x_ind1) + " " + String(x_ind2) + " " + String(x_ind3));
  Serial.println("Y indices: " + String(y_ind1) + " " + String(y_ind2) + " " + String(y_ind3));

  for (int i = 0; i < n; i++) {
    if (samples[i].x_samples[x_ind1] == 1) {
      time_x1 = (samples[i].time_val - samples[0].time_val) / 1000;
    }

    if (samples[i].x_samples[x_ind2] == 1) {
      time_x2 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    if (samples[i].x_samples[x_ind3] == 1 && x_ind3 != -1 ) {
      time_x3 = (samples[i].time_val - samples[0].time_val) / 1000;
    }

    if (samples[i].y_samples[y_ind1] == 1) {
      time_y1 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    if (samples[i].y_samples[y_ind2] == 1) {
      time_y2 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    if (samples[i].y_samples[y_ind3] == 1 && y_ind3 != -1) {
      time_y3 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
  }

  Serial.println("time_x1: " + String(time_x1));
  Serial.println("time_y1: " + String(time_y1));
  Serial.println("time_x2: " + String(time_x2));
  Serial.println("time_y2: " + String(time_y2));
  Serial.println("time_x3: " + String(time_x3));
  Serial.println("time_y3: " + String(time_y3));

  // 1 corresponds to leftmost or bottommost laser
  // 2 corresponds to rightmost or topmost laser

  float x1_location;
  float x2_location;
  float y1_location;
  float y2_location;

  float time_x1_calc;
  float time_x2_calc;
  float time_y1_calc;
  float time_y2_calc;

  if (time_x1 > time_x3) {
    x1_location = x_receivers[x_ind1].location;
    x2_location = x_receivers[x_ind2].location;
    time_x1_calc = time_x1;
    time_x2_calc = time_x2;
  }
  else if (time_x3 > time_x1) {
    x1_location = x_receivers[x_ind2].location;
    x2_location = x_receivers[x_ind3].location;
    time_x1_calc = time_x2;
    time_x2_calc = time_x3;
  }
  else {
    Serial.println("REDO KICK");
    x1_location = x_receivers[x_ind1].location;
    x2_location = x_receivers[x_ind2].location;
    time_x1_calc = time_x1;
    time_x2_calc = time_x2;
  }

  if (time_y1 > time_y3) {
    y1_location = y_receivers[y_ind1].location;
    y2_location = y_receivers[y_ind2].location;
    time_y1_calc = time_y1;
    time_y2_calc = time_y2;
  }
  else if (time_y3 > time_y1) {
    y1_location = y_receivers[y_ind2].location;
    y2_location = y_receivers[y_ind3].location;
    time_y1_calc = time_y2;
    time_y2_calc = time_y3;
  }
  else {
    Serial.println("REDO KICK");
    y1_location = y_receivers[y_ind1].location;
    y2_location = y_receivers[y_ind2].location;
    time_y1_calc = time_y1;
    time_y2_calc = time_y2;
  }

  float point11[] = {x1_location, y1_location};
  Serial.println("Point11: " + String(point11[0]) + "," + String(point11[1]));
  float point12[] = {x1_location, y2_location};
  Serial.println("Point12: " + String(point12[0]) + "," + String(point12[1]));
  float point21[] = {x2_location, y1_location};
  Serial.println("Point21: " + String(point21[0]) + "," + String(point21[1]));
  float point22[] = {x2_location, y2_location};
  Serial.println("Point22: " + String(point22[0]) + "," + String(point22[1]));

  // find the center point of the ball with a weighted average
  ret.x_pos = (point11[0] * time_x1_calc + point21[0] * time_x2_calc + point12[0] * time_x1_calc + point22[0] * time_x2_calc) / (2 * time_x1_calc + 2 * time_x2_calc);
  ret.y_pos = (point11[1] * time_y1_calc + point21[1] * time_y1_calc + point12[1] * time_y2_calc + point22[1] * time_y2_calc) / (2 * time_y1_calc + 2 * time_y2_calc);

  float* points[] = {point11, point12, point21, point22};
  float d = -100000.0; // very small value
  float min_weighted_point[] = {0, 0};
  for (int i = 0; i < 4; i++) {
    float dist = sqrt((*(points[i] + 0) - ret.x_pos) * (*(points[i] + 0) - ret.x_pos) + (*(points[i] + 1) - ret.x_pos) * (*(points[i] + 1) - ret.x_pos));
    if (dist > d) {
      min_weighted_point[0] = points[i][0];
      min_weighted_point[1] = points[i][1];
    }
  }

  float velocity_calc_time;
  if (min_weighted_point[0] == point11[0] || min_weighted_point[0] == point12[0]) {
    velocity_calc_time = time_x1_calc;
  }
  else if (min_weighted_point[0] == point21[0] || min_weighted_point[0] == point22[0]) {
    velocity_calc_time = time_x2_calc;
  }

  ret.velocity = (2 * (min_weighted_point[1] - ret.y_pos)) / velocity_calc_time;
  return ret;
}
// initialize global variables
bool kick_detected = false;

void setup() {

  delay(1000);

  Serial.begin(9600);

  pinMode(x1, INPUT);
  pinMode(x2, INPUT);
  pinMode(x3, INPUT);
  pinMode(x4, INPUT);
  pinMode(x5, INPUT);
  pinMode(x6, INPUT);
  pinMode(x7, INPUT);
  pinMode(x8, INPUT);

  pinMode(y1, INPUT);
  pinMode(y2, INPUT);
  pinMode(y3, INPUT);
  pinMode(y4, INPUT);
  pinMode(y5, INPUT);
  pinMode(y6, INPUT);
  pinMode(y7, INPUT);
  pinMode(y8, INPUT);

  // initialize all x receivers!!
  x_receivers[0] = Receiver{'x', 1, 4.0, x1};
  x_receivers[1] = Receiver{'x', 2, 8.0, x2};
  x_receivers[2] = Receiver{'x', 3, 12.0, x3};
  x_receivers[3] = Receiver{'x', 4, 16.0, x4};
  x_receivers[4] = Receiver{'x', 5, 20.0, x5};
  x_receivers[5] = Receiver{'x', 6, 24.0, x6};
  x_receivers[6] = Receiver{'x', 7, 28.0, x7};
  x_receivers[7] = Receiver{'x', 8, 32.0, x8};

  // initialize all y receivers!!
  y_receivers[0] = Receiver{'y', 1, 4.0, y1};
  y_receivers[1] = Receiver{'y', 2, 8.0, y2};
  y_receivers[2] = Receiver{'y', 3, 12.0, y3};
  y_receivers[3] = Receiver{'y', 4, 16.0, y4};
  y_receivers[4] = Receiver{'y', 5, 20.0, y5};
  y_receivers[5] = Receiver{'y', 6, 24.0, y6};
  y_receivers[6] = Receiver{'y', 7, 28.0, y7};
  y_receivers[7] = Receiver{'y', 8, 32.0, y8};

}

void loop() {

  // detect kick in x axis
  for (int i = 0; i < num_x_receivers; i++) {
    if (digitalRead(x_receivers[i].pin) == 1 && i != 6 && i != 2) {
      Serial.println("Kick detected at x"  + String(x_receivers[i].index));
      kick_detected = true;
      break;
    }
  }

  // detect kick in y axis
  for (int i = 0; i < num_y_receivers; i++) {
    if (digitalRead(y_receivers[i].pin) ==  1 && i != 2 && i != 6) {
      Serial.println("Kick detected at y" + String(y_receivers[i].index) + " i=" + String(i));
      kick_detected = true;
      break;
    }
  }

  // take samples
  if (kick_detected) {

    int iter_count = 0; // sample num

    while (kick_detected) {

      int curr_x_samples[num_x_receivers]; // array of x samples
      int curr_y_samples[num_y_receivers]; // array of y samples
      float curr_time; // current time
      int count_x_zeros = 0; // number of x lasers tripped
      int count_y_zeros = 0; // number of y lasers tripped

      for (int i = 0; i < num_x_receivers; i++) {
        curr_x_samples[i] = digitalRead(x_receivers[i].pin);
        if (curr_x_samples[i] == 0) {
          count_x_zeros++;
        }
      }

      for (int i = 0; i < num_y_receivers; i++) {
        curr_y_samples[i] = digitalRead(y_receivers[i].pin);
        if (curr_y_samples[i] == 1) {
          count_y_zeros++;
        }
      }

      curr_time = millis();

      assign_val(iter_count, curr_x_samples, curr_y_samples, curr_time);

      iter_count++;

      if (count_x_zeros == num_x_receivers && count_y_zeros == num_y_receivers) {
        kick_detected = false;
      }

      delay(10);

    }

    Result res = getResults(samples, iter_count, x_receivers, y_receivers);
    Serial.println("Center X Coordinate: " + String(res.x_pos));
    Serial.println("Center Y Coordinate: " + String(res.y_pos));
    Serial.println("Ball Velocity: " + String(res.velocity));
    Serial.println("");

    //write code to UI
    Serial.println(String(x_receivers[found_x].location + rand_x) + "," + String(y_receivers[found_y].location + rand_y));

    kick_detected = false;

  }
  
  kick_detected = false;
  
}
