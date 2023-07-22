<?php
  if (!isset($_SERVER['PHP_AUTH_USER'])) {
   header('WWW-Authenticate: Basic realm="CyberKit AppCache Test Realm"');
   header('HTTP/1.0 401 Unauthorized');
   echo 'Authentication canceled';
   exit;
  } else {
   echo "OK\n";
  }
?>
