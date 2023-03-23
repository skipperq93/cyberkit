function fetchTest(event)
{
    event.respondWith(fetch("/CyberKit/service-workers/resources/lengthy-pass.py?delay=0.5"));
}

self.addEventListener("fetch", fetchTest);
