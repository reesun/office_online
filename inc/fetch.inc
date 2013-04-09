<?

	function new_id() {
		// TBD: While not taken, get md5 id
		return substr(md5(uniqid("dff")), 0, 8); /* Unique enough */
	}

	function fetch_wget($fname, $url) {
		global $cmd_wget;
		$cmd = "/bin/bash -c \"$cmd_wget -O $fname ".escapeshellcmd($url)." 2>&1\" >> $(date +%Y%m%d)_log";
//		$cmd = "/usr/local/bin/wget -O $fname ".escapeshellcmd($url);
#DEB		echo $cmd."<br>";
		system($cmd); // Can decide from wget if dl succeeded...
		return true;
	}

	function fetch_try_unzip($fname) {
		global $dir_bin;
 		$cmd = "$dir_bin/try_unzip.sh $fname";
#		echo $cmd."<br>";
                system($cmd);
		return true;
	}

	function rsc_type($fname) {
		global $cmd_file;
		$cmd = "$cmd_file -b $fname";
		exec($cmd, $output);

		if(count($output) == 1) { # One line of output
			if(ereg("Microsoft Office", $output[0])) {
				$tmpret = 4;
			} elseif(ereg("^PDF", $output[0])) {
				$tmpret = 3;
			} elseif(ereg("^PostScript", $output[0])) {
				$tmpret = 2;
			} else {
				$tmpret = 1; # Unknown
			}
		} else {
			$tmpret = 0; # No output
		}
		return $tmpret;
	}

	function endsWith($haystack, $needle){
    		$length = strlen($needle);
    		if ($length == 0) 
        		return true;
	    	return (substr($haystack, -$length) === $needle);
	}

	$starttime = microtime();

	/* Url or File? */
	 if(is_array($HTTP_POST_FILES) && $HTTP_POST_FILES["file"]["name"] != "") {
		/* File upload */
		$file = fopen("test.txt","a");
		fwrite($file, $HTTP_POST_FILES["file"]["name"]);
		fclose($file);
		/* Always new rsc */
		$tmpRsc = new Rsc;
		$tmpRsc->RscMD5 = new_id();
		$tmpRsc->RscName = $HTTP_POST_FILES["file"]["name"];
		$tmpRsc->IsURL = 0;

		$tmpFname = $dir_rsc."/".$tmpRsc->RscMD5.".infile";
		if (copy($HTTP_POST_FILES["file"]["tmp_name"], $tmpFname) ) {
			#echo "<!-- ok -->";
			/* Perform file voodoo (possible unzip and file type determination) */
			fetch_try_unzip($tmpFname);

			$tmpRsc->RscTypeID = rsc_type($tmpFname);
			#$tmpRsc->RscTypeID = 4;
			$tmpRsc->InCache = -1; // If not failed ...
			$tmpRsc->Size = filesize($tmpFname);
			$add_rsc = true;
		} else {
			$tmpRsc->InCache = 0;
			$add_rsc = true;
		}
	}
	$url = $_GET['url'];
	if(isset($url)) {
		/* Fetch url */
		if($url == "http://") {
			/* Press back, and enter a real url, or view one of the examples */
			#echo "Press back, and enter a real url, or view one of the examples.<br>";
			$got_file = 0;
		} else {
			
			// 现在数据库中查找url对应的文档id
			$sql = "SELECT * FROM Rsc WHERE ";
			$sql .= "IsURL = -1 AND RscName ='".mysql_escape_string($url)."'";
#DEB			echo $sql;
			$qRsc = mysql_query($sql);
			if(!$tmpRsc = mysql_fetch_object($qRsc)) {
				/* Not in cache - add rsc and fetch */
				$tmpRsc = new Rsc;
				$tmpRsc->RscMD5 = new_id();
				$tmpRsc->RscName = $url;
				$tmpRsc->IsURL = -1;
				$tmpRsc->InCache = 0;

				$download = true;
				$add_rsc = true;
			} else { // Got rsc
#DEB				echo "Got object<br>";
				if($tmpRsc->InCache) {
#DEB					echo "In cache. No fetch<br>";
				} else {					
#DEB					echo "Fetch<br>";
					$download = true;
					$upd_rsc = true;
				}
			}

			if($download) {
				/* Go get it */
				$tmpFname = $dir_rsc.$tmpRsc->RscMD5.".infile";

				#echo "Downloading...<BR>";
				fetch_wget($tmpFname, $url);

				/* Perform file voodoo (possible unzip and file type determination) */
				fetch_try_unzip($tmpFname);
				if(endsWith($url, xls) || endsWith($url, xlsx)){
					$tmpRsc->RscTypeID = 8;  // RscTypeID defined in table RscType 
				} else if(endsWith($url, doc) || endsWith($url, docx)){
					$tmpRsc->RscTypeID = 4;
				} else{
					$tmpRsc->RscTypeID = rsc_type($tmpFname);
				}
				$tmpRsc->InCache = -1; // If not failed ...
				$tmpRsc->Size = filesize($tmpFname);
			}
		}
	}

	if($add_rsc) {
		/* Add $tmpRsc to Rsc */
		$sql = "INSERT INTO Rsc";
		$sql .= " (RscMD5, RscTypeID, RscName, IsURL, InCache, Size)";
		$sql .= sprintf(" VALUES ('%s', '%s', '%s', %d, %d, %d)",
						$tmpRsc->RscMD5,
						$tmpRsc->RscTypeID,
						mysql_escape_string($tmpRsc->RscName),
						$tmpRsc->IsURL,
						$tmpRsc->InCache,
						$tmpRsc->Size
						);
		#echo $sql."<BR>";
		$q = mysql_query($sql);
		$tmpRsc->RscID = mysql_insert_id();
	}

	if($upd_rsc) {
		$sql = "UPDATE Rsc SET RscTypeID=".$tmpRsc->RscTypeID;
		$sql .= ", InCache=".$tmpRsc->InCache;
		$sql .= ", Size=".$tmpRsc->Size;
		$sql .= " WHERE RscID=".$tmpRsc->RscID;
#DEB		echo $sql."<BR>";
		$q = mysql_query($sql);
	}

	$stoptime = microtime();

	/* Tell rscfetch about this fetch */
	$host = mysql_escape_string(substr(GetEnv("REMOTE_HOST"), 0, 254));
	$ip = mysql_escape_string(substr(GetEnv("REMOTE_ADDR"), 0, 15));
	$req_status = "200";	
	$req_method = mysql_escape_string(substr(GetEnv("REQUEST_METHOD"), 0, 254));
	$request = mysql_escape_string(substr(GetEnv("QUERY_STRING"), 0, 254));
	$referer = mysql_escape_string(substr(GetEnv("HTTP_REFERER"), 0, 254));
	$user_agent = mysql_escape_string(substr(GetEnv("HTTP_USER_AGENT"), 0, 254));

	$sql .= "'$req_status', '$req_method', '$request', '$referer', '$user_agent', '$host', '$ip')";

	$sql = "INSERT INTO RscFetch";
	$sql .= " (RscID, FetchDate, FromCache, FetchSeconds, Size, req_status, req_method, request, referer, user_agent, host, ip) ";
	$sql .= sprintf("VALUES ('%s', NOW(), %d, '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
						$tmpRsc->RscID,
						($download || isset($file)) ? 0 : -1,
						elapsedtime($starttime, $stoptime),
						$tmpRsc->Size,
						$req_status,
						$req_method,
						$request,
						$referer,
						$user_agent,
						$host,
						$ip
					);

#	echo $sql."<BR>";
	$q = mysql_query($sql);

?>
