/*

  website/readme.txt
  Installation : JOS Project @ sourceforge.net
  Date Written : GCHII 19 October 2005
  Date Revised : GCHII 28 January 2006

*/

This module is a reflection of our production website at
http://jos.sourceforge.net/

1. PRODUCTION WEBSITE
---------------------

1.1 Home
/home/groups/j/j/jos

1.2 Tree
backup/ - Backup directory is excluded from CVS.
cgi-bin/ - Empty directory is excluded from CVS.
exclude - A list of article names.
htdocs/ - Contains our homepage and htaccess file. This is
no longer a symbolic link to ./sfWiki/htdocs/.
htdocs/html/ - Contains rendered JOS Wiki pages.
josdata.tar.gz
scripts/
sfWiki/ - Directory contains production wiki.
sfWiki.1/ - Directory contains test wiki.
sfWiki.2005-10-6tar.gz - archive is excluded from CVS.
sfWiki.Apr20.tar.gz - archive is excluded from CVS.
sfWiki.Apr25.tar.gz - archive is excluded from CVS.
sfWiki.May12.tar.gz - archive is excluded from CVS.
www/ -> htdocs/ (symbolic link)

1.3 Building
build.xml - Ant script builds a snapshot to install in production.

 $ ant
 $ scp jos-20051019.tgz jos.sourceforge.net:/home/groups/j/jo/jos/mirror
 $ ssh jos.sourceforge.net
 $ cd /home/groups/j/jo/jos
 $ tar xzf mirror/jos-20051019.tgz
 $ mv joswebsite/html/ sfWiki/htdocs/

1.4 Homepage
index.html - ?

1.5 htaccess
.htaccess - ?

