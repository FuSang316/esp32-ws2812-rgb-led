 /*
 用 ESP32 制造炫彩 LED 球
 使用 Neopixel BUS 库控制LED
 Arduino IDE 源代码：*/

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
 
#define PIN_LEDS 21
#define NUMPIXELS 194
 
#define LIGHTNESS 0.05f
#define BRIGHTNESS 60 //亮度
 
RgbColor BLACK(0, 0, 0);
RgbColor RED(BRIGHTNESS, 0, 0);
RgbColor GREEN(0, BRIGHTNESS, 0);
RgbColor BLUE(0, 0, BRIGHTNESS);
RgbColor YELLOW(BRIGHTNESS, BRIGHTNESS, 0);
RgbColor WHITE(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);
 
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> leds(NUMPIXELS, PIN_LEDS);
 
NeoPixelAnimator animations(10); // 新像素动画管理对象
 
#define NUM_RINGS 11
#define RING_MAX_PIXELS 26 //定义环_最大_像素
#define RING_MIN_PIXELS 8 //定义环_最小_像素
 
// 所有的环从0射线开始
byte RINGS[NUM_RINGS][RING_MAX_PIXELS] = {
  {78, 79, 80, 81, 82, 83, 84, 77}, // 8
  {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 63, 64}, // 14
  {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 45, 46, 47}, // 18
  {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 25, 26, 27}, // 20
  {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 1, 2, 3, 4}, // 24
  {102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 110, 109, 108, 107, 106, 105, 104, 103}, // 26
  {127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 134, 133, 132, 131, 130, 129, 128}, // 24
  {148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 154, 153, 152, 151, 150, 149}, // 20
  {167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 172, 171, 170, 169, 168}, // 18
  {182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 186, 185, 184, 183}, // 14
  {192, 191, 190, 189, 188, 187, 194, 193} // 8
};
 
byte RING_SIZES[NUM_RINGS] = {8, 14, 18, 20, 24, 26, 24, 20, 18, 14, 8};
 
unsigned long startMillis = 0;
short animation = 0;
 
void setup() {
  Serial.begin(115200);
  initRandom();
 
  leds.Begin();
  leds.ClearTo(BLACK);
  leds.Show();
  
 /*可以变化的 6 个动画为：1、垂直圆模式；2、水平圆模式；3、垂直彩虹模式；4、水平彩虹模式；
 5、彩虹模式:单色渐变；6、随机模式。*/
  animations.StartAnimation(0, 6000, rainbowAnimation); //彩虹模式:单色渐变
  animations.StartAnimation(0, 2000, raysRainbow); //水平彩虹模式
  animations.StartAnimation(0, 2000, ringsRainbow); //垂直彩虹模式
  animations.StartAnimation(0, 800, white_and_red_OrbAnimation);  //水平圆模式：0、5、10白色长亮、3、7红白水平替换滚动，<1、2、4、6、8、9红色长亮> 
  animations.StartAnimation(0, 800, blue_and_red_OrbAnimation);  //水平圆模式：0、5、10红色长亮、3、7红蓝水平替换滚动，<1、2、4、6、8、9蓝色长亮>
  animations.StartAnimation(0, 100, randomAnimation); //随机模式
  //*****************************************************************
  animations.StartAnimation(0, 1000, BLUEAnimation); //垂直圆模式：蓝环
  animations.StartAnimation(0, 1000, GREENAnimation); //垂直圆模式：绿环
  animations.StartAnimation(0, 1000, REDAnimation); //垂直圆模式：红环
  animations.StartAnimation(0, 1000, WHITEAnimation); //垂直圆模式：白环
  animations.StartAnimation(0, 1000, blue_red_Animation); //垂直圆模式：蓝色长亮,红环上下
  animations.StartAnimation(0, 1000, green_red_Animation);  //垂直圆模式：绿色长亮,红环上下
  animations.StartAnimation(0, 1000, white_red_Animation);  //垂直圆模式：红色长亮,白环上下
  //*****************************************************************
  //垂直环动态单色环
  animations.StartAnimation(0, 500, BLUE_Animation);  //水平圆模式:蓝环
  animations.StartAnimation(0, 500, RED_Animation);  //水平圆模式:红环
  animations.StartAnimation(0, 500, GREEN_Animation);  //水平圆模式:绿环
  animations.StartAnimation(0, 500, WHITE_Animation);  //水平圆模式:白环
}
 
void loop() {
  animations.UpdateAnimations();
 
  leds.Show();
  delay(10);
 
  if (true && (startMillis == 0 || startMillis + 20000 < millis())) { // 30s false or true
    startMillis = millis();
    switch (animation) {
      case 0: 
        animations.StartAnimation(0, 800, white_and_red_OrbAnimation); //水平圆模式：0、5、10白色长亮、3、7红白水平替换滚动，<1、2、4、6、8、9红色长亮> 
        break;
      case 1: 
        animations.StartAnimation(0, 1000, BLUEAnimation); //垂直圆模式：蓝环
        break;
      case 2: 
        animations.StartAnimation(0, 1000, GREENAnimation); //垂直圆模式：绿环
        break;
      case 3: 
        animations.StartAnimation(0, 1000, REDAnimation); //垂直圆模式：红环
        break;
      case 4: 
        animations.StartAnimation(0, 1000, WHITEAnimation); //垂直圆模式：白环
        break;
      case 5: 
        animations.StartAnimation(0, 1000, blue_red_Animation); //垂直圆模式：蓝色长亮,红环上下
        break;
      case 6: 
        animations.StartAnimation(0, 1000, green_red_Animation); //垂直圆模式：绿色长亮,红环上下
        break;
      case 7: 
        animations.StartAnimation(0, 1000, white_red_Animation); //垂直圆模式：红色长亮,白环上下
        break;
      case 8: 
        animations.StartAnimation(0, 2000, ringsRainbow); //垂直彩虹模式
        break;
      case 9: 
        animations.StartAnimation(0, 2000, raysRainbow); //水平彩虹模式
        break;
      case 10: 
        animations.StartAnimation(0, 6000, rainbowAnimation); //彩虹模式:单色渐变
        break;
      case 11:
        animations.StartAnimation(0, 100, randomAnimation); //随机模式
        break;
      case 12:
        animations.StartAnimation(0, 500, BLUE_Animation);  //水平圆模式:蓝环
        break;
      case 13:
        animations.StartAnimation(0, 500, RED_Animation);  //水平圆模式:红环
        break;
      case 14:
        animations.StartAnimation(0, 500, GREEN_Animation);  //水平圆模式:绿环
        break;
      case 15:
        animations.StartAnimation(0, 500, WHITE_Animation);  //水平圆模式:白环
        break;
      case 16:
        animations.StartAnimation(0, 800, blue_and_red_OrbAnimation); //水平圆模式：0、5、10红色长亮、3、7红蓝水平替换滚动，<1、2、4、6、8、9蓝色长亮>
        animation = 0;
        break;
    }
    animation ++;
  }
}
 
void randomAnimation(const AnimationParam& param) {
  float hue;
  HslColor color;
 
  if (param.state == AnimationState_Completed) {
    for (byte i = 0; i < 194; i ++) {
      hue = random(0, 1000) / 1000.0f;
      color = HslColor(hue, 1.0f, LIGHTNESS);
      leds.SetPixelColor(i, color);
    }
 
    animations.RestartAnimation(0);
  }
}
 
void rainbowAnimation(const AnimationParam& param) {
  HslColor color = HslColor(param.progress, 1.0f, LIGHTNESS);
  leds.ClearTo(color);
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
 
void raysRainbow(const AnimationParam& param) {
  HslColor color;
  float hue;
 
  for (int i = 0; i < RING_MAX_PIXELS; i++) {
    hue = param.progress + (float) i / (float) RING_MAX_PIXELS;
    if (hue > 1.0f) {
      hue -= 1.0f;
    }
 
    color = HslColor(hue, 1.0f, LIGHTNESS);
    rayColor(i, RgbColor(color));
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
 
void ringsRainbow(const AnimationParam& param) {
  HslColor color;
  float hue;
 
  for (int i = 0; i < NUM_RINGS; i++) {
    hue = param.progress + (float) i / (float) NUM_RINGS;
    if (hue > 1.0f) {
      hue -= 1.0f;
    }
 
    color = HslColor(hue, 1.0f, LIGHTNESS);
    ringColor(i, RgbColor(color));
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//*****************************************************************
//0、5、10白色长亮、3、7红白水平替换滚动，<1、2、4、6、8、9红色长亮> 
void white_and_red_OrbAnimation(const AnimationParam& param) {
  ringColor(0, WHITE);
  ringColor(1, RED);
  ringColor(2, RED);
  ringColor(3, RED);
  ringColor(4, RED);
  ringColor(5, WHITE);
  ringColor(6, RED);
  ringColor(7, RED);
  ringColor(8, RED);
  ringColor(9, RED);
  ringColor(10, WHITE);
 
  byte offset = round(param.progress);
  for (byte i = offset; i < RING_SIZES[3]; i+=2) {
    leds.SetPixelColor(RINGS[3][i] - 1, WHITE);
  }
  for (byte i = offset; i < RING_SIZES[7]; i+=2) {
    leds.SetPixelColor(RINGS[7][i] - 1, WHITE);
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//*****************************************************************
//0、5、10红色长亮、3、7红蓝水平替换滚动，<1、2、4、6、8、9蓝色长亮>
void blue_and_red_OrbAnimation(const AnimationParam& param) {
  ringColor(0, RED);
  ringColor(1, BLUE);
  ringColor(2, BLUE);
  ringColor(3, BLUE);
  ringColor(4, BLUE);
  ringColor(5, RED);
  ringColor(6, BLUE);
  ringColor(7, BLUE);
  ringColor(8, BLUE);
  ringColor(9, BLUE);
  ringColor(10, RED);
 
  byte offset = round(param.progress);
  for (byte i = offset; i < RING_SIZES[3]; i+=2) {
    leds.SetPixelColor(RINGS[3][i] - 1, RED);
  }
  for (byte i = offset; i < RING_SIZES[7]; i+=2) {
    leds.SetPixelColor(RINGS[7][i] - 1, RED );
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//*****************************************************************
//BLUE旋转
void BLUEAnimation(const AnimationParam& param) {
  int index = param.progress * (NUM_RINGS * 2 - 2);
 
  leds.ClearTo(BLACK);
  if (index < NUM_RINGS) {
    ringColor(index, BLUE);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, BLUE);
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//GREEN旋转
void GREENAnimation(const AnimationParam& param) {
  int index = param.progress * (NUM_RINGS * 2 - 2);
 
  leds.ClearTo(BLACK);
  if (index < NUM_RINGS) {
    ringColor(index, GREEN);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, GREEN);
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//RED旋转
void REDAnimation(const AnimationParam& param) {
  int index = param.progress * (NUM_RINGS * 2 - 2);
 
  leds.ClearTo(BLACK);
  if (index < NUM_RINGS) {
    ringColor(index, RED);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, RED);
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//WHITE旋转
void WHITEAnimation(const AnimationParam& param) {
  int index = param.progress * (NUM_RINGS * 2 - 2);
 
  leds.ClearTo(BLACK);
  if (index < NUM_RINGS) {
    ringColor(index, WHITE);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, WHITE);
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//*************************************************************************
void blue_red_Animation(const AnimationParam& param) {   //垂直圆模式：蓝色长亮,红环上下
  int index = param.progress * (NUM_RINGS * 2 - 2);
 
  leds.ClearTo(BLUE);
  if (index < NUM_RINGS) {
    ringColor(index, RED);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, RED);
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
void green_red_Animation(const AnimationParam& param) {   //垂直圆模式：绿色长亮,红环上下
  int index = param.progress * (NUM_RINGS * 2 - 2);
 
  leds.ClearTo(GREEN);
  if (index < NUM_RINGS) {
    ringColor(index, RED);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, RED);
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
void white_red_Animation(const AnimationParam& param) {   //垂直圆模式：红色长亮,白环上下
  int index = param.progress * (NUM_RINGS * 2 - 2);
 
  leds.ClearTo(RED);
  if (index < NUM_RINGS) {
    ringColor(index, WHITE);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, WHITE);
  }
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//**************************************************************************
//垂直环动态单色环
//蓝色
void BLUE_Animation(const AnimationParam& param) {
  int index = param.progress * (RING_MAX_PIXELS / 2);
 
  if (index > 12) {
    index = 12;
  }
  leds.ClearTo(BLACK);
  rayColor(index, BLUE);
  rayColor(index + (RING_MAX_PIXELS / 2), BLUE);
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//绿色
void GREEN_Animation(const AnimationParam& param) {
  int index = param.progress * (RING_MAX_PIXELS / 2);
 
  if (index > 12) {
    index = 12;
  }
  leds.ClearTo(BLACK);
  rayColor(index, GREEN);
  rayColor(index + (RING_MAX_PIXELS / 2), GREEN);
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//红色
void RED_Animation(const AnimationParam& param) {
  int index = param.progress * (RING_MAX_PIXELS / 2);
 
  if (index > 12) {
    index = 12;
  }
  leds.ClearTo(BLACK);
  rayColor(index, RED);
  rayColor(index + (RING_MAX_PIXELS / 2), RED);
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//白色
void WHITE_Animation(const AnimationParam& param) {
  int index = param.progress * (RING_MAX_PIXELS / 2);
 
  if (index > 12) {
    index = 12;
  }
  leds.ClearTo(BLACK);
  rayColor(index, WHITE);
  rayColor(index + (RING_MAX_PIXELS / 2), WHITE);
 
  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
//**************************************************************************

void rayColor(byte rayIndex, RgbColor color) {
  int pixelIndex;
  byte pixel;
 
  if (rayIndex >= RING_MAX_PIXELS) {
    return; // 防止越界
  }
 
  for (byte i = 0; i < NUM_RINGS; i ++) {
    pixelIndex = round((float) RING_SIZES[i] / (float) RING_MAX_PIXELS * rayIndex);
    pixel = RINGS[i][pixelIndex];
    if (pixel == 0) {
      continue; // 跳过条件
    }
    leds.SetPixelColor(pixel - 1, color); // 索引从1开始(0是停止条件)
  }
}
 
void ringColor(byte ringIndex, RgbColor color) {
  byte pixel;
 
  if (ringIndex >= NUM_RINGS) {
    return; // 防止越界
  }
 
  for (byte i = 0; i < RING_MAX_PIXELS; i ++) {
    pixel = RINGS[ringIndex][i];
    if (pixel == 0) {
      return; // 结束条件
    }
    leds.SetPixelColor(pixel - 1, color); // 索引从1开始(0是停止条件)
  }
}
 
void initRandom() {
  // random works best with a seed that can use 31 bits
  // 未连接引脚上的模拟读数倾向于少于4位
  uint32_t seed = analogRead(0);
  delay(1);
 
  for (int shifts = 3; shifts < 31; shifts += 3) {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }
 
  // Serial.println(seed);
  randomSeed(seed);
}
 
RgbColor colorWheel(byte wheelPos) {
  // 输入一个0到255的值来获得一个颜色值。
  // 颜色是从r - g - b -回到r的过渡
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85) {
    return RgbColor(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170) {
    wheelPos -= 85;
    return RgbColor(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return RgbColor(wheelPos * 3, 255 - wheelPos * 3, 0);
}
