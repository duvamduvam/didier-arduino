//------------------------------------------------------------------------------------------------------------
//                                                  DEBUG
//------------------------------------------------------------------------------------------------------------

#define DEBUG                 true
#define DEBUG_DMX             true
#define DEBUG_DMX_FADE        true
#define DEBUG_KEYBOARD        false
#define DEBUG_TAPSYNC         false

#define DEBUG_DMX_COL_WIDTH   4
#define DEBUG_DMX_LEFT_MARGIN 6

//------------------------------------------------------------------------------------------------------------
//                                                  CONFIGURATION
//------------------------------------------------------------------------------------------------------------

#define HEADER    "      SCANNER      |      ROLLER 1     |      ROLLER 2     "

// should match exactly the config
#define DMX_NB_PORTS        12 // MAX 140 (UNO memory) - MAX 255 (byte limit)


// Define The Ports (always incremental values, zero based)
#define SCAN1_LIGHT    0
#define SCAN1_COLOR    1
#define SCAN1_PAN     2
#define SCAN1_TILT     3

#define ROLLER1_LIGHT    4
#define ROLLER1_COLOR    5
#define ROLLER1_PAN     6
#define ROLLER1_ROLL     7

#define ROLLER2_LIGHT    8
#define ROLLER2_COLOR    9
#define ROLLER2_PAN     10
#define ROLLER2_ROLL     11


// Contest constants
#define CONTEST_SHUTTER_OPEN  35
#define CONTEST_COLOR_WHITE  17
#define CONTEST_COLOR_RED  34
#define CONTEST_COLOR_BLUE  51
#define CONTEST_COLOR_YELLOW  68
#define CONTEST_COLOR_CIRCLE  85
#define CONTEST_COLOR_ORANGE_FINGERPRINT  102
#define CONTEST_COLOR_ROSE_SPIRAL  119
#define CONTEST_COLOR_GREEN_LEAF  136
#define CONTEST_COLOR_BLUE_RED_FLOWER  153
#define CONTEST_COLOR_TURQUOISE_ASTERISK  170
#define CONTEST_COLOR_YELLOW_WHEEL  187
#define CONTEST_COLOR_MULTICOLOR_WHEEL  204
#define CONTEST_COLOR_BLUE_STARS  221
#define CONTEST_COLOR_RANDOM  238

// Define channels patchs (in "port" order)
const byte CHANNELS[] = 
{
    1, 2, 3, 4,     // SCAN 1
    5, 6, 7, 8,     // ROLLER 1
    21, 22, 23, 24, // ROLLER 2
};


//------------------------------------------------------------------------------------------------------------
//                                                  DMX PLAYER
//------------------------------------------------------------------------------------------------------------

#include <DmxSimple.h>

// Pins
#define PIN_LED_RYTHME      4
#define PIN_LED_BLACKOUT    5
#define PIN_DMX_OUT         2

// Rythm period
#define MIN_PERIOD_RYTHM      100
#define DEFAULT_PERIOD_RYTHM  1000
#define MAX_PERIOD_RYTHM      5000

// Fade config
#define FADE_STEPS      8

// Timecodes
uint32_t timeRythm;   // Rythm
uint32_t timeFlash;   // Rythm flash
uint32_t timeTapSync; // Tap sync
uint32_t timeFade;    // Fade

// Cache vars
byte   dmx[DMX_NB_PORTS];
float  steps[DMX_NB_PORTS];
byte   currentBank = 0;
byte   currentScene = 0;
byte   currentStep = 0;
byte   nextScene = 1;
int    periodRythm = DEFAULT_PERIOD_RYTHM;
int    periodFade;
bool   blackout = false; // Blackout mode
bool   fade = false;
bool   useCache = true;  // Pass to false for forcing changes
bool   useBlind = false; // Used for reading values from the bank, without sending DMX


void Dmx(int port, int value)
{
  if (useBlind)
  {
    // Calculte the fade step for this scene
    if (fade)
      DmxPopulateFadeStep(port, value);
  }
  // Already cached
  else if (useCache && (dmx[port] == value))
    return;

  else
  {
    // Send to DMX
    DmxSimple.write(CHANNELS[port], value);
  
    // Store in cache
    dmx[port] = value;   
  }
}

void DmxDebugConsoleScene()
{
  if (!DEBUG || !DEBUG_DMX)
    return;

  // Prepare the left margin description
  String strScene;
  strScene += currentScene; 
  if (fade)
    strScene = "~" + strScene;

  // Echo a grid row
  DmxDebugConsoleRowByte(dmx, strScene);


  // DEBUG : Print the fade steps offset values (+/-)
  if (fade && DEBUG_DMX_FADE)
  {
    int offsets[DMX_NB_PORTS];
    
    // Make readable the fade steps
    for (int port = 0 ; port < DMX_NB_PORTS ; port++)
    {
      // Decode the 128 centered
      offsets[port] = steps[port] - 128;
    }

    // Print the offsets values
    DmxDebugConsoleRowInt(offsets, "+/-");    
  }
}


void DmxDebugConsoleRowByte(byte values[], String strScene)
{

  // Prepare the margin description
  String line = HelperPadLeft(strScene, DEBUG_DMX_LEFT_MARGIN);
  line += "|";

  // Every column is a DMX port
  for (int port = 0 ; port < DMX_NB_PORTS ; port++)
  {
    
    byte value = values[port];
    line += HelperPadLeft(String(value), DEBUG_DMX_COL_WIDTH);
    line += "|";
  }

  // Print the row
  Serial.println(line);
}


//Copied from DmxDebugConsoleRowByte
void DmxDebugConsoleRowInt(int values[], String strScene)
{

  // Prepare the margin description
  String line = HelperPadLeft(strScene, DEBUG_DMX_LEFT_MARGIN);
  line += "|";

  // Every column is a DMX port
  for (int port = 0 ; port < DMX_NB_PORTS ; port++)
  {

    // HERE THE CHANGE
    // byte value = values[port];
    int value = values[port];
    
    line += HelperPadLeft(String(value), DEBUG_DMX_COL_WIDTH);
    line += "|";
  }

  // Print the row
  Serial.println(line);
}

void DmxDebugConsoleHeader()
{
  if (!DEBUG || !DEBUG_DMX)
    return;

  // Line break
  Serial.println("");

  // Text format of the header (pad the left margin)
  String header = "";
  for (byte j = 0 ; j < DEBUG_DMX_LEFT_MARGIN ; j++)
    header += " ";
  header += "|";
  header += HEADER;
  header += "|";
  
  // Echo the header
  Serial.println(header);

  // SEPARATOR
  DmxDebugConsoleSeparator();

  
  // CHANNELS
  DmxDebugConsoleRowByte(CHANNELS, "");
}


void DmxDebugConsoleSeparator()
{
  if (!DEBUG || !DEBUG_DMX)
    return;


  // Left margin padding
  String line = "";
  for (byte j = 0 ; j < DEBUG_DMX_LEFT_MARGIN ; j++)
      line += "-";

  // Begining
  line += "+";

  for (int port = 0 ; port < DMX_NB_PORTS ; port++)
  {    
    // Repeat '-' char for the column width
    for (byte j = 0 ; j < DEBUG_DMX_COL_WIDTH ; j++)
      line += "-";

    // Ending
    line += "+";
  }
  
  Serial.println(line);
}

void DmxPlayBank(byte bank)
{
  // Change the current bank
  currentBank = bank;

  // Reset the fade mode
  fade = false;


  // Reset the scene
  currentScene = 0;
  nextScene = 1;


  if (DEBUG)
  { 
    // styling
    if (DEBUG_DMX)
      Serial.println("");
    
    Serial.print("Play Bank: ");
    Serial.println(currentBank);

    // Show columns headers (Ports)
    if (DEBUG_DMX)
      DmxDebugConsoleHeader();
  }

  // Force to re-send all values
  useCache = false;
  
  // Start and syncronize the scene
  DmxTriggerChaser();

  // Use the cache
  useCache = true;
}

void DmxTriggerChaser()
{
  // Rythme flash light
  DmxFlashRythm();
  
  // Store the time of last trigger
  timeRythm = millis();

  // Do not execute the program (silent)
  if (blackout)
    return;

  // Reset the fade steps
  for (int port = 0 ; port < DMX_NB_PORTS ; port++)
    steps[port] = 128;

  // Store the current scene
  currentScene = nextScene;

  // Reset the current fade step
  currentStep = 0;

  // DEBUG : SEPARATOR
  if (currentScene == 1)
    DmxDebugConsoleSeparator();

  // Be sure to call and send DMX
  useBlind = false;

  // Choose the correct program to execute (bank)
  switch(currentBank)
  {
    case 1: nextScene = DmxBank1(nextScene); break;
    case 2: nextScene = DmxBank2(nextScene); break;
    case 3: nextScene = DmxBank3(nextScene); break;
    case 4: nextScene = DmxBank4(nextScene); break;
    case 5: nextScene = DmxBank5(nextScene); break;
    case 6: nextScene = DmxBank6(nextScene); break;
    case 7: nextScene = DmxBank7(nextScene); break;
    case 8: nextScene = DmxBank8(nextScene); break; 
  }

  if (fade)
  {
    // Only reading the bank for calculating the FADE next scene values
    useBlind = true;
    
    switch(currentBank)
    {
      case 1: DmxBank1(nextScene); break;
      case 2: DmxBank2(nextScene); break;
      case 3: DmxBank3(nextScene); break;
      case 4: DmxBank4(nextScene); break;
      case 5: DmxBank5(nextScene); break;
      case 6: DmxBank6(nextScene); break;
      case 7: DmxBank7(nextScene); break;
      case 8: DmxBank8(nextScene); break; 
    }

    // Restores the normal use
    useBlind = false;
  
  }

  //DEBUG : Print the DMX Scene state
  DmxDebugConsoleScene();
}

void DmxTriggerFade()
{
  // Store the time of last trigger
  timeFade = millis();

  // Do not execute the program (silent)
  if (blackout)
    return;

  // Increment the fade step
  currentStep ++;

  byte values[DMX_NB_PORTS];
  for (int port = 0 ; port < DMX_NB_PORTS ; port++)
  {
    // Per default take from cache
    if (DEBUG && DEBUG_DMX && DEBUG_DMX_FADE)
      values[port] = dmx[port];
    
    float step = steps[port];

    // Zero value (unchanged)
    if (step == 128)
      continue;

    // Read the previous state value from cache
    byte current = dmx[port];

    // Decode the 128 centered and round the value for DMX output
    byte value = (byte)(current + ((step-128) * currentStep));
    
    
    if (DEBUG && DEBUG_DMX && DEBUG_DMX_FADE)
      values[port] = value;
    
    // Send to DMX
    DmxSimple.write(CHANNELS[port], value);
  }



  String label = String(currentScene);
  label += ".";
  label += currentStep;
  
  
  // Echo the step values
  if (DEBUG && DEBUG_DMX && DEBUG_DMX_FADE)
    DmxDebugConsoleRowByte(values, label);
}

void DmxPopulateFadeStep(int port, int value)
{
  byte current = dmx[port];
  int gap = value - current;

  // Calculate and stores the step
  steps[port] = (gap / FADE_STEPS) + 128;
}

void DmxRefreshFadePeriod()
{
  // Calculate the fade period
  periodFade = (int)(periodRythm / FADE_STEPS);
}

void DmxLoopChaser()
{
  // FLASH RYTHM
  if (timeFlash && HelperChrono(timeFlash) > 50)
  {
    // Rythm flash OFF : After 50 ms
    digitalWrite(PIN_LED_RYTHME, LOW);
    timeFlash = 0;
  }

  // TAPSYNC - Max delay
  if (timeTapSync && HelperChrono(timeTapSync) > MAX_PERIOD_RYTHM)
  {
    // Stop the timer
    timeTapSync = 0;
  }
 
  // Trigger the next scene
  if (HelperChrono(timeRythm) > periodRythm)
  {
    DmxTriggerChaser(); 
  }

  // Trigger the next fade step
  else if (fade)
  {
    if (HelperChrono(timeFade) > periodFade)
    {
      // Trigger a fade step
      DmxTriggerFade();
    }
  }

}

void DmxFlashRythm()
{
  // Switch ON the LED
  digitalWrite(PIN_LED_RYTHME, HIGH);

  // Store the time of last flash on
  timeFlash = millis();
}

void DmxTapSync()
{
  // First tap
  if (!timeTapSync)
  {
    // Store the time of last flash on
    timeTapSync = millis();
  }
  else
  {
    // Measure the tap delay (period)
    int period = HelperChrono(timeTapSync);

    // Avoid bounces
    if (period < MIN_PERIOD_RYTHM)
      period = MIN_PERIOD_RYTHM;

    // From here a new rythm is defined 
    periodRythm = period;  

    // Refresh the fade period AFTER rythm changed
    DmxRefreshFadePeriod();

    // Reset the timecode
    timeTapSync = 0;

    // DEBUG : Print the rythm
    if (DEBUG && DEBUG_TAPSYNC)
    {   
      Serial.print("Period: ");   
      Serial.print(periodRythm);   
      Serial.println(" ms"); 

      int BPM = (int)(60000 / periodRythm);
      
      Serial.print("BPM: ");   
      Serial.println(BPM); 
    }

    // Syncronize the chaser starting at the second tap.
    DmxTriggerChaser();
  } 
}

void DmxToggleBlackout()
{
  // ENTER blackout
  if (!blackout)
  {
    // Blackout all (preset)
    DmxBlackout();

    // Switch ON the LED
    digitalWrite(PIN_LED_BLACKOUT, HIGH);

    // Enter mode
    blackout = true;

    // DEBUG : Print the blackout DMX state
    if (DEBUG)
    {
      if (DEBUG_DMX)
        DmxDebugConsoleRowByte(dmx, "B/O");
      else
        Serial.println("Blackout - IN");   
    }    
  }

  // EXIT blackout
  else
  {
    // Restore the cache
    for (int port = 0 ; port < DMX_NB_PORTS ; port ++)
      DmxSimple.write(CHANNELS[port], dmx[port]);

    // Switch OFF the LED
    digitalWrite(PIN_LED_BLACKOUT, LOW);
    
    // Exit mode
    blackout = false;

    if (DEBUG)
    {
      if (!DEBUG_DMX)
        Serial.println("Blackout - OUT");   
    }
  }
}

void DmxPlayerInit()
{
  // Pins layout
  pinMode(PIN_LED_RYTHME, OUTPUT);
  pinMode(PIN_LED_BLACKOUT, OUTPUT);
  DmxSimple.usePin(PIN_DMX_OUT);


  // Retrieve the higher channel used
  int higherChannel = 0;
  for (int port = 0 ; port < DMX_NB_PORTS; port ++)
  {
    int channel = CHANNELS[port];
    if (channel > higherChannel)
      higherChannel = channel;  
  }

  // Define the number of DMX channels
  DmxSimple.maxChannel(higherChannel);


  // Initialize fade period
  DmxRefreshFadePeriod();
    
  // Launch the first bank
  DmxPlayBank(1);
}




//------------------------------------------------------------------------------------------------------------
//                                                  KEYBOARD MATRIX
//------------------------------------------------------------------------------------------------------------

// Matrix config
#define KEYBOARD_COLS    4
#define KEYBOARD_ROWS    3

int keyboardPinRow[] = {11, 12, 13};
int keyboardPinCol[] = {7, 8, 9, 10};


// Cache vars
int keyboardLastKey;
int timeKey;

void KeyboardSeedRow(int seed)
{
  // Set up all rows (OFF)
  for (int row = 0 ; row < KEYBOARD_ROWS ; row++)
    digitalWrite(keyboardPinRow[row], HIGH); 

  // -1 is given to relax the keyboard
  if (seed == -1)
    return;

  // Seed the requested row
  digitalWrite(keyboardPinRow[seed], LOW);
}

int KeyboardReadRow(int row)
{
  // Key offset
  int offset = KEYBOARD_COLS * row;

  // Read if key is pressed
  for (int col = 0 ; col < KEYBOARD_COLS ; col++)
  {
    // Found a key, exit
    if (digitalRead(keyboardPinCol[col]) == 0) 
      return offset + col + 1;
  }

  // Not found
  return 0;
}

int KeyboardRead()
{
  // Anti-bouncing timer
  if (timeKey && HelperChrono(timeKey) < 100)
     return 0;

  // Matrix scan
  int found = 0;
  for (int row = 0 ; row < KEYBOARD_ROWS ; row++)
  {
    if (found)
      continue;

    // Seed a row
    KeyboardSeedRow(row);

    // Read a key on this row
    int key = KeyboardReadRow(row);

    // Found a key pressed
    if (key)
      found = key;
  }

  // Reset Matrix Seed
  KeyboardSeedRow(-1);

  // Anti bouncing
  if (found == keyboardLastKey)
    return 0;

  // Store the current key in cache
  keyboardLastKey = found;

  // Reset the anti-bouncing timecode
  if (found)
    timeKey = millis();
    
  // DEBUG
  if (DEBUG && DEBUG_KEYBOARD)
    if (found)
    {
      Serial.print("Key: ");
      Serial.println(found);
    }

  return found;
}

void KeyboardInit()
{
  // Digital output for seeding
  for (int row = 0 ; row < KEYBOARD_ROWS ; row++)
    pinMode(keyboardPinRow[row], OUTPUT);

  // Pullup input for reading
  for (int col = 0 ; col < KEYBOARD_COLS ; col++)
    pinMode(keyboardPinCol[col], INPUT_PULLUP);
}

//------------------------------------------------------------------------------------------------------------
//                                                  HELPERS
//------------------------------------------------------------------------------------------------------------
uint32_t HelperChrono(uint32_t timer)
{
  uint32_t now = millis();
  uint32_t dif = now-timer;
  return dif;
}

String HelperPadLeft(String str, int totalLength)
{
  // Format the source string into array
  int stringLength = str.length() + 1;

  char sourceString[stringLength];
  str.toCharArray(sourceString, stringLength);

  // Init the output string
  char padded[totalLength + 1];
  
  // Prepare the replacement pattern
  String stringPattern = "%";
  stringPattern += totalLength;
  stringPattern += "s";
  int lenPattern = 6;
  char pattern[lenPattern];
  stringPattern.toCharArray(pattern, lenPattern);

  
  sprintf(padded, pattern, sourceString);

  return padded;
}


//------------------------------------------------------------------------------------------------------------
//                                                  SETUP
//------------------------------------------------------------------------------------------------------------
void setup() 
{
  
  if (DEBUG)
  {
    // Init Debug console
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600); 
  }


  // Matrix Keyboard
  KeyboardInit();

  // DMX Player
  DmxPlayerInit();

}

//------------------------------------------------------------------------------------------------------------
//                                                  LOOP
//------------------------------------------------------------------------------------------------------------


void loop() 
{
  // Keyboard
  switch (KeyboardRead())
  {
    case 1: DmxTapSync(); break;
    case 4: DmxToggleBlackout(); break;

    case 5: DmxPlayBank(1); break;
    case 6: DmxPlayBank(2); break;
    case 7: DmxPlayBank(3); break;
    case 8: DmxPlayBank(4); break;
    case 9: DmxPlayBank(5); break;
    case 10: DmxPlayBank(6); break;
    case 11: DmxPlayBank(7); break;
    case 12: DmxPlayBank(8); break;
    
  }


  // DMX Player loop
  DmxLoopChaser();

}



//------------------------------------------------------------------------------------------------------------
//                                                  CONDUIT
//------------------------------------------------------------------------------------------------------------


// Blackout preset
void DmxBlackout()
{
  Dmx(SCAN1_LIGHT, 0);
  Dmx(ROLLER1_LIGHT, 0);
  Dmx(ROLLER2_LIGHT, 0);

  // Silent wheel (colors) - (Only when "random color" is used)
  if (dmx[SCAN1_COLOR] >= CONTEST_COLOR_RANDOM)
    Dmx(SCAN1_COLOR, CONTEST_COLOR_BLUE_STARS);
    
  if (dmx[ROLLER1_COLOR] >= CONTEST_COLOR_RANDOM)
    Dmx(ROLLER1_COLOR, CONTEST_COLOR_BLUE_STARS);
    
  if (dmx[ROLLER2_COLOR] >= CONTEST_COLOR_RANDOM)
    Dmx(ROLLER2_COLOR, CONTEST_COLOR_BLUE_STARS);


  // Silent Rollers
  Dmx(ROLLER1_ROLL, 128);
  Dmx(ROLLER1_ROLL, 128); 
}


byte DmxBank1(int scene)
{
  // Switch on
  Dmx(SCAN1_LIGHT, CONTEST_SHUTTER_OPEN);
  Dmx(ROLLER1_LIGHT, CONTEST_SHUTTER_OPEN);
  Dmx(ROLLER2_LIGHT, CONTEST_SHUTTER_OPEN);

  switch(scene)
  {
    case 1:
      Dmx(SCAN1_COLOR, CONTEST_COLOR_RANDOM);
      Dmx(SCAN1_PAN, 128);
      Dmx(SCAN1_TILT, 128);
      break;


    case 2:
      Dmx(SCAN1_COLOR, CONTEST_COLOR_RANDOM);
      Dmx(SCAN1_PAN, 0);
      Dmx(SCAN1_TILT, 128);
      break;

 
    case 3:
      Dmx(SCAN1_COLOR, CONTEST_COLOR_RANDOM);
      Dmx(SCAN1_PAN, 0);
      Dmx(SCAN1_TILT, 0);
      break;


    case 4:
      Dmx(SCAN1_COLOR, 150);
      Dmx(SCAN1_PAN, 255);
      Dmx(SCAN1_TILT, 0);


      // Last scene - Reset to first step
      scene = 0;
      break;
  }

  // Return the next scene step
  return scene +1;
}

// Test Fade
byte DmxBank2(int scene)
{
  fade = true;

  
  // Switch on
  Dmx(SCAN1_LIGHT, CONTEST_SHUTTER_OPEN);
  Dmx(ROLLER1_LIGHT, CONTEST_SHUTTER_OPEN);
  Dmx(ROLLER2_LIGHT, CONTEST_SHUTTER_OPEN);

  Dmx(SCAN1_PAN, 128);
  Dmx(SCAN1_TILT, 128);
      
  switch(scene)
  {
    case 1:
      Dmx(SCAN1_COLOR, 200); // Couleur
      break;
      
    case 2:
      Dmx(SCAN1_COLOR, 50); // Couleur
      break;

    case 3:
      Dmx(SCAN1_COLOR, 100); // Couleur

      // Last scene - Reset to first step
      scene = 0;
      break;

      
  }

  // Return the next scene step
  return scene +1;
}

// Test Shutter
byte DmxBank3(int scene)
{
  // Center position
  Dmx(SCAN1_PAN, 128);
  Dmx(SCAN1_TILT, 128);


  Dmx(SCAN1_LIGHT, scene * 5);

  if (scene >= 25)
    scene = 0;
    

  // Return the next scene step
  return scene +1;
}

// Scan all values
byte DmxBank4(int scene)
{
  // Define here the port to scan
  int scanPort = SCAN1_COLOR;
  
  // Open Shutter
  Dmx(SCAN1_LIGHT, CONTEST_SHUTTER_OPEN);
  
  // Center position
  Dmx(SCAN1_PAN, 128);
  Dmx(SCAN1_TILT, 128);



  Dmx(scanPort, scene);

  if (scene >= 255)
    scene = 0;
    

  // Return the next scene step
  return scene +1;
}

// Scanner Colors
byte DmxBank5(int scene)
{
  // Switch on
  Dmx(SCAN1_LIGHT, CONTEST_SHUTTER_OPEN);

  // Center
  Dmx(SCAN1_PAN, 128);
  Dmx(SCAN1_TILT, 128);
      
  switch(scene)
  {
    case 1: Dmx(SCAN1_COLOR, CONTEST_COLOR_WHITE); break;
    case 2: Dmx(SCAN1_COLOR, CONTEST_COLOR_RED); break;
    case 3: Dmx(SCAN1_COLOR, CONTEST_COLOR_BLUE); break;
    case 4: Dmx(SCAN1_COLOR, CONTEST_COLOR_YELLOW); break;
    case 5: Dmx(SCAN1_COLOR, CONTEST_COLOR_CIRCLE); break;
    case 6: Dmx(SCAN1_COLOR, CONTEST_COLOR_ORANGE_FINGERPRINT); break;
    case 7: Dmx(SCAN1_COLOR, CONTEST_COLOR_ROSE_SPIRAL); break;
    case 8: Dmx(SCAN1_COLOR, CONTEST_COLOR_GREEN_LEAF); break;
    case 9: Dmx(SCAN1_COLOR, CONTEST_COLOR_BLUE_RED_FLOWER); break;
    case 10: Dmx(SCAN1_COLOR, CONTEST_COLOR_TURQUOISE_ASTERISK); break;
    case 11: Dmx(SCAN1_COLOR, CONTEST_COLOR_YELLOW_WHEEL); break;
    case 12: Dmx(SCAN1_COLOR, CONTEST_COLOR_MULTICOLOR_WHEEL); break;
    case 13: Dmx(SCAN1_COLOR, CONTEST_COLOR_BLUE_STARS); break;


    case 14: Dmx(SCAN1_COLOR, CONTEST_COLOR_MULTICOLOR_WHEEL); break;
    case 15: Dmx(SCAN1_COLOR, CONTEST_COLOR_YELLOW_WHEEL); break;
    case 16: Dmx(SCAN1_COLOR, CONTEST_COLOR_TURQUOISE_ASTERISK); break;
    case 17: Dmx(SCAN1_COLOR, CONTEST_COLOR_BLUE_RED_FLOWER); break;
    case 18: Dmx(SCAN1_COLOR, CONTEST_COLOR_GREEN_LEAF); break;
    case 19: Dmx(SCAN1_COLOR, CONTEST_COLOR_ROSE_SPIRAL); break;
    case 20: Dmx(SCAN1_COLOR, CONTEST_COLOR_ORANGE_FINGERPRINT); break;
    case 21: Dmx(SCAN1_COLOR, CONTEST_COLOR_CIRCLE); break;
    case 22: Dmx(SCAN1_COLOR, CONTEST_COLOR_YELLOW); break;
    case 23: Dmx(SCAN1_COLOR, CONTEST_COLOR_BLUE); break;
    case 24: Dmx(SCAN1_COLOR, CONTEST_COLOR_RED); 
    
      // Last scene - Reset to first step
      scene = 0;
    break;

      
  }

  // Return the next scene step
  return scene +1;

  
}

byte DmxBank6(int scene)
{
  
}

byte DmxBank7(int scene)
{
  
}

byte DmxBank8(int scene)
{
 

}





