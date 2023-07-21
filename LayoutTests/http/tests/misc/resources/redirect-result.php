<html>
<body>
<script>
if (window.testRunner)
    testRunner.dumpAsText();

var userAgent = "<?php echo $_SERVER['HTTP_USER_AGENT']; ?>";
if (userAgent.match(/CyberKit/)) {
    document.write("PASS: User-Agent header contains the string 'CyberKit'.");
} else {
    document.write("FAIL: User-Agent header does not contain the string 'CyberKit', value is '" + userAgent + "'");
}
</script>

</body>
</html>
