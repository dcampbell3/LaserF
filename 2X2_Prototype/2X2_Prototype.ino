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
const int num_x_receivers = 2; 
const int num_y_receivers = 2; 

const int x_pins[num_x_receivers] = {A4, A5}; // stores pins for x receivers in idnex order
const int y_pins[num_y_receivers] = {A2, A1}; // stores pins for y receivers in index order

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
  samples[n].y_samples[0] = curr_y_samples[0];
  samples[n].y_samples[1] = curr_y_samples[1];
  //Serial.println(samples[n].y_samples[0]);
}

Result getResults(Sample samples[], int n, Receiver x_receivers[], Receiver y_receivers[]){

  Result ret; 
  
  float time_x1; 
  float time_x2; 
  float time_y1; 
  float time_y2; 
  
  for(int i = 0; i < n; i++){
    if(samples[i].x_samples[0] == 1){
      time_x1 = (samples[i].time_val - samples[0].time_val) / 1000;
    }

    if(samples[i].x_samples[1] == 1){
      time_x2 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    
    if(samples[i].y_samples[0] == 1){
      time_y1 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
    if(samples[i].y_samples[1] == 1){
      time_y2 = (samples[i].time_val - samples[0].time_val) / 1000;
    }
  }

  Serial.println("time_x1: " + String(time_x1)); 
  Serial.println("time_y1: " + String(time_y1));
  Serial.println("time_x2: " + String(time_x2));
  Serial.println("time_y2: " + String(time_y2));
  
  float point13[] = {x_receivers[0].location, y_receivers[0].location};
  Serial.println("Point13: " + String(point13[0]) + "," + String(point13[1])); 
  float point14[] = {x_receivers[0].location, y_receivers[1].location};
  Serial.println("Point14: " + String(point14[0]) + "," + String(point14[1]));
  float point23[] = {x_receivers[1].location, y_receivers[0].location};
  Serial.println("Point23: " + String(point23[0]) + "," + String(point23[1]));
  float point24[] = {x_receivers[1].location, y_receivers[1].location};
  Serial.println("Point24: " + String(point24[0]) + "," + String(point24[1]));
  
  // find the center point of the ball with a weighted average
  ret.x_pos = (point13[0]*time_x1 + point23[0]*time_x2 + point14[0]*time_x1 + point24[0]*time_x2) / (2*time_x1 + 2*time_x2);
  ret.y_pos = (point13[1]*time_y1 + point23[1]*time_y1 + point14[1]*time_y2 + point24[1]*time_y2) / (2*time_y1 + 2*time_y2);
 
  float* points[] = {point13, point14, point23, point24}; 
  float d = -100000.0; // very small value
  float min_weighted_point[] = {0,0};
  for(int i = 0; i < 4; i++){ 
    float dist = sqrt((*(points[i] + 0) - ret.x_pos)*(*(points[i] + 0) - ret.x_pos) + (*(points[i] + 1) - ret.x_pos)*(*(points[i] + 1) - ret.x_pos));
    if (dist > d){ 
      min_weighted_point[0] = points[i][0];
      min_weighted_point[1] = points[i][1]; 
    }
  }
//
//  Serial.println("Min-Point: " + String(min_weighted_point[0]) + "," + String(min_weighted_point[1]));
//  
  
  float velocity_calc_time;
  if(min_weighted_point[0] == point13[0] || min_weighted_point[0] == point14[0]){
    velocity_calc_time = time_x1;
  }
  else if(min_weighted_point[0] == point23[0] || min_weighted_point[0] == point24[0]){
    velocity_calc_time = time_x2;
  }
  
  ret.velocity = (2*(min_weighted_point[1] - ret.y_pos)) / velocity_calc_time;
  Serial.println("ret y_pos: " + String(ret.y_pos));
  return ret;
}

void setup() {
  Serial.begin(9600);
  
  start_time = millis(); 

  x_receivers[0] = Receiver{'x', 3, 8.0, A4, analogRead(A4) - 10};
  x_receivers[1] = Receiver{'x', 5, 12.0, A5, analogRead(A5) -10};
  y_receivers[0] = Receiver{'y', 3, 8.0, A2, analogRead(A2) - 10};
  y_receivers[1] = Receiver{'y', 5, 12.0, A1, analogRead(A1) - 10}; 
  
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
      //samples[iter_count] = {curr_x_samples, curr_y_samples, curr_time};
      
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
