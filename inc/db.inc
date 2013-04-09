<? /* Set up connection to mysql database. */

	$hst = "172.16.1.101"; // Host
	$usr = "root";          // User
	$pwd = "t2e0s1t0";          // Password
	$db  = "psview";          // Database name

	@mysql_connect($hst,$usr,$pwd)
		or exit("Cannot connect to database");
	mysql_select_db($db)
		or exit("Cannot open database!");
?>
