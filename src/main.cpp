#include <Arduino.h>
#include <TimeLib.h>
#include <dht.h>

dht1wire DHT(7, dht::DHT11);

#define TIME_HEADER "T"

void processSyncMessage() {
	unsigned long pctime;
	const unsigned long DEFAULT_TIME = 1357041600;  // Jan 1 2013

	if (Serial.find(TIME_HEADER)) {
		pctime = Serial.parseInt();
		if (pctime >= DEFAULT_TIME) {  // check the integer is a valid
					       // time (greater than Jan 1 2013)
			setTime(pctime);       // Sync Arduino clock to the time
					       // received on the serial port
		}
	}
}

time_t requestSync() {
	// Serial.write(TIME_REQUEST);
	return 0;  // the time will be sent later in response to serial mesg
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(9600);

	// Set function to call when time sync requested
	setSyncProvider(requestSync);
}

static unsigned long last_reading = 0;

void printd(int d) {
	if (d < 10) Serial.print('0');
	Serial.print(d);
}

void loop() {
	if (Serial.available()) {
		processSyncMessage();
	}

	if (timeStatus() != timeNotSet) {
		if (millis() - last_reading > 1000) {
			last_reading = millis();

			DHT.read();
			Serial.print(year());
			Serial.print('-');
			printd(month());
			Serial.print('-');
			printd(day());
			Serial.print('T');
			printd(hour());
			Serial.print(':');
			printd(minute());
			Serial.print(':');
			printd(second());
			Serial.print('\t');
			Serial.print(DHT.getTemperature());
			Serial.print('\t');
			Serial.print(DHT.getHumidity());
			Serial.println();
		}
	}

	if (timeStatus() == timeSet) {
		digitalWrite(LED_BUILTIN, HIGH);
	} else {
		digitalWrite(LED_BUILTIN, LOW);
	}
}
