<?php
  /* do we have something to look for? */
  if (!isset($topic) || $topic == "")
  {
    $topic = "WebHome";
  }

  /* body */
  require( "html/" . $topic . ".html" )
?>
