from ..webdriver_server import CyberKitDriverServer
from .base import WdspecExecutor, WebDriverProtocol


class CyberKitDriverProtocol(WebDriverProtocol):
    server_cls = CyberKitDriverServer


class CyberKitDriverWdspecExecutor(WdspecExecutor):
    protocol_cls = CyberKitDriverProtocol
