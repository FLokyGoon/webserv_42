#!/usr/bin/php-cgi
<?php

ob_start();
header("Content-Type: text/plain");

parse_str(getenv('QUERY_STRING'), $_GET);

if (isset($_GET['name'])) 
{
    $name = $_GET['name'];
    echo "Hello, " . htmlspecialchars($name) . "!";
} 
else {
    echo "Hello, stranger!";
}

$content = ob_get_contents();
$contentLength = strlen($content);

ob_end_clean();

echo $content;

?>
