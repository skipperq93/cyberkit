#!/usr/bin/env python3

import os
import sys

user_agent = os.environ.get('HTTP_USER_AGENT', '')

sys.stdout.write(
    'Content-Type: text/html\r\n\r\n'
    '<html>\n'
    '<body>\n'
    '<script>\n'
    'if (window.testRunner)\n'
    '    testRunner.dumpAsText();\n'
    '\n'
    'var userAgent = "{}";\n'
    'if (userAgent.match(/CyberKit/)) {{\n'
    '    document.write("PASS: User-Agent header contains the string \'CyberKit\'.");\n'
    '}} else {{\n'
    '    document.write("FAIL: User-Agent header does not contain the string \'CyberKit\', value is \'" + userAgent + "\'");\n'
    '}}\n'
    '</script>\n'
    '\n'
    '</body>\n'
    '</html>\n'.format(user_agent)
)