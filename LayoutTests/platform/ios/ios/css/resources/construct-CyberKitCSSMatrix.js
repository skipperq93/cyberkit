description(
'Test constructor for CyberKitCSSMatrix.  &lt;<a href="rdar://problem/6481690">rdar://problem/6481690</a>&gt;'
);

shouldBe("CyberKitCSSMatrix.prototype.toString.call(new CyberKitCSSMatrix)",
         "'matrix(1, 0, 0, 1, 0, 0)'");

shouldBe("CyberKitCSSMatrix.prototype.toString.call(new CyberKitCSSMatrix(new CyberKitCSSMatrix))",
         "'matrix(1, 0, 0, 1, 0, 0)'");

shouldBe("CyberKitCSSMatrix.prototype.toString.call(new CyberKitCSSMatrix('matrix3d(1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000)'))",
         "'matrix(1, 0, 0, 1, 0, 0)'");

var successfullyParsed = true;
