<?php
	function render($dir_rsc, $rsc_id, $rsc_type) {
		global $dir_bin;
 		$cmd = "$dir_bin/render.sh $dir_rsc $rsc_id $rsc_type";
#DEB		echo str_repeat(" ", 512)."\n";
#DEB		echo $cmd."<br>";
#DEB		flush();
#DEB		sleep(10);

		system($cmd);
		return true;
	}

	function discover_gif_comps($dir_rsc, $rsc_md5, $rsc_id, $rsc_type) {
		/* Fetch directory. See how many files there are */
		$my_dir = array();

		$d = dir($dir_rsc."/".$rsc_md5);
		while($entry=$d->read()) {
			if(ereg(".gif", $entry)) {
				$my_dir[] = (int) substr($entry,3,strlen($entry)-7);
			}
		}
		$d->close();
		sort($my_dir);

		$sql = "INSERT INTO RscComp (RscID, RscCompTypeID, Position, Size, Filename)";
		$sql .= " VALUES ";

		$arr = array();
		foreach($my_dir as $key=>$val) {
			$arr[] = sprintf("('%s', 1, %d, %d, '%s')",
							$rsc_id,
							$val,
							filesize("$dir_rsc/$rsc_md5/tmp$val.gif"),
							"tmp$val.gif"
							);
		}
		$sql .= join(",", $arr);
#DEB		echo $sql."<br>";
		$q = mysql_query($sql);
		return true;
	}

	function discover_html_comps($dir_rsc, $rsc_md5, $rsc_id, $rsc_type) {
		/* Fetch directory. See how many files there are */
		$my_dir = array();

		$d = dir($dir_rsc."/".$rsc_md5);
		while($entry=$d->read()) {
			if($entry == "index.html") {
				$my_dir[] = $entry;
			}
		}
		$d->close();
		sort($my_dir);

		$sql = "INSERT INTO RscComp (RscID, RscCompTypeID, Position, Size, Filename)";
		$sql .= " VALUES ";

		$arr = array();
		foreach($my_dir as $key=>$val) {
			$arr[] = sprintf("('%s', 3, %d, %d, '%s')",
							$rsc_id,
							$val,
							filesize("$dir_rsc/$rsc_md5/index.html"),
							"tmp$val.gif"
							);
		}
		$sql .= join(",", $arr);
#DEB		echo $sql."<br>";
		$q = mysql_query($sql);
		return true;
	}

#DEB	echo $tmpRsc->RscID."<br>";
#DEB	echo $tmpRsc->RscTypeID."<br>";

	/* If first component not in cache... */
	$sql = "SELECT RscCompID FROM RscComp WHERE Position=1 AND RscID=".$tmpRsc->RscID;
	$q = mysql_query($sql);
	if(mysql_num_rows($q) == 0) { // Not in component cache - render rsc */
#DEB		echo "Do render<br>";

		/* Dir for rsc component files */
		mkdir($dir_rsc."/".$tmpRsc->RscMD5, 0700);
	
		$file = fopen("test.txt","a");
		/* Do rendition */
		if($tmpRsc->RscTypeID == 8){
			require_once '../Classes/PHPExcel/IOFactory.php';
			#echo $dir_rsc.$tmpRsc->RscMD5.".infile";
			$objPHPExcel = PHPExcel_IOFactory::load($dir_rsc.$tmpRsc->RscMD5.".infile");
			$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'HTML');
			$objWriter->setSheetIndex(0);
			//$objWriter->setImagesRoot('http://www.example.com');
			$objWriter->save($dir_rsc.$tmpRsc->RscMD5."/index.html");
		} else if ($tmpRsc->RscTypeID == 4){
			if(!file_exists($dir_rsc.$tmpRsc->RscMD5."/index.html")) {
			
				$cmd = 'python docvert/docvert-cli.py -o '.$tmpRsc->RscMD5.' -p "web standards" '. $dir_rsc.$tmpRsc->RscMD5.'.infile';
				$status = 0;
				fwrite($file, $cmd);
				system($cmd, $status);
				fwrite($file, "\n\n".$status);
				fclose($file);
			}
		} else{
			render($dir_rsc, $tmpRsc->RscMD5, $tmpRsc->RscTypeID);
		}

		// Insert rendition in RscComp
		if($tmpRsc->RscTypeID == 4 || $tmpRsc->RscTypeID == 8)
			discover_html_comps($dir_rsc, $tmpRsc->RscMD5, $tmpRsc->RscID, $tmpRsc->RscTypeID);
		else
			discover_gif_comps($dir_rsc, $tmpRsc->RscMD5, $tmpRsc->RscID, $tmpRsc->RscTypeID);
	}
?>

