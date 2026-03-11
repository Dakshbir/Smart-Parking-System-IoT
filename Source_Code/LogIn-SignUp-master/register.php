<?php
// User Registration API for Smart Parking System
// This endpoint receives registration data from Android app and creates new user account

$conn = mysqli_connect("localhost","root","");
mysqli_select_db($conn,"car_parking");

// Get registration data from GET parameters sent by Android app
$name = $_GET['R_name'];
$phone_no = $_GET['R_Phone_no'];
$email_id = $_GET['R_Email_id'];
$number_plate = $_GET['R_Number_plate'];
$password = $_GET['Pass'];

// Validate that all required fields are provided
if (!empty($name) && !empty($phone_no) && !empty($email_id) && !empty($number_plate) && !empty($password))
{
    // Check if user already exists
    $check_qry = "SELECT * FROM `users` WHERE `name` = '$name' OR `email_id` = '$email_id' OR `number_plate` = '$number_plate'";
    $check_result = mysqli_query($conn, $check_qry);
    
    if(mysqli_num_rows($check_result) > 0) {
        // User already exists
        echo "User already exists with this name, email, or number plate";
    } else {
        // Insert new user into database
        $qry = "INSERT INTO `users` (`name`, `phone_no`, `email_id`, `number_plate`, `password`) VALUES ('$name', '$phone_no', '$email_id', '$number_plate', '$password');";
        
        if(mysqli_query($conn, $qry)) {
            echo "Registration Successful";
        } else {
            echo "Registration Failed: " . mysqli_error($conn);
        }
    }
} else {
    echo "Please fill all required fields";
}

mysqli_close($conn);
?>
