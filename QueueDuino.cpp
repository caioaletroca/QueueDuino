#include "QueueDuino.h"

/**
 * Default constructor
 */
QueueDuino::QueueDuino() {
	// Mount file system
	SPIFFS.begin();

	this->filename = "queue";

	// Starts the system
	this->setup();
}

/**
 * Constructor with custom file name definition
 */
QueueDuino::QueueDuino(String filename) {
	// Mount file system
	SPIFFS.begin();

	this->filename = filename;

	// Starts the system
	this->setup();
}

/**
 * Adds a new item to the queue and saves to the flash memory
 * @param item The item as a String value
 */
void QueueDuino::push(String item) {
	this->queue.push_back(item);

	// Save changes
	this->save();
}

/**
 * Removes the last inserted item from the queue and saves to the flash memory
 */
void QueueDuino::pop() {
	this->queue.pop_back();

	// Save changes
	this->save();
}

/**
 * Clears the current queue list
 */
void QueueDuino::clear() {
	this->queue.clear();
}

/**
 * Set the queue actions to run
 */
void QueueDuino::run() {
	this->active = true;
}

/**
 * Cancels the current running actions
 */
void QueueDuino::cancel() {
	this->active = false;
}

/**
 * Configures and setups the queue system
 */
void QueueDuino::setup() {
	// Load previous saved data
	this->load();
}

/**
 * The main loop method. Runs the actions when requested
 */
void QueueDuino::loop() {
	// Run only if is active
	if(this->active) {
		while(this->queue.size() > 0) {
			// Skip loop if the execution is cancelled
			if(!this->active)
				return;

			// Run the actions
			if(!this->action(this->queue[0])) {
				
				// Fire Failed event
				this->fireFailed();

				// Cancel run
				this->cancel();
				return;
			}

			// Remove executed action from the queue
			this->queue.erase(this->queue.begin());

			// Save changes
			this->save();
		}

		// Fire Success event
		this->fireSuccess();

		// Disable the flag
		this->cancel();
	}
}

/**
 * Register a event on the Failed event
 * @param action The method to run
 */
void QueueDuino::onFailed(EventMethod action) {
	this->failedEvent.push_back(action);
}

/**
 * Register a event on the Success event
 * @param action The method to run
 */
void QueueDuino::onSuccess(EventMethod action) {
	this->successEvent.push_back(action);
}

/**
 * Set the action to a new value
 * @param action The new action
 */
void QueueDuino::setAction(ActionMethod action) {
	this->action = action;
}

/**
 * Runs the current action with a specific item value
 * @param item The item value
 */
void QueueDuino::runAction(String item) {
	this->action(item);
}

/**
 * Saves the current queue to a file on the flash memory
 */
void QueueDuino::save() {
	// Open the file in write mode
	File file = SPIFFS.open(this->filename, "w");

	// Check if successul
	if (!file) {
		Serial.println("[INFO] File creation failed");
		return;
	}

	// Write all the items
	for(int i = 0; i < this->queue.size(); i++) {
		file.println(this->queue[i]);
	}

	// Close the file
	file.close();
}

/**
 * Loads data from a file from the flash memory to the queue
 */
void QueueDuino::load() {
	// Open the file in read mode
	File file = SPIFFS.open(this->filename, "r");

	// Check if successul
	if (!file) {
		return;
	}

	// Read all the file
	String bufferString;
	while(file.available()) {
		bufferString += (char)file.read();
	}

	// Explode the string line by line and set the queue
    char *dataChar = const_cast<char*>(bufferString.c_str());
	char *buffer = strtok(dataChar, "\n");
	while(buffer != 0)
	{
		// Add to the queue
		this->queue.push_back(buffer);
		buffer = strtok(0, " ");
	}

    // Close the file
    file.close();
}

/**
 * Prints the entire queue on the serial
 */
void QueueDuino::toSerial() {
	for(int i = 0; i < this->queue.size(); i++) {
		Serial.println(this->queue[i]);		
	}
}

/**
 * Fires the failed Event
 */
void QueueDuino::fireFailed() {
	for(int i = 0; i < this->failedEvent.size(); i++) {
		this->failedEvent[i]();
	}
}

/**
 * Fires the success Event
 */
void QueueDuino::fireSuccess() {
	for(int i = 0; i < this->successEvent.size(); i++) {
		this->successEvent[i]();
	}
}
