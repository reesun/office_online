<? /* Common functionality */
	function elapsedtime($starttime, $stoptime) {
	        $startsep = strpos($starttime, " ");
       		$startsec = substr($starttime, $startsep);
        	$startmicro = substr($starttime, 0, $startsep - 1);

        	$stopsep = strpos($starttime, " ");
        	$stopsec = substr($stoptime, $stopsep);
        	$stopmicro = substr($stoptime, 0, $stopsep - 1);

        	$elapsed = ($stopsec - $startsec) + ($stopmicro - $startmicro);

        	return $elapsed;
	}
?>