<?php
$conn = mysqli_connect("localhost", "root", "");
mysqli_select_db($conn, "car_parking");

if (empty($_GET['n1'])) {
    echo "invalid user";
    exit();
}

$slot = $_GET['n1'];

$userQry = "SELECT * FROM `users` WHERE `name` LIKE '$slot'";
$userResult = mysqli_query($conn, $userQry);
$user = mysqli_fetch_array($userResult);
if (!$user) {
    echo "invalid user";
    exit();
}

$slotStateQry = "SELECT `state` FROM `slot_2` WHERE `no` = 1";
$slotStateResult = mysqli_query($conn, $slotStateQry);
$slotStateRow = mysqli_fetch_array($slotStateResult);
$slotState = $slotStateRow ? $slotStateRow['state'] : 'full';

if ($slotState != 'empty') {
    echo "full";
    exit();
}

// Prevent duplicate active booking for the same user.
$existingBookingQry = "SELECT * FROM `booking_up` WHERE `name` = '$slot' AND `slot_no` != ''";
$existingBookingResult = mysqli_query($conn, $existingBookingQry);
if (mysqli_num_rows($existingBookingResult) > 0) {
    echo "already booked";
    exit();
}

$u_name = $user['name'];
$n_plate = $user['number_plate'];

$bookingUp = "UPDATE `booking_up` SET `name` = '$u_name', `number_plate` = '$n_plate', `slot_no` = '2' WHERE `booking_up`.`SR_NO` = 2";
mysqli_query($conn, $bookingUp);

$slotUpdate = "UPDATE `slot_2` SET `state` = 'booked' WHERE `slot_2`.`no` = 1";
mysqli_query($conn, $slotUpdate);

$bookLog = "INSERT INTO `booking` (`name`, `number_plate`, `slot_no`) VALUES ('$u_name', '$n_plate', '2')";
mysqli_query($conn, $bookLog);

echo "booked";
?>