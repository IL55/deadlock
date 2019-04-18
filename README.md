# Deadlock in JS world (WebAssembly).

## Motivation:
Deadlock is highly dangerous error, normal sw-developer start to run away when he/she understand that someone wants to assign task to fix it.
On one C++ project we knew that we had evil deadlock in src for a long time. A number of bright minds tried to catch it and we wait for fix it about one year.
I don't joke. One year we faced app freezing (which cause emergency restart) during high performance-load period and we couldn't do nothing. Because of nature of the problem.

## Definition of deadlock
Just to add here definition
According wikipedia https://en.wikipedia.org/wiki/Deadlock
"In concurrent computing, a deadlock is a state in which each member of a group is waiting for another member, including itself".
I.e. part of your application (possible during very important presentation, or under heavy load on Xmas) stop to work, your queues, RAM, buffers are growing up.
Customers start to complain, managers start to argue. But only solution which you have is "restart all and relax".

## Deadlock in JS
You can said: hold on! in JS world we have only one thread, and nothing like mutex!
We, JS developers, can play with cross-locking resources with help of async/await https://blog.mayflower.de/6369-javascript-mutex-synchronizing-async-operations.html
but thread is only one and it is still running! Don't call to police! Nobody dead here.

We also can achieve some bad-locking things with help of webworkers https://news.ycombinator.com/item?id=13786737
Unfortunately with WebWorkers our possibility to make harm very limited...

But true-oldscool deadlock in my browser?! Nonsense!


## WebAssembly
Yes and no.
We can said that WebAssembly is not far future, but our reality https://caniuse.com/#feat=wasm

We know that WebAssembly Threads ready to try in Chrome 70
https://developers.google.com/web/updates/2018/10/wasm-threads

And why, if we hear words "threads and multitasking" we don't see crying developers, which could praying:
Father! Help me to remove "race condition" and "deadlock" from my src!

## C++ Deadlock example
We try to fix that lack.
Here some classical example when main thread creates 2 child-threads T1 and T2. Both try in parallel access to the same resources (resource1, resource2).
At the end T1 waits when T2 release resource2. T2 waits for T1 release of resource1. Something like that:

T2: Job started in resource2..
T1: Job started in resource1..
T2: Trying to get resource1
T1: Trying to get resource2

## Try it in your Chrome

To see threads and deadlock you have to set "experimental WebAssembly threads" in `chrome://flags`
https://developers.google.com/web/updates/images/2018/10/WasmThreads3.png
Chrome restart is required.

Open Chrome Dev-tools1 and select Console to see debug output
Open test page here:
https://il55.github.io/deadlock/test.html

## I don't want to see that evil deadlock in my browser
Sorry. Tab is locked. There is no unlock button:-)
But you can close tab.

## Run normal version without lock
try to see it without lock, open Chrome console and see results
https://il55.github.io/deadlock/withoutlock/test.html

## But does it real deadlock
Where threads are locked? Maybe we observe just old good webworkers and nothing really locked here?
Yes and no. As described here:
https://developers.google.com/web/updates/2018/10/wasm-threads#threads_what_about_workers
"Web Workers do not share mutable data between them"
But "WebAssembly threads, on the other hand, are threads that can share the same Wasm memory."
...
"This means that the applications that use Wasm threads are responsible for managing access to the shared memory as in any traditional threaded application."

So if we responsible for that, let's deadlock it, as normal app do. https://allstarnix.blogspot.com/2012/07/real-life-deadlock.html
:-)


## Build from source and try locally

compile
`emcc -O2 -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=3 -o test.js test.c`

start test server
`emrun --no_browser --port 8080 test.html`

open browser
localhost:8080