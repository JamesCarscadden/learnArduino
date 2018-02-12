// Define which pins the various inputs on the matrix board
// are connected to
const int LEDARRAY_D = 45;
const int LEDARRAY_C = 43;
const int LEDARRAY_B = 41;
const int LEDARRAY_A = 39;
const int LEDARRAY_G = 37;
const int LEDARRAY_DI = 35;
const int LEDARRAY_CLK = 33;
const int LEDARRAY_LAT = 31;

// For some reason which would probably be clear to an electrical
//  engineer, or someone who could read chip specs, to turn on
//  an LED in the matrix we use a LOW signal, and to turn it
//  off we use a high. Note that these aren't actually used
//  in the code below, I just include it as a reminder.
const int ON = LOW;
const int OFF = HIGH;

// Values needed to write each row of LEDs from top to bottom when
//  connector to arduino is in top-left. These correspond to
//  pins D, C, B and A.
const int ROW[16][4] =
{
  {LOW, LOW,  LOW,  LOW},
  {LOW, LOW,  LOW,  HIGH},
  {LOW, LOW,  HIGH, LOW},
  {LOW, LOW,  HIGH, HIGH},

  {LOW, HIGH, LOW,  LOW},
  {LOW, HIGH, LOW,  HIGH},
  {LOW, HIGH, HIGH, LOW},
  {LOW, HIGH, HIGH, HIGH},

  {HIGH, LOW, LOW, LOW},
  {HIGH, LOW, LOW, HIGH},
  {HIGH, LOW, HIGH, LOW},
  {HIGH, LOW, HIGH, HIGH},

  {HIGH, HIGH, LOW, LOW},
  {HIGH, HIGH, LOW, HIGH},
  {HIGH, HIGH, HIGH, LOW},
  {HIGH, HIGH, HIGH, HIGH}
};

// Set up our pins for output
void setup() {
  // put your setup code here, to run once:
  pinMode(LEDARRAY_D, OUTPUT); 
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);

  Serial.begin(9600);
}

// Toggle a particular pin HIGH/LOW
void toggle_pin(int pin)
{
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

// Get ready to write a row of data
//  turns the display off, then makes sure that Clock and Latch are reset
void start_write()
{
  digitalWrite(LEDARRAY_G, OFF);  // G Pin appears to toggle the whole display on/off

  // Reset clock and latch
  digitalWrite(LEDARRAY_CLK, LOW);
  digitalWrite(LEDARRAY_LAT, LOW);
}

// Writes a value into the buffer array
//  toggle clock to move to the next slot
void write_value(int value)
{
  digitalWrite(LEDARRAY_DI, value);
  toggle_pin(LEDARRAY_CLK);
}

// Transmit the buffer data by toggling latch
void send_write()
{
  // Data is transmitted when the latch is toggled (rising edge)
  toggle_pin(LEDARRAY_LAT);
}

// Transmit the latched data to the row
void send_row(int rowIndex)
{
  digitalWrite(LEDARRAY_D, ROW[rowIndex][0]);
  digitalWrite(LEDARRAY_C, ROW[rowIndex][1]);
  digitalWrite(LEDARRAY_B, ROW[rowIndex][2]);
  digitalWrite(LEDARRAY_A, ROW[rowIndex][3]); 
}

// Complete our write process by turning the display back on.
void end_write()
{
  digitalWrite(LEDARRAY_G, ON);
}

// Write a single row of data to the matrix
//  runs from right to left when the arduino connector is at the top-left
//  note that we are reading the rowValues backwards though, so the value
//  array should be set-up as normal english left to right.
// We declare rowValues specifically as 16 since that is the width of
//  our matrix
void write_row(int rowIndex, int rowValues[16])
{
  start_write();
  
  int i = 0;
  for (i = 15; i >= 0; i--)
  {
    write_value(1 - rowValues[i]);
  }
  
  send_write();
  send_row(rowIndex);
  end_write();
  delayMicroseconds(100); // Without this, some rows appear dim
}

const int messageLength = 31;
// "Hello"
const int message[16][messageLength] =
  {
    {1,1,0,0,1,1,0,1,1,1,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1},
    {1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1},
    {1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1},
    {1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1},
    {1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,0,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
  };

const int displaySpeed = 30;

template< typename T, size_t N >
void compositeMatrix(T (&rowValues)[N], int offset, int workingRow[])
{
  int i = 0;
  for (i = 0; i < 16; i++)
  {

    // assuming our offset is between 0 and messageLenth, add that
    //  part of the rowValue to the workingRow, otherwise add 0
    if (((offset + i) < N) && ((offset + i) >= 0))
    {
      workingRow[i] = rowValues[offset + i];
    } else {
      workingRow[i] = 0;
    }
  }
}

template< typename T, size_t N, size_t X >
void scroll( T (&rowValues)[N][X], int scrollSpeed)
{
  int i = 0;
  int j = 0;

  Serial.println("    Enter scroll");
  // We start at -16 here so that we start with a blank display
  for (j = -16; j < 31; j++)
  {

    String out = "        scroll loop: ";
    String out2 = out + j;
    Serial.println(out2);
    // This loop controls how fast the message scrolls by
    //  displaying the same image 'scrollSpeed' times
    //  with a 300 microsecond delay per image
    //  So '30' speed * 100 microseconds means each
    //  frame is displayed for 3000 microseconds
    int k = 0;
    for (k = 0; k < scrollSpeed; k++)
    {
      // This loop controls the display of a single frame
      //  of the message, by writing 16 rows of data
      for (i = 0; i < 16; i++)
      {   
        int displayRow[16] = {0};
        compositeMatrix(rowValues[i], j, displayRow);
        write_row(i, displayRow);
      }
    }
  }

  Serial.println("    Exit scroll");
}

void loop() {
  Serial.println("Starting loop");
  // put your main code here, to run repeatedly:
  scroll( message, displaySpeed);
  Serial.println("Ending loop");

  delay(1000);
}
