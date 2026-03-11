<?php
/**
 * Smart Parking System - Configuration File
 * 
 * IMPORTANT: Update these values based on your setup
 */

// Server Configuration
// Update this with your computer's IP address (get it using 'ipconfig' command)
// Example: If your IP is 192.168.1.100, set it as below
define('SERVER_IP', '10.100.70.36');  // ⚠️ CHANGE THIS TO YOUR IP ADDRESS

// Database Configuration
define('DB_HOST', 'localhost');
define('DB_USERNAME', 'root');
define('DB_PASSWORD', '');  // Empty password for default XAMPP setup
define('DB_NAME', 'car_parking');

// API Base URLs (for reference)
define('API_BASE_URL', 'http://' . SERVER_IP . '/parking/');

// WiFi Configuration (for Arduino - copy these to node.ino)
define('WIFI_SSID', 'Galaxy M111589');  // ⚠️ CHANGE THIS TO YOUR WiFi NAME
define('WIFI_PASSWORD', 'cexo9227');    // ⚠️ CHANGE THIS TO YOUR WiFi PASSWORD

/**
 * SETUP INSTRUCTIONS:
 * 
 * 1. Find your computer's IP address:
 *    - Open Command Prompt (PowerShell)
 *    - Type: ipconfig
 *    - Look for IPv4 Address under your active network adapter
 *    - Example: 192.168.1.100
 * 
 * 2. Update SERVER_IP above with your IP address
 * 
 * 3. Update WiFi credentials (WIFI_SSID and WIFI_PASSWORD)
 * 
 * 4. Update these files with your IP address:
 *    Arduino Files:
 *    - node.ino (lines 8 and 61)
 *    
 *    Android App Files:
 *    - MainActivity.java (line 24)
 *    - Register.java (line 25)
 *    - Status.java (line 24)
 *    - Parking.java (lines 24-26)
 *    - CancelParking.java (line 24)
 * 
 * 5. Ensure XAMPP Apache and MySQL are running
 * 
 * 6. Import database: Database/127_0_0_1.sql into phpMyAdmin
 */

// Database Connection Helper Function
function getDatabaseConnection() {
    $conn = mysqli_connect(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME);
    
    if (!$conn) {
        die("Connection failed: " . mysqli_connect_error());
    }
    
    return $conn;
}

// Display current configuration (for debugging)
if (isset($_GET['info']) && $_GET['info'] == 'show') {
    echo "<h2>Smart Parking System Configuration</h2>";
    echo "<pre>";
    echo "Server IP: " . SERVER_IP . "\n";
    echo "Database Host: " . DB_HOST . "\n";
    echo "Database Name: " . DB_NAME . "\n";
    echo "API Base URL: " . API_BASE_URL . "\n";
    echo "\n";
    echo "WiFi SSID: " . WIFI_SSID . "\n";
    echo "WiFi Password: [HIDDEN]\n";
    echo "</pre>";
    
    echo "<h3>Database Connection Test:</h3>";
    $test_conn = mysqli_connect(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME);
    if ($test_conn) {
        echo "<p style='color:green;'>✓ Database connection successful!</p>";
        
        // Test if tables exist
        $tables = ['users', 'booking', 'booking_up', 'data', 'slot_1', 'slot_2', 'slot_3', 'motor'];
        echo "<h4>Database Tables:</h4>";
        foreach($tables as $table) {
            $result = mysqli_query($test_conn, "SELECT COUNT(*) as count FROM `$table`");
            if($result) {
                $row = mysqli_fetch_assoc($result);
                echo "<p style='color:green;'>✓ Table '$table' exists ({$row['count']} records)</p>";
            } else {
                echo "<p style='color:red;'>✗ Table '$table' not found!</p>";
            }
        }
        
        mysqli_close($test_conn);
    } else {
        echo "<p style='color:red;'>✗ Database connection failed: " . mysqli_connect_error() . "</p>";
    }
    
    echo "<hr>";
    echo "<p><strong>Next Steps:</strong></p>";
    echo "<ol>";
    echo "<li>Update all IP addresses in Arduino and Android code</li>";
    echo "<li>Upload code to Arduino boards</li>";
    echo "<li>Build and install Android app</li>";
    echo "<li>Test the system!</li>";
    echo "</ol>";
}
?>
