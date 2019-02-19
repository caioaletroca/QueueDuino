#ifndef __QUEUE_DUINO__
#define __QUEUE_DUINO__

#include "Arduino.h"
#include "FS.h"

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	#include "ArduinoSTL.h"
#endif

/**
 * Creates and handles a queue with items as Strings that can be persisted on the flash memory
 * The queue can be triggered to run a user defined action on all the items
 * The first use of this class was to create a way to make stores requests when the server on the connection is offline
 * And then trigger latter the queue to finish the stored requests.
 *
 * This class needs ArduinoSTL to proper work, because of std::vector variables
 */
class QueueDuino
{
	public:
		// Type definition
		typedef bool(*ActionMethod)(const String);
		typedef void(*EventMethod)(void);
		
		// Methods
		QueueDuino();
		QueueDuino(String filename);

		// Queue manipulation
		void push(String item);
		void pop();
		void clear();
		void run();
		void cancel();
		void setup();
		void loop();

		// Event Methods
		void onStart(EventMethod action);
		void onFailed(EventMethod action);
		void onSuccess(EventMethod action);

		// Action methods
		void setAction(ActionMethod action);
		void runAction(String item);

		// File methods
		void save();
		void load();

		// Misc methods
		void toSerial();

	private:
		bool active = false;
		String filename = "queue";
		ActionMethod action;
		std::vector<String> queue;
		std::vector<EventMethod> startEvent;
		std::vector<EventMethod> failedEvent;
		std::vector<EventMethod> successEvent;

		// Private methods
		void fireStart();
		void fireFailed();
		void fireSuccess();
};

#endif
