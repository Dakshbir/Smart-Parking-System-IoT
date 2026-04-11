<?php

$host = "localhost";
$dbname = "car_parking";
$username = "root";
$password = "";

$conn = new mysqli($host, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

if (!empty($_POST['sensor1']) && !empty($_POST['sensor2']) && !empty($_POST['sensor3'])) {
    // sensor1 = gate sensor, sensor2 = slot1 sensor, sensor3 = slot2 sensor
    $gateSensor = $_POST['sensor1'];
    $slot1Sensor = $_POST['sensor2'];
    $slot2Sensor = $_POST['sensor3'];

    // Persist latest hardware snapshot as-is.
    $sql = "INSERT INTO `data` (`Sensor1`, `Sensor2`, `Sensor3`) VALUES ('" . $gateSensor . "','" . $slot1Sensor . "','" . $slot2Sensor . "');";
    mysqli_query($conn, $sql);

    // Keep slot_1 state aligned with sensor2 (slot 1 occupancy).
    $slot1StateResult = mysqli_query($conn, "SELECT `state` FROM `slot_1` WHERE `no` = 1");
    $slot1StateRow = mysqli_fetch_array($slot1StateResult);
    $slot1State = $slot1StateRow ? $slot1StateRow['state'] : 'empty';

    if ($slot1Sensor == 'p1s2on') {
        mysqli_query($conn, "UPDATE `slot_1` SET `state` = 'full' WHERE `slot_1`.`no` = 1");
    } elseif ($slot1Sensor == 'p1s2off' && $slot1State != 'booked') {
        mysqli_query($conn, "UPDATE `slot_1` SET `state` = 'empty' WHERE `slot_1`.`no` = 1");
    }

    // Keep slot_2 state aligned with sensor3 (slot 2 occupancy).
    $slot2StateResult = mysqli_query($conn, "SELECT `state` FROM `slot_2` WHERE `no` = 1");
    $slot2StateRow = mysqli_fetch_array($slot2StateResult);
    $slot2State = $slot2StateRow ? $slot2StateRow['state'] : 'empty';

    if ($slot2Sensor == 'p1s3on') {
        mysqli_query($conn, "UPDATE `slot_2` SET `state` = 'full' WHERE `slot_2`.`no` = 1");
    } elseif ($slot2Sensor == 'p1s3off' && $slot2State != 'booked') {
        mysqli_query($conn, "UPDATE `slot_2` SET `state` = 'empty' WHERE `slot_2`.`no` = 1");
    }

    // Two-slot implementation: ignore any stale slot-3 command.
    $mot = "SELECT `slot` FROM `motor` WHERE `no` = 1";
    $result_2 = mysqli_query($conn, $mot);
    $raw = mysqli_fetch_array($result_2);
    $rot = $raw ? $raw['slot'] : '0';

    if ($rot != '1' && $rot != '2') {
        $rot = '0';
    }

    echo $rot;

    $motr = "UPDATE `motor` SET `slot` = '0' WHERE `motor`.`no` = 1";
    mysqli_query($conn, $motr);
}

?>