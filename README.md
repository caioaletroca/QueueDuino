# QueueDuino

This libraries creates a queue list with Strings data, passed as argument to a user configured action. Recommended when you have to perform a current action later, like request a server. The list is also stored on the flash memory.

# Quick Start

## Installation
Download as zip and use the Arduino Library Manager. QueueDuino requires [ArduinoSTL](https://github.com/mike-matera/ArduinoSTL) to run.

## Usage
Include the library on your sketch

```cpp
#include <QueueDuino.h>
```
Initialize a global instance of the library

```cpp
QueueDuino queue;
```

In your loop function, run the queue loop

```cpp
queue.loop();
```

# Documentation

### Constructors

```cpp
QueueDuino();
```
> Default constructor. Uses a default file name on the flash memory

```cpp
QueueDuino(String filename);
```
> Defines a custom filename to save or load on the flash memory

## Methods

### push
```cpp
void push(String item)
```
> Adds (push) a new string item to the queue

### pop
```cpp
void pop()
```
> Removes (pop) the last item from the queue

### clear
```cpp
void clear()
```
> Clears and wipe out all items

### run
```cpp
void run()
```
> Set the queue to run the next time the loop method will be called

### cancel
```cpp
void cancel()
```
> Cancels the queue execution

### loop
```cpp
void loop()
```
> The queue main method. When active, tries to perform the actions and triggers events about the results. If some of the actions failed, the queue is cancelled

## Events

### onSuccess
```cpp
void onSuccess( [] () -> void {} )
```
> Receives a lambda function as argument. onSuccess event is fired when the entire queue was successful

### onFailed
```cpp
void onFailed( [] () -> void {} )
```
> Receives a lambda function as argument. onFailed event is fired when one item action failed

## Action Methods

### setAction
```cpp
void setAction( [] (const String item) -> bool {} )
```
> Receives a lambda function as argument. Configures the action that will run on every queue item. Must return a boolean if the action was successful or not

### runAction
```cpp
void runAction(String item)
```
> Runs the current configured action if a argument item

## Misc Methods

### save
```cpp
void save()
```
> Saves the current queue on the flash memory. Push, pop and loop automatically saves the queue


### load
```cpp
void load()
```
> Loads the queue saved on the flash memory. The queue is loaded automatically on the constructor

### toSerial
```cpp
void toSerial()
```
> Prints the entire queue, line by line on the default Serial


