#Deadlock in JS world (WebAssembly).

##Motivation:
Deadlock is highly dangerous error, normal sw-developer start to run away when he/she understand that someone wants to assign task to fix it.
On one C++ project we knew that we had evil deadlock in src for a long time. A number of bright minds tried to catch it and we wait for fix it about one year.
I don't joke. One year we faced app freezing (which cause emergency restart) during high performance-load period and we couldn't do nothing. Because of nature of the problem.

##Definition of deadlock
Just to add here definition
According wikipedia https://en.wikipedia.org/wiki/Deadlock
"In concurrent computing, a deadlock is a state in which each member of a group is waiting for another member, including itself".
I.e. part of your application (possible during very important presentation, or under heavy load on Xmas) stop to work, your queues, RAM, buffers are growing up.
Customers start to complain, managers start to argue. But only solution which you have is "restart all and relax".

##Deadlock in JS
You can said: hold on! in JS world we have only one thread, and nothing like mutex!
We, JS developers, can play with cross-locking resources with help of async/await https://blog.mayflower.de/6369-javascript-mutex-synchronizing-async-operations.html
but thread is only one and it is still running! Don't call to police! Nobody dead here.

We also can achieve some bad-locking things with help of webworkers https://news.ycombinator.com/item?id=13786737
Unfortunately with WebWorkers our possibility to make harm very limited...

But true-oldscool deadlock in my browser?! Nonsense!


##WebAssembly
Yes and no.
We can said that WebAssembly is not far future, but our reality https://caniuse.com/#feat=wasm

We know that WebAssembly Threads ready to try in Chrome 70
https://developers.google.com/web/updates/2018/10/wasm-threads

And why, if we hear words "threads and multitasking" we don't see crying developers, which could praying:
Father! Help me to remove "race condition" and "deadlock" from my src!

##C++ Deadlock example
We try to fix that lack.
Here some classical example when main thread creates 2 child-threads T1 and T2. Both try in parallel access to the same resources (resource1, resource2).
At the end T1 waits when T2 release resource2. T2 waits for T1 release of resource1. Something like that:

T2: Job started in resource2..
T1: Job started in resource1..
T2: Trying to get resource1
T1: Trying to get resource2

To see threads and deadlock you have to set "experimental WebAssembly threads" in `chrome://flags`
https://developers.google.com/web/updates/images/2018/10/WasmThreads3.png

compile
`emcc -O2 -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=3 -o test.js test.c`

start test server
`emrun --no_browser --port 8080 test.html`

open browser
localhost:8080