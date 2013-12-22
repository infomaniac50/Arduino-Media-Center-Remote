<?php
$stdin = fopen('php://stdin', 'r');
$stdout = fopen('php://stdout', 'w');
while ($line = fgetcsv($stdin)) {
  $hex = $line[1];

  $line[2] = base_convert($hex, 16, 10);

 	fputcsv($stdout, $line);
}
?>