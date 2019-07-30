#define beat1 350
#define beat2 700
#define DECAY 0.012
float prevPow = 1.0;

int cl(int value) {
  if (value < 0) {
    value = 0;
  }
  else if (value > 255) {
    value = 255;
  }
  return value;
}
void light(int r, int g, int b) {
  printf("%d %d %d\n", r, g, b);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(r, g, b);
  }
}
void setPallet(int r, int g, int b) {
  printf("%d %d %d\n", r, g, b);

  CRGB norm = CRGB(r, g, b);
  CRGB  hr = CRGB(cl(r + VAR), g, b);
  CRGB hg = CRGB(r, cl(g + VAR), b);
  CRGB hb = CRGB(r, g, cl(b + VAR));
  CRGB  lr = CRGB(cl(r - VAR), g, b);
  CRGB lg = CRGB(r, cl(g - VAR), b);
  CRGB lb = CRGB(r, g, cl(b - VAR));
  currentPalette = CRGBPalette16(norm, hr, hg, hb,
                                 norm, lr, lg , lb,
                                 norm, hr, hg, hb,
                                 norm, lr, lg , lb);
}
void storeSolid(int r, int g, int b){
  solidColour[0] = r;
  solidColour[1] = g;
  solidColour[2] = b;
}
void processRgb(String data, void func(int, int, int)) {
  int nums[3] = {0, 0, 0};
  int index = 0;
  int maxIndex = 2;
  for (int i = 0; i < data.length(); i++) {
    char c = (char) data[i];
    if (index <= maxIndex) {
      if (c != '(' && c != ')') {
        if (c == ',') {
          index++;
        }
        else {
          nums[index] = nums[index] * 10;
          int dig = c - '0';
          nums[index] += dig;
        }
      }
    }
  }

  //light(nums[0],nums[1],nums[2]);
  func(nums[0], nums[1], nums[2]);
}
void fill( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}
void beat(uint16_t ind){
  float new_ind = (float) ind;
  float power = 0.0;
  if(ind == 0 || ind == beat1){
    power = 1.0;
  }
  else{
    power = prevPow - DECAY;
  }
  if(power < 0.0){
    power = 0.0;
  }
  else if(power > 1.0){
    power = 1.0;
  }
  prevPow = power;
  Serial.print("ind ");
  Serial.println(ind);
  Serial.print("power ");
  Serial.println(power*255.0);
  light(power*solidColour[0], power*solidColour[1], power*solidColour[2]);
}

