<?php
  /* do we have something to look for? */
  if (!isset($topic) || $topic == "")
  {
    $topic = "WebHome";
  }

  header( "Location: " . "/html/" . $topic . ".html" );
?>
