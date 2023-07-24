addEventListener("fetch", async (e) => {
    e.respondWith(Response.redirect("/CyberKit/service-workers/navigation-redirect-main-frame.https.html#redirected"));
});
