<HTML><HEAD><TITLE>Online viewer</TITLE></HEAD>
<BODY onload="self.location=document.links[0]">
<PRE>
<?php 
   include("rsc.class.inc");
   include("const.inc");
   include("func.inc");
   include("db.inc");
   include("fetch.inc");
   include("render.inc");
 // send headers to tell the browser to close the connection

	if(file_exists("$dir_rsc".$tmpRsc->RscMD5."/tmp1.gif")) {
		echo "\n\n<A HREF=\"psview.php?id=$tmpRsc->RscMD5&page=1\"></A>";

	} elseif(file_exists("$dir_rsc".$tmpRsc->RscMD5."/index.html")) {
		echo "\n\n<a href=\"rsc/$tmpRsc->RscMD5/\"></A>";

	} else {
		echo "\n\n<a href=\"psfail.php\"></A>";
	}


 
// flush all output

?>
</PRE>
</BODY></HTML>

