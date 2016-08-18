<!doctype html>
<html lang="fr">
<head>
  <meta charset="utf-8">
  <title>Generator of sentences of XXX </title>
  <style>
    body{
      margin: auto;
      position: absolute;
      top: 0; left: 0; bottom: 0; right: 0;
    }
  </style>
</head>
<body>
<p> This is a sentences generated with markov chain with last post of XXX : </p>
<?php
include 'Parsedown.php';
$Parsedown = new Parsedown();
$fichier = file("sentenceswithoutrepetition");
srand((double)microtime()*1000000);
$nb = rand(0,numberofsentences);
echo "<h2>".$Parsedown->text($fichier[$nb])."</h2>";
?>

</body>
</html>
