navigator.serviceWorker.register("/CyberKit/service-workers/resources/lengthy-pass.py?delay=0.5", { scope: "/CyberKit/service-workers/resources/" });
setTimeout(() => self.postMessage("ok"), 100);
