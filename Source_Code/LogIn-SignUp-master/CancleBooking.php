<?php
$conn = $conn = mysqli_connect("localhost","root","");
mysqli_select_db($conn,"car_parking");

$slot1 = true;
$z = 0;
if(!empty($_GET['n1']))
    {
		$slot = $_GET['n1'];
        $z = 1;
        //$val2 = $_POST['sensor2'];
    }
    
    $get = "SELECT * FROM `booking_up` WHERE `name` = '$slot';";
    $resutl = mysqli_query($conn, $get);

    $raw = mysqli_fetch_array($resutl);
    $slot_no = $raw['slot_no'];
    echo $slot_no;

    $udp_up = "UPDATE `booking_up` SET `name` = '', `number_plate` = '', `slot_no` = '' WHERE `booking_up`.`name` = '$slot';";
    $result_4 = mysqli_query($conn,$udp_up);

    if($slot_no == 1){
    $empt = "UPDATE `slot_1` SET `state` = 'empty' WHERE `slot_1`.`no` = 1";
    }
    elseif($slot_no == 2){
        $empt = "UPDATE `slot_2` SET `state` = 'empty' WHERE `slot_2`.`no` = 1";
    }
    else{
        $empt = "SELECT 1";
    }
    $result_1 = mysqli_query($conn,$empt);
    echo "Booking Cancled";
?>