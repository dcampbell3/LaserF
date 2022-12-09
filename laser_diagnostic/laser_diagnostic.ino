
// define struct for photoresistors
struct Receiver {
  char axis;
  int index;
  float location;
  int pin;
};

const int num_x_receivers = 16;
const int num_y_receivers = 11;
Receiver x_receivers[num_x_receivers];
Receiver y_receivers[num_y_receivers]; 

const int x1 = 23;
const int x2 = 6;
const int x3 = 27;
const int x4 = 34;
const int x5 = 36;
const int x6 = 38;
const int x7 = 40;
const int x8 = 42;
const int x9 = 45; 
const int x10 = 47; 
const int x11 = 49; 
const int x12 = 22; 
const int x13 = 24; 
const int x14 = 26; 
const int x15 = 28; 
const int x16 = 30;

const int y1 = 29;
const int y2 = 31;
const int y3 = 33;
const int y4 = 44;
const int y5 = 46;
const int y6 = 48;
const int y7 = 50;
const int y8 = 52;
const int y9 = 39; 
const int y10 = 41;
const int y11 = 43;


void setup() {
  Serial.begin(9600);
  
  pinMode(x1, INPUT);
  pinMode(x2, INPUT);
  pinMode(x3, INPUT);
  pinMode(x4, INPUT);
  pinMode(x5, INPUT);
  pinMode(x6, INPUT);
  pinMode(x7, INPUT);
  pinMode(x8, INPUT);
  pinMode(x9, INPUT); 
  pinMode(x10, INPUT); 
  pinMode(x11, INPUT); 
  pinMode(x12, INPUT); 
  pinMode(x13, INPUT);
  pinMode(x14, INPUT); 
  pinMode(x15, INPUT); 
  pinMode(x16, INPUT);

  pinMode(y1, INPUT);
  pinMode(y2, INPUT);
  pinMode(y3, INPUT);
  pinMode(y4, INPUT);
  pinMode(y5, INPUT);
  pinMode(y6, INPUT);
  pinMode(y7, INPUT);
  pinMode(y8, INPUT);
  pinMode(y9, INPUT); 
  pinMode(y10, INPUT); 
  pinMode(y11, INPUT); 
  
  // initialize all x receivers!!
 
  x_receivers[0] = Receiver{'x', 1, 4.0, x1};
  x_receivers[1] = Receiver{'x', 2, 8.0, x2};
  x_receivers[2] = Receiver{'x', 3, 12.0, x3}; 
  x_receivers[3] = Receiver{'x', 4, 16.0, x4};
  x_receivers[4] = Receiver{'x', 5, 20.0, x5};
  x_receivers[5] = Receiver{'x', 6, 24.0, x6};
  x_receivers[6] = Receiver{'x', 7, 28.0, x7};
  x_receivers[7] = Receiver{'x', 8, 32.0, x8};
  x_receivers[8] = Receiver{'x', 9, 36.0, x9};
  x_receivers[9] = Receiver{'x', 10, 40.0, x10};
  x_receivers[10] = Receiver{'x', 11, 44.0, x11};
  x_receivers[11] = Receiver{'x', 12, 48.0, x12};
  x_receivers[12] = Receiver{'x', 13, 52.0, x13};
  x_receivers[13] = Receiver{'x', 14, 56.0, x14};
  x_receivers[14] = Receiver{'x', 15, 60.0, x15};
  x_receivers[15] = Receiver{'x', 16, 64.0, x16};

  // initialize all y receivers!! 
  y_receivers[0] = Receiver{'y', 1, 4.0, y1};
  y_receivers[1] = Receiver{'y', 2, 8.0, y2};
  y_receivers[2] = Receiver{'y', 3, 12.0, y3};
  y_receivers[3] = Receiver{'y', 4, 16.0, y4};
  y_receivers[4] = Receiver{'y', 5, 20.0, y5};
  y_receivers[5] = Receiver{'y', 6, 24.0, y6};
  y_receivers[6] = Receiver{'y', 7, 28.0, y7};
  y_receivers[7] = Receiver{'y', 8, 32.0, y8};
  y_receivers[8] = Receiver{'y', 9, 36.0, y9};
  y_receivers[9] = Receiver{'y', 10, 40.0, y10};
  y_receivers[10] = Receiver{'y', 11, 44.0, y11};
  
}

void loop() {

  for(int i = 0; i < num_x_receivers; i++){ 
    Serial.print(String(x_receivers[i].axis) + String(x_receivers[i].index) + ": " + String(digitalRead(x_receivers[i].pin)) + " ");
  }
//  Serial.println("");
//  for(int i = 0; i < num_y_receivers; i++){ 
//    Serial.print(String(y_receivers[i].axis) + String(y_receivers[i].index) + ": " + String(digitalRead(y_receivers[i].pin)) +" ");
//  }
//  Serial.print("\n\n");

  Serial.println("");

  delay(10);

}
