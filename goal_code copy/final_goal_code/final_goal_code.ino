// define struct for photoresistors
struct Receiver {
  char axis;
  int index;
  float location;
  int pin;
  int threshold;
};

struct Result {
  float x_pos; 
  float y_pos; 
  float velocity;
};

// define constants
const float ball_radius = 6.0; // inches
const float goal_width = 30.0; // inches
const float goal_height = 10.0; // inches
const float laser_spacing = 2 * ball_radius / 3; // inches

//const int num_x_receivers = floor(goal_width / laser_spacing);
//const int num_y_receivers = floor(goal_height / laser_spacing);
const int num_x_receivers = 3; 
const int num_y_receivers = 3; 

const int x_pins[num_x_receivers] = {A4, A5, A6}; // stores pins for x receivers in idnex order
const int y_pins[num_y_receivers] = {A2, A1, A0}; // stores pins for y receivers in index order

const int num_samples = 100;   

// initialize global variables
int laser_threshold; 
float start_time; 
bool kick_detected = false; 

Receiver x_receivers[num_x_receivers];
Receiver y_receivers[num_y_receivers];

// define struct for sampled data 
struct Sample { 
  int x_samples[num_x_receivers]; 
  int y_samples[num_y_receivers]; 
  float time_val;
};

Sample samples[num_samples];

// create helper functions
void assign_val(int n, int curr_x_samples[], int curr_y_samples[], float curr_time ){
  //Serial.println(curr_y_samples[0]); 
  samples[n] = Sample{*curr_x_samples, *curr_y_samples, 0.0}; 
  samples[n].time_val = curr_time;
  samples[n].x_samples[0] = curr_x_samples[0];
  samples[n].x_samples[1] = curr_x_samples[1];
  samples[n].x_samples[2] = curr_x_samples[2];
  samples[n].y_samples[0] = curr_y_samples[0];
  samples[n].y_samples[1] = curr_y_samples[1];
  samples[n].y_samples[2] = curr_y_samples[2];
  //Serial.println(samples[n].y_samples[0]);
}

Result getResults(Sample samples[], int n, Receiver x_receivers[], Receiver y_receivers[]){

  Result ret; 
  
  float time_x1; 
  float time_x2; 
  float time_x3;
  float time_y1; 
  float time_y2; 
  float time_y3;
  
  
  for(int i = 0; i < n; i++){
    if(samples[i].x_samples[0] == 1){
      time_x1 = (samples[i].time_val - samples[0].time_val) / 1000;
    }

    if(samples[i].x_samples[1] == 1){
      time_x2 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    if(samples[i].x_samples[2] == 1){
      time_x3 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    
    if(samples[i].y_samples[0] == 1){
      time_y1 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    if(samples[i].y_samples[1] == 1){
      time_y2 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    if(samples[i].y_samples[2] == 1){
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

  if(time_x1 > time_x3) {
    x1_location = x_receivers[0].location;
    x2_location = x_receivers[1].location;
    time_x1_calc = time_x1;
    time_x2_calc = time_x2; 
  }
  else if(time_x3 > time_x1){
    x1_location = x_receivers[1].location;
    x2_location = x_receivers[2].location; 
    time_x1_calc = time_x2; 
    time_x2_calc = time_x3; 
  }
  else{
    Serial.println("REDO KICK");
    x1_location = x_receivers[0].location;
    x2_location = x_receivers[1].location;
    time_x1_calc = time_x1;
    time_x2_calc = time_x2; 
  }

  if(time_y1 > time_y3) {
    y1_location = y_receivers[0].location;
    y2_location = y_receivers[1].location; 
    time_y1_calc = time_y1;
    time_y2_calc = time_y2;
  }
  else if(time_y3 > time_y1){
    y1_location = y_receivers[1].location;
    y2_location = y_receivers[2].location;  
    time_y1_calc = time_y2;
    time_y2_calc = time_y3;
  }
  else{
    Serial.println("REDO KICK");
    y1_location = y_receivers[0].location;
    y2_location = y_receivers[1].location; 
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
  ret.x_pos = (point11[0]*time_x1_calc + point21[0]*time_x2_calc + point12[0]*time_x1_calc + point22[0]*time_x2_calc) / (2*time_x1_calc + 2*time_x2_calc);
  ret.y_pos = (point11[1]*time_y1_calc + point21[1]*time_y1_calc + point12[1]*time_y2_calc + point22[1]*time_y2_calc) / (2*time_y1_calc + 2*time_y2_calc);
 
  float* points[] = {point11, point12, point21, point22}; 
  float d = -100000.0; // very small value
  float min_weighted_point[] = {0,0};
  for(int i = 0; i < 4; i++){ 
    float dist = sqrt((*(points[i] + 0) - ret.x_pos)*(*(points[i] + 0) - ret.x_pos) + (*(points[i] + 1) - ret.x_pos)*(*(points[i] + 1) - ret.x_pos));
    if (dist > d){ 
      min_weighted_point[0] = points[i][0];
      min_weighted_point[1] = points[i][1]; 
    }
  }
  
  float velocity_calc_time;
  if(min_weighted_point[0] == point11[0] || min_weighted_point[0] == point12[0]){
    velocity_calc_time = time_x1_calc;
  }
  else if(min_weighted_point[0] == point21[0] || min_weighted_point[0] == point22[0]){
    velocity_calc_time = time_x2_calc;
  }
  
  ret.velocity = (2*(min_weighted_point[1] - ret.y_pos)) / velocity_calc_time;
  return ret;
}

void setup() {
  Serial.begin(9600);
  
  start_time = millis(); 

  x_receivers[0] = Receiver{'x', 3, 8.0, A4, analogRead(A4) - 10};
  x_receivers[1] = Receiver{'x', 5, 12.0, A5, analogRead(A5) -10};
  x_receivers[2] = Receiver{'x', 7, 16.0, A6, analogRead(A6) -10}; 
  y_receivers[0] = Receiver{'y', 3, 8.0, A2, analogRead(A2) - 10};
  y_receivers[1] = Receiver{'y', 5, 12.0, A1, analogRead(A1) - 10}; 
  y_receivers[2] = Receiver{'y', 7, 16.0, A0, analogRead(A0) - 10}; 
  
  
//  for (int i = 0; i < num_x_receivers; i++) {
//    x_receivers[i] = Receiver{'x', i, (i + 1)*laser_spacing, 0};
//    pinMode(x_receivers[i].pin, OUTPUT);
//  }
//  
//  for (int i = 0; i < num_y_receivers; i++) {
//    y_receivers[i] = Receiver{'y', i, (i + 1)*laser_spacing, 0};
//    pinMode(y_receivers[i].pin, OUTPUT);
//  }
}

void loop() {
  for(int i = 0; i < num_x_receivers; i++){
    if(analogRead(x_receivers[i].pin) < x_receivers[i].threshold){
      kick_detected = true;
      break;
    }
  }
    
  for(int i = 0; i < num_y_receivers; i++){
    if(analogRead(y_receivers[i].pin) < y_receivers[i].threshold){
      kick_detected = true;
      break; 
    }
  }

  if(kick_detected){
    int iter_count = 0; 
    while(kick_detected){ 
      
      int curr_x_samples[num_x_receivers];
      int curr_y_samples[num_y_receivers]; 
      float curr_time; 
      int count_x_zeros = 0; 
      int count_y_zeros = 0; 
      
      for(int i = 0; i < num_x_receivers; i++){
        curr_x_samples[i] = analogRead(x_receivers[i].pin) < x_receivers[i].threshold;
        if(curr_x_samples[i] == 0){
          count_x_zeros++;
        }
      }
      
//      Serial.println("y_receivers: " + String(y_receivers[0].location) + "," + String(y_receivers[1].location));

      for(int i = 0; i < num_y_receivers; i++){
        curr_y_samples[i] = analogRead(y_receivers[i].pin) < y_receivers[i].threshold;
        if(curr_y_samples[i] == 0){
          count_y_zeros++;
        }
      }
      
      curr_time = millis();
      
      assign_val(iter_count, curr_x_samples, curr_y_samples, curr_time);
      
      iter_count++; 
      
      if(count_x_zeros == num_x_receivers && count_y_zeros == num_y_receivers){
        kick_detected = false; 
      }
      delay(10);
    }

//    Serial.println("Iter Count: " + String(iter_count));
    Result res = getResults(samples, iter_count, x_receivers, y_receivers);
    Serial.println("Center X Coordinate: " + String(res.x_pos));
    Serial.println("Center Y Coordinate: " + String(res.y_pos)); 
    Serial.println("Ball Velocity: " + String(res.velocity)); 
    Serial.print("\n");
    
    
  }
  kick_detected = false;

}
