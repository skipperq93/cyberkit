function isGtk()
{
    return navigator.userAgent.includes("CyberKitTestRunnerGTK");
}

function isEfl()
{
    return navigator.userAgent.includes("CyberKitTestRunnerEFL");
}

function videoCanvasPixelComparisonTolerance()
{
    if (isGtk() || isEfl())
        return 6;

    return 2;
}
