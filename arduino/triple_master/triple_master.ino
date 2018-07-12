#include <SoftwareSerial.h>
#include <CapacitiveSensor.h>

#define NUM_SAMPLES 30
#define THRESHOLD 5000

SoftwareSerial myComCat(8, 4); // RX, TX
SoftwareSerial myComDog(5, 6); // 5MISO, RX / 6MOSI, TX

CapacitiveSensor cs_2_3 = CapacitiveSensor(2, 3);

// Change this depending on the values coming
// fromthe sensor.
unsigned long average = 0;
unsigned long threshold = THRESHOLD;
unsigned long samples[NUM_SAMPLES];
int currSample = 0;
int maxSamples = NUM_SAMPLES;

void setup() {
  myComCat.begin(9600);
  myComDog.begin(9600);
}

void loop(){
  myComCat.listen();
  while (myComCat.available() > 0) {
    char inByte = myComCat.read();
    myComDog.write(inByte);
  }
  
  delay(100);

  unsigned long reading = cs_2_3.capacitiveSensor(30);

  if(currSample < maxSamples){
    // in the beginning we have to read the first NUM_SAMPLES samples
    // since the array is empty.
    samples[currSample] = reading;
    currSample++;

    myComCat.println("sampling...");
  }else{
    // when we are donre reading inthe NUM_SAMPLES readings
    // we have an array we can use for averaging
    // and we have to shift all readings of an array
    // we have to drop the first one and add one on top
    // so that we still have only NUM_SAMPLES readings

    // remove one sample from the beginning 
    // and add one sample at the end of the array
    for(int i = 0; i < maxSamples; i++){
      if(i < maxSamples - 1){
        samples[i] = samples[i+1];
      }else{
        samples[i] = reading;
      }
    }

    unsigned long sum = 0;
    for(int i = 0; i < maxSamples; i++){
      sum += samples[i];
    }
    
    unsigned long numSamples = NUM_SAMPLES;
    average = sum / numSamples;
    myComCat.print("sum: ");
    myComCat.print(sum);
    myComCat.print("\t");
    myComCat.print("average: ");
    myComCat.print(average);
    myComCat.print("\t");
    myComCat.print("reading: ");
    myComCat.println(reading);
        
    if (reading > average + threshold) {
      myComCat.println("Fingers move!");
      myComDog.write('1');
    }
  }
}

