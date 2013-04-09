Office-online
=============

Converts Word¡¢Excel and PDF to structured HTML.


Prerequisite software:
-----------

    python ./docvert-web.py [-p PORT] [-H host]

Command Line
------------

    - Apache/PHP
    - MySQL
    - ImageMagick (image conversion)
    - Ghostscript (for postscript and pdf)
    - Wget
    - Python2.6/2.7 
    - sudo apt-get install libreoffice python-uno python-lxml python-imaging pdf2svg librsvg2-2

To install the viewer software, perform the following steps:
---------

0: Install prerequisite software.

1: start soffice as deamon
/usr/bin/soffice -headless -norestore -nologo -norestore -nofirststartwizard -accept="socket,port=2002;urp;" &

2: Point apache at the html directory and allow php to include files from the inc directory. An example virtual host configuration can be found in the example.httpd.conf file included in this package. Restart apache.

3: Create a mysql database for the viewer to use, and feed it the file
   - db/psview.sql

4: Edit database and path information in the files
   - inc/db.inc (database connection info)
   - inc/const.inc (path to directories and binaries)
   - bin/render.sh (path to ghostscript and convert)

5: Run the viewer (browse index.html) and fix any problems that crop up, such as missing or wrong path's and url's :-)



LICENCE
-------
Released under the GPL3 see LICENCE


