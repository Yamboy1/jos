<?php

/* filesystem characteristics */
$_SITE["portaldir"] = "/home/<g>/<gr>/<groupname>/sfWiki/portal/";
$_SITE["mirrordir"] = $_SITE["portaldir"] . "htdocs/wiki/";
$_SITE["backupdir"] = $_SITE["portaldir"] . "BACKUP/";
$_SITE["phpcgi"] = "/usr/bin/php";

/* website characteristics */
$_SITE["docrootdir"] = "/sfWiki/";
$_SITE["darkbgcolor"] = "#737B9C";
$_SITE["lightbgcolor"] = "#EEEEF8";
$_SITE["textcolor"] = "#FFFFFF";

$_SITE["linkcolor"] = "#0000FF";

/* Who do I mail with problems, if anyone? */
$_SITE["mailto"] = "myusername@users.sourceforge.net";

/* database characteristics */
$_SITE["dbhost"] = "database host";
$_SITE["dbuser"] = "database user name";
$_SITE["dbpass"] = "password for that user";
$_SITE["database"] = "database name";
$_SITE["dbtable"] = "table to use";
$_SITE["dbsessiontable"] = "second table to use";
$_SITE["dbusertable"] = "third table to use";
$_SITE["dbstattable"] = "fourth table to use";

/* CVS characteristics */
$_SITE["cvs_link"] = "cvs.sourceforge.net/cgi-bin/viewcvs.cgi/~checkout~/";
$_SITE["cvs_ctype_default"] = "text/plain";                       
$_SITE["cvs_ctype_html"] = "text/html";

/* Offsite backup characteristics. */
$_SITE["offsite_dir"] = "directory on remote machine";
$_SITE["offsite_user"] = "username on remote machine";
$_SITE["offsite_address"] = "address of remote machine";
$_SITE["offsite_port"] = "port to use to connect to remote machine; leave blank ( = ""; )for default port";


?>
