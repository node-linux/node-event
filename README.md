# node-event
 Simple Nodejs wrapper around Linux Events. The most low-level API you'll find other than reading the event file. Trust me, it's a step up.

 ```typescript
 const io = event.open('/dev/input/event2');

 while (true)
     parseEvent(io.next());
 ```
