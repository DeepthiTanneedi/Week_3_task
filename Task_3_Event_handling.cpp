/*Task 3: Event Handling System for Touchscreen Input
Objective: Simulate an event-driven system for processing touchscreen inputs on an HMI.
Requirements:
Create an Event class to represent user interactions:
Attributes: eventType (e.g., Tap, Swipe), x and y coordinates, and timestamp.
Implement an event queue using std::queue:
Store multiple events.
Process events one by one.
Handle specific events:
Tap: Display a message showing the tapped position.
Swipe: Calculate the swipe direction (up/down/left/right) and display it.
Simulate event generation:
Populate the queue with random events.
Deliverables:
A program simulating event processing.
Example output demonstrating event handling.*/
#include <iostream>
#include <queue>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <iomanip>

using namespace std;

class Event {
public:
    enum EventType { Tap, Swipe };
    EventType eventType;
    int x, y;
    int x2, y2;
    long long timestamp;

    Event(EventType type, int x, int y, long long time)
        : eventType(type), x(x), y(y), x2(-1), y2(-1), timestamp(time) {}

    Event(EventType type, int x1, int y1, int x2, int y2, long long time)
        : eventType(type), x(x1), y(y1), x2(x2), y2(y2), timestamp(time) {}
};

class EventQueue {
private:
    queue<Event> eventQueue;
    mutex queueMutex;

public:
    void addEvent(const Event& event) {
        lock_guard<mutex> lock(queueMutex);
        eventQueue.push(event);
    }

    void processEvents() {
        while (true) {
            Event event = getNextEvent();
            if (event.eventType == Event::Tap) {
                cout << "[Event: Tap] Position: (" << event.x << ", " << event.y
                     << "), Timestamp: " << event.timestamp << endl;
            } else if (event.eventType == Event::Swipe) {
                string direction = getSwipeDirection(event.x, event.y, event.x2, event.y2);
                cout << "[Event: Swipe] From: (" << event.x << ", " << event.y
                     << ") To: (" << event.x2 << ", " << event.y2
                     << "), Direction: " << direction
                     << ", Timestamp: " << event.timestamp << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }

private:
    Event getNextEvent() {
        unique_lock<mutex> lock(queueMutex);
        while (eventQueue.empty()) {
            lock.unlock();
            this_thread::sleep_for(chrono::milliseconds(100));
            lock.lock();
        }
        Event event = eventQueue.front();
        eventQueue.pop();
        return event;
    }

    string getSwipeDirection(int x1, int y1, int x2, int y2) {
        int dx = x2 - x1;
        int dy = y2 - y1;

        if (abs(dx) > abs(dy)) {
            return dx > 0 ? "Right" : "Left";
        } else {
            return dy > 0 ? "Down" : "Up";
        }
    }
};

void generateEvents(EventQueue& eventQueue) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> coordDist(0, 100);
    uniform_int_distribution<> eventDist(0, 1);

    while (true) {
        int eventType = eventDist(gen);
        int x = coordDist(gen);
        int y = coordDist(gen);
        long long timestamp = chrono::duration_cast<chrono::milliseconds>(
                                  chrono::system_clock::now().time_since_epoch())
                                  .count();

        if (eventType == 0) {
            eventQueue.addEvent(Event(Event::Tap, x, y, timestamp));
        } else {
            int x2 = coordDist(gen);
            int y2 = coordDist(gen);
            eventQueue.addEvent(Event(Event::Swipe, x, y, x2, y2, timestamp));
        }

        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    EventQueue eventQueue;

    thread eventGenerator(generateEvents, ref(eventQueue));
    thread eventProcessor(&EventQueue::processEvents, &eventQueue);

    eventGenerator.join();
    eventProcessor.join();

    return 0;
}