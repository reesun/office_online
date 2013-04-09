<? 
	include("db.inc");
?>
<HTML><HEAD><TITLE>Online viewer</TITLE></HEAD>
<BODY>
<PRE><?
	if(isset($id) && ereg("^[0-9a-f]{8}$", $id)) { # Validate $id
		$view=1;
	} else {
		$view=0;
	}

	if(!($view==1 && isset($page) && ereg("^[0-9]+$", $page))) { # Validate $page
		$page=1;
	}

	if($view==1 && isset($email) && $email<>"email here" ) {
		/* OK. Send mail */
		$Subject = "A document from the online viewer";
		$Mottagare = $email;
		$From = "From: view@samurajdata.se";
		$Return_Path = "Reply-To: view@samurajdata.se";
		$Headers = $From."\n".$Return_Path;
	
		$tidPunkt = Date("Y-m-d H:i:s");
		$Message = "This is a message from the online ps/pdf/word-viewer\n\n";
		$Message .= "Someone wishes that you view the following document:\n";
		$Message .= "http://view.samurajdata.se/psview.php?id=$id&page=$page\n\n";

		$Message .= "The viewer can be found at:\n";
		$Message .= "http://view.samurajdata.se/\n";

		mail( $Mottagare, $Subject, $Message, $Headers );
		echo "Email has been sent to: $email\n\n";

		/* Tell RscMail */
		$host = mysql_escape_string(substr(GetEnv("REMOTE_HOST"), 0, 254));
		$ip = mysql_escape_string(substr(GetEnv("REMOTE_ADDR"), 0, 15));
		$req_status = "200";	
		$req_method = mysql_escape_string(substr(GetEnv("REQUEST_METHOD"), 0, 254));
		$request = mysql_escape_string(substr(GetEnv("QUERY_STRING"), 0, 254));
		$referer = mysql_escape_string(substr(GetEnv("HTTP_REFERER"), 0, 254));
		$user_agent = mysql_escape_string(substr(GetEnv("HTTP_USER_AGENT"), 0, 254));

		$sql = "INSERT INTO RscMail";
		$sql .= " (RscMD5, Position, Mailto, req_date, req_status, req_method, request, referer, user_agent, host, ip)";
		$sql .= sprintf(" VALUES ('%s', '%d', '%s', NOW(), '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
						$id,
						$page,
						mysql_escape_string($email),
						$req_status,
						$req_method,
						$request,
						$referer,
						$user_agent,
						$host,
						$ip
					);

#DEB		echo $sql."<BR>";
		$qRscMail = mysql_query($sql);

	} else {
		/* Not OK */
		echo "No email has been sent\n\n";
	}
?>	
<A HREF="psview.php?id=<? echo $id."&page=$page"; ?>">Click here to return to the document</A>
</BODY></HTML>


