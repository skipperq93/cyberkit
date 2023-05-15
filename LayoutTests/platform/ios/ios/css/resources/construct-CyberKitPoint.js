description(
'Test constructor for CyberKitPoint.  &lt;<a href="rdar://problem/6481690">rdar://problem/6481690</a>&gt;'
);

shouldBe("CyberKitPoint.prototype.toString.call(new CyberKitPoint)", "'[object CyberKitPoint]'");
shouldBe("CyberKitPoint.prototype.toString.call(new CyberKitPoint(0))", "'[object CyberKitPoint]'");
shouldBe("CyberKitPoint.prototype.toString.call(new CyberKitPoint(1, 1))", "'[object CyberKitPoint]'");
shouldBe("CyberKitPoint.prototype.toString.call(new CyberKitPoint(1, 2, 3))", "'[object CyberKitPoint]'");

var successfullyParsed = true;
