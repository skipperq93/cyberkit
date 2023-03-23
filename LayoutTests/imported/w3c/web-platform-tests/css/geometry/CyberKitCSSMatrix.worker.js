// https://drafts.fxtf.org/geometry/#DOMMatrix

importScripts('/resources/testharness.js');

test(() => {
  assert_false('CyberKitCSSMatrix' in self);
}, 'CyberKitCSSMatrix in worker');

done();
