#include <QueueDuino.h>

// Instantiate the queue
QueueDuino queue;

void setup() {
  Serial.begin(9600);

  // Create the action
  queue.setAction([] (String item) -> bool {
    // Log
    Serial.print(item); Serial.println(" was done");

    return true;
  });

  // Set a event when work was done
  queue.onSuccess([] () -> void {
    Serial.println("Queue finished successful");
  });

  // Add items to the queue
  queue.push("Item 1");
  queue.push("Item 2");

  // Start the queue
  queue.run();
}

void loop() {
  queue.loop();
}
