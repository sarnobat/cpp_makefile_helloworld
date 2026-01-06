<?php

$loaded = extension_loaded('helloworld');
$value = $loaded ? hello_world() : 'Extension not loaded';

?>
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>PHP C++ Extension Demo</title>
    <style>
      body {
        font-family: Georgia, "Times New Roman", serif;
        margin: 2rem;
      }
      code {
        background: #f2f2f2;
        padding: 0.2rem 0.4rem;
        border-radius: 4px;
      }
    </style>
  </head>
  <body>
    <h1>PHP + C++ Extension</h1>
    <p>Extension loaded: <strong><?= $loaded ? 'yes' : 'no' ?></strong></p>
    <p>hello_world(): <code><?= htmlspecialchars($value, ENT_QUOTES) ?></code></p>
  </body>
</html>
