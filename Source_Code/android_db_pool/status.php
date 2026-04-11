
<?php
$conn = $conn = mysqli_connect("localhost","root","");
mysqli_select_db($conn,"car_parking");

$cookie_name = "Slot";

if(!empty($_GET['n1']))
    {
		$slot = $_GET['n1'];
        $get = "SELECT * FROM `booking_up` WHERE `name` = '$slot';";
        $resutl = mysqli_query($conn, $get);

        $raw = mysqli_fetch_array($resutl);
        $slot_no = $raw ? $raw['slot_no'] : '';

        if($slot_no == '1' || $slot_no == '2'){
            $mot = "UPDATE `motor` SET `slot` = '$slot_no' WHERE `motor`.`no` = 1;";
            $result_1 = mysqli_query($conn,$mot);
            echo "queued";
        } else {
            echo "no active slot";
        }

    } else {
        echo "missing user";
    }
    
    
    /*if(!isset($_COOKIE[$cookie_name]))
    {  
        $cookie_name = "category";
        setcookie($cookie_name, "Books", time() + 86400, "/"); 
        echo "The cookie is set<br>";
    }
    if (isset($_COOKIE[$cookie_name])) 
    {
        $cat = $_COOKIE[$cookie_name];
        echo $cat;
    }*/  
?>
