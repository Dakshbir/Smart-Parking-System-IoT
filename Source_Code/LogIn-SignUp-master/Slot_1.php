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

$qry = "SELECT * FROM `slot_1` WHERE `no` = 1";
$qry_1 = "SELECT * FROM `data` ORDER BY `data`.`ID` DESC LIMIT 1";
$qry_2 = "SELECT * FROM `users` WHERE `name` LIKE '$slot'";
 
$result = mysqli_query($conn,$qry);
$result_1 = mysqli_query($conn,$qry_1);
$result_3 = mysqli_query($conn,$qry_2);
$result_4 = mysqli_query($conn,$udp_up);

$data = mysqli_fetch_array($result);
$data_1 = mysqli_fetch_array($result_1);
$data_3 = mysqli_fetch_array($result_3);

$id = $data_1['Sensor1'];
$id_2 = $data_1['Sensor2'];
$id_3 = $data_1['Sensor3'];
//echo $id."<br>";

$no = $data['no'];
$state = $data['state'];

$u_name = $data_3['name'];
$n_plate = $data_3['number_plate'];

$state_a;

$no = $data['no'];
$state = $data['state'];
$a = "empty";
//echo $state."<br>";

//INSERT INTO `a_test` (`SR_NO`, `Name`) VALUES ('1', 'book')

if($slot1 == true){
if($id_3 == 'p1s1off'){
    if($state == 'empty'){
        $udp_up = "UPDATE `booking_up` SET `name` = '$u_name', `number_plate` = '$n_plate', `slot_no` = '1' WHERE `booking_up`.`SR_NO` = 1;";
        $result_4 = mysqli_query($conn,$udp_up);
        $udp = "UPDATE `slot_1` SET `state` = 'booked' WHERE `slot_1`.`no` = 1";
        $result_1 = mysqli_query($conn,$udp);
        $book = "INSERT INTO `booking` (`name`, `number_plate`, `slot_no`) VALUES ('$u_name', '$n_plate', '1');";
        $result_2 = mysqli_query($conn,$book);
        echo "Data entered<br>";
        
}
else if($state == 'booked')
{
    $state_a = "Cannot enter data";
    echo $state_a."<br>";
}
else{
    $udp = "UPDATE `slot_1` SET `state` = 'empty' WHERE `slot_1`.`no` = 1";
    $result_1 = mysqli_query($conn,$udp);
    echo "Updated<br>";
}
}
else if($id_3 == 'p1s1on'){
    if($state == 'empty'){
        $udp = "UPDATE `slot_1` SET `state` = 'full' WHERE `slot_1`.`no` = 1";
        $result_1 = mysqli_query($conn,$udp);
        $state_a = "updata full";
        echo $state_a."<br>";
    }
    else if($state == 'booked'){
        $udp = "UPDATE `slot_1` SET `state` = 'full' WHERE `slot_1`.`no` = 1";
        $result_1 = mysqli_query($conn,$udp);
        $state_a =  "update full";
        echo $state_a."<br>";
    }
    else{
        $state_a =  "full";
        echo $state_a."<br>";
    }
}
}

/*if($state == 'empty' )
{
    echo "book";
}
else if($state == 'booked'){
    echo "full";
}
else if($id == 'p1s1on' && $state == 'full'){
    echo "full";
}
else{
    echo "empty";
}*/


?>