# Smart Parking System IoT - Complete Setup Guide

## 📋 System Overview

This is a complete IoT-based Smart Parking System with:
- **Hardware**: 2 Arduino boards (Arduino Uno + ESP8266 NodeMCU)
- **Backend**: PHP + MySQL running on XAMPP
- **Mobile App**: Android application
- **Features**: 3 parking slots with real-time monitoring, booking, and automated barrier gate

---

## 🔧 Hardware Components Needed

### Arduino Setup (2-Board System)

#### Board 1: Arduino Uno/Mega (Main Controller)
**Components:**
- 1x Arduino Uno or Arduino Mega
- 3x IR Obstacle Sensors (for parking slot detection)
- 1x Stepper Motor (28BYJ-48 with ULN2003 Driver) (for barrier gate)
- Jumper wires
- Breadboard

**Wiring Connections:**
- IR Sensor 1 → Digital Pin 4 (Slot 1)
- IR Sensor 2 → Digital Pin 5 (Slot 2)
- IR Sensor 3 → Digital Pin 6 (Slot 3)
- Stepper Motor:
  - IN1 → Digital Pin 9
  - IN2 → Digital Pin 10
  - IN3 → Digital Pin 11
  - IN4 → Digital Pin 12
- Serial Communication to NodeMCU:
  - TX (Pin 3) → RX of NodeMCU (D2)
  - RX (Pin 2) → TX of NodeMCU (D3)

#### Board 2: ESP8266 NodeMCU (WiFi Module)
**Components:**
- 1x ESP8266 NodeMCU
- USB cable for power
- Jumper wires

**Wiring Connections:**
- Serial from Arduino Uno:
  - RX (D2/Pin marked D3 in code) → TX of Arduino (Pin 3)
  - TX (D3/Pin marked D2 in code) → RX of Arduino (Pin 2)
- Serial to Arduino (for return data):
  - RX (D7/Pin marked D8 in code) → TX of Arduino (Pin 8)
  - TX (D8/Pin marked D7 in code) → RX of Arduino (Pin 7)
- GND → Common ground with Arduino

**How it Works:**
1. Arduino Uno reads IR sensors detecting car presence
2. Arduino sends sensor data to NodeMCU via serial (JSON format)
3. NodeMCU connects to WiFi and sends data to PHP backend
4. NodeMCU receives booking slot number from backend
5. NodeMCU sends slot number back to Arduino
6. Arduino activates stepper motor to open barrier gate

---

## 💻 Software Requirements

### 1. Arduino IDE
- **Download**: https://www.arduino.cc/en/software
- **Version**: 1.8.19 or higher

### 2. XAMPP
- **Download**: https://www.apachefriends.org/download.html
- **Version**: 8.0 or higher (includes PHP 7.4+ & MySQL)

### 3. Android Studio
- **Download**: https://developer.android.com/studio
- **Version**: 2022.1 or higher

### 4. Arduino Libraries
Install these libraries via Arduino IDE Library Manager:
- **ArduinoJson** (by Benoit Blanchon) - Version 5.x (NOT 6.x)
- **ESP8266WiFi** (comes with ESP8266 board package)
- **ESP8266HTTPClient** (comes with ESP8266 board package)

**To install ESP8266 Board Support:**
1. Open Arduino IDE
2. File → Preferences
3. Add to "Additional Board Manager URLs":
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Tools → Board → Boards Manager → Search "ESP8266" → Install

---

## 📝 Step-by-Step Setup

### STEP 1: Database Setup

#### 1.1 Install and Start XAMPP
1. Install XAMPP in `C:\xampp\`
2. Open XAMPP Control Panel
3. Start **Apache** and **MySQL** services

#### 1.2 Create Database
1. Open browser and go to: `http://localhost/phpmyadmin`
2. Click "Import" tab
3. Click "Choose File" and select: `Source_Code/Database/127_0_0_1.sql`
4. Click "Go" to import

**Database Created:**
- Name: `car_parking`
- Tables: `users`, `booking`, `booking_up`, `data`, `slot_1`, `slot_2`, `slot_3`, `motor`

#### 1.3 Verify Database
Run this SQL query to check:
```sql
USE car_parking;
SHOW TABLES;
SELECT * FROM users;
```

---

### STEP 2: Backend PHP Setup

#### 2.1 Copy PHP Files to XAMPP
Copy all files from `Source_Code/` into XAMPP's web directory:

**Option A (Recommended):**
```powershell
# Create symbolic link (easier for development)
New-Item -ItemType SymbolicLink -Path "C:\xampp\htdocs\parking" -Target "C:\Users\Admin\Smart-Parking-System-IoT\Source_Code"
```

**Option B:**
```powershell
# Copy files
Copy-Item -Path "C:\Users\Admin\Smart-Parking-System-IoT\Source_Code\*" -Destination "C:\xampp\htdocs\parking\" -Recurse
```

#### 2.2 Configure Database Connection
All PHP files already use correct settings:
- **Host**: `localhost`
- **Username**: `root`
- **Password**: (empty)
- **Database**: `car_parking`

**No changes needed!** The PHP files are already configured correctly.

#### 2.3 Test Backend APIs
Open browser and test these URLs:

1. **Test Database Connection:**
   ```
   http://localhost/parking/android_db_pool/Sensor_data.php
   ```

2. **Test Login (should return "not found" for invalid credentials):**
   ```
   http://localhost/parking/android_db_pool/login_maker.php?t1=test&t2=test
   ```

3. **Test User Registration:**
   ```
   http://localhost/parking/LogIn-SignUp-master/DataBaseConfig.php?R_name=Test&R_Phone_no=1234567890&R_Email_id=test@test.com&R_Number_plate=MH01AB1234&Pass=12345
   ```

---

### STEP 3: Arduino Hardware Setup

#### 3.1 Upload Code to Arduino Uno
1. Open Arduino IDE
2. Open file: `Source_Code/test.ino`
3. Select: Tools → Board → Arduino Uno (or Mega)
4. Select: Tools → Port → (your Arduino port)
5. Click "Upload" ✓

#### 3.2 Upload Code to ESP8266 NodeMCU
1. Open Arduino IDE
2. Open file: `Source_Code/node.ino`
3. **IMPORTANT: Update WiFi Credentials**
   
   Find this line (around line 22):
   ```cpp
   WiFi.begin("Galaxy M111589","cexo9227");
   ```
   
   Change to YOUR WiFi:
   ```cpp
   WiFi.begin("YOUR_WIFI_NAME","YOUR_WIFI_PASSWORD");
   ```

4. **Update Server IP Address**
   
   Find your computer's IP address:
   ```powershell
   ipconfig
   ```
   Look for "IPv4 Address" (e.g., 192.168.1.100)
   
   Update these lines in `node.ino` (lines 8 and 61):
   ```cpp
   const char* serverName = "http://YOUR_IP_ADDRESS/parking/android_db_pool/Sensor_data.php";
   // and
   http.begin(client,"http://YOUR_IP_ADDRESS/parking/android_db_pool/Sensor_data.php");
   ```
   
   Example:
   ```cpp
   const char* serverName = "http://192.168.1.100/parking/android_db_pool/Sensor_data.php";
   ```

5. Select: Tools → Board → NodeMCU 1.0 (ESP-12E Module)
6. Select: Tools → Port → (your NodeMCU port)
7. Click "Upload" ✓

#### 3.3 Test Arduino Communication
1. Keep both Arduinos connected to PC
2. Open Serial Monitor (Tools → Serial Monitor)
3. Set baud rate to 115200
4. You should see:
   ```
   ..
   ..
   NodeMCU is connected
   192.168.x.x
   Jeson Object Recived
   p1s3off
   p1s2off
   p1s1off
   HTTP Response code: 200
   Values uploaded successfully.
   ```

---

### STEP 4: Android App Setup

#### 4.1 Open Project in Android Studio
1. Open Android Studio
2. File → Open → Select: `Source_Code/SmartParking/`
3. Wait for Gradle sync to complete

#### 4.2 Update API URLs in Android App
Find your computer's IP address using `ipconfig` (e.g., 192.168.1.100)

Update the following files:

**File 1: MainActivity.java** (Line 24)
```java
private static final String apiurl="http://YOUR_IP_ADDRESS/parking/android_db_pool/login_maker.php";
```

**File 2: Register.java** (Line 25)
```java
private final String url = "http://YOUR_IP_ADDRESS/parking/LogIn-SignUp-master/DataBaseConfig.php";
```

**File 3: Status.java** (Line 24)
```java
private static final String url = "http://YOUR_IP_ADDRESS/parking/android_db_pool/status.php";
```

**File 4: Parking.java** (Lines 24-26)
```java
private static final String url_slot1 = "http://YOUR_IP_ADDRESS/parking/LogIn-SignUp-master/Slot_1.php";
private static final String url_slot2 = "http://YOUR_IP_ADDRESS/parking/LogIn-SignUp-master/Slot_2.php";
private static final String url_slot3 = "http://YOUR_IP_ADDRESS/parking/LogIn-SignUp-master/Slot_3.php";
```

**File 5: CancelParking.java** (Line 24)
```java
private static final String url = "http://YOUR_IP_ADDRESS/parking/LogIn-SignUp-master/CancleBooking.php";
```

#### 4.3 Build and Install App
1. Connect Android phone via USB (enable USB Debugging)
   - OR use Android Emulator (AVD Manager)
2. Click Run ▶️ button in Android Studio
3. Select your device
4. Wait for app to install and launch

---

## 🧪 Complete System Testing

### Test 1: User Registration & Login

**Register a new user:**
1. Open app on phone
2. Click "Register"
3. Fill in details:
   - Name: John Doe
   - Phone: 9876543210
   - Email: john@example.com
   - Number Plate: MH12AB1234
   - Password: 12345
4. Click Register
5. Should show "Inserted Successfully"

**Login:**
1. Enter Name: John Doe
2. Enter Password: 12345
3. Click Login
4. Should navigate to Dashboard

### Test 2: View Parking Status

1. In Dashboard, click "Check Parking Status"
2. Should display:
   - Slot 1: Empty/Booked/Full
   - Slot 2: Empty/Booked/Full
   - Slot 3: Empty/Booked/Full
3. Status updates every 5 seconds from sensors

### Test 3: Book a Parking Slot

**Without hardware connected:**
1. Click "Book Parking"
2. Select an available slot (e.g., Slot 1)
3. Click "Book"
4. Check database:
   ```sql
   SELECT * FROM booking_up;
   SELECT * FROM slot_1;
   ```

**With hardware connected:**
1. Book a slot (e.g., Slot 3)
2. Arduino should activate stepper motor
3. Gate opens for 10 seconds
4. Gate closes automatically
5. Check Serial Monitor for confirmation

### Test 4: Sensor Detection

1. Connect all 3 IR sensors to Arduino
2. Place an object in front of IR Sensor 1
3. Check Serial Monitor - should show: `p1s1on`
4. Check database:
   ```sql
   SELECT * FROM data ORDER BY ID DESC LIMIT 5;
   ```
5. Remove object - should show: `p1s1off`

### Test 5: Cancel Booking

1. In app, go to "Cancel Booking"
2. Click "Cancel" for your booking
3. Slot should become available again
4. Verify in database:
   ```sql
   SELECT * FROM booking_up;
   ```

---

## 📊 System Data Flow

```
┌─────────────────┐
│  IR Sensors (3) │
│  Detect Car     │
└────────┬────────┘
         │ Pins 4,5,6
         ▼
┌─────────────────┐      JSON via Serial      ┌──────────────────┐
│  Arduino Uno    │─────────────────────────►│  ESP8266 NodeMCU │
│  (test.ino)     │◄─────────────────────────│  (node.ino)      │
│  + Stepper Motor│      Slot Number         │  + WiFi          │
└─────────────────┘                           └────────┬─────────┘
         ▲                                             │
         │ Barrier Gate Control                        │ HTTP POST
         │                                             ▼
         │                                    ┌─────────────────┐
         │                                    │  PHP Backend    │
         │                                    │  (on XAMPP)     │
         │                                    └────────┬────────┘
         │                                             │
         │                                             ▼
         │                                    ┌─────────────────┐
         └────────────────────────────────────│  MySQL Database │
                 Slot booking triggers        │  (car_parking)  │
                 motor activation             └────────┬────────┘
                                                       │
                                                       ▼
                                              ┌─────────────────┐
                                              │  Android App    │
                                              │  User Interface │
                                              └─────────────────┘
```

---

## 🐛 Troubleshooting

### Arduino Issues

**Problem: ESP8266 can't connect to WiFi**
- Check WiFi name and password in `node.ino`
- Try mobile hotspot instead of router
- Check if WiFi is 2.4GHz (ESP8266 doesn't support 5GHz)

**Problem: "Compilation error: ArduinoJson.h: No such file"**
- Install ArduinoJson library (version 5.x, NOT 6.x)
- Tools → Manage Libraries → Search "ArduinoJson" → Install version 5.13.5

**Problem: Sensors always show 'on' or 'off'**
- Check IR sensor wiring (VCC, GND, OUT)
- Adjust sensor potentiometer (sensitivity)
- Test with multimeter: sensor output should be HIGH (5V) when no object

**Problem: Stepper motor doesn't move**
- Check power supply to ULN2003 driver (5-12V)
- Verify pin connections (9,10,11,12)
- Test motor separately with example sketch

### Backend Issues

**Problem: "Connection failed" in PHP**
- Ensure MySQL is running in XAMPP
- Check database name is `car_parking`
- Verify username is `root` with no password

**Problem: "404 Not Found" when accessing PHP files**
- Verify files are in `C:\xampp\htdocs\parking\`
- Check Apache is running in XAMPP
- Try: `http://localhost/parking/` should show file list

**Problem: Data not inserting into database**
- Check PHP error logs: `C:\xampp\php\logs\php_error_log`
- Open phpMyAdmin and manually run INSERT query
- Verify table structure matches code

### Android App Issues

**Problem: App shows "Unable to connect"**
- Ensure phone and PC are on SAME WiFi network
- Verify IP address in app matches PC's IP (`ipconfig`)
- Check XAMPP Apache is running
- Disable Windows Firewall temporarily to test
- Use browser on phone to test: `http://YOUR_IP/parking/`

**Problem: "Gradle sync failed"**
- Update Android Studio to latest version
- File → Invalidate Caches / Restart
- Check internet connection (downloads dependencies)

**Problem: Login always fails**
- Check database has user with exact name and password
- Test login URL directly in browser:
  ```
  http://YOUR_IP/parking/android_db_pool/login_maker.php?t1=YourName&t2=YourPassword
  ```
- Should return "found" or "not found"

### Network Issues

**Problem: ESP8266 connects but can't reach server**
- Check firewall isn't blocking port 80
- Temporarily disable antivirus
- Use PC's IP address, not "localhost" or "127.0.0.1"
- Test URL from another device on same network

---

## 📱 Default Test Credentials

Database includes sample users:

| Name | Password | Number Plate | Phone |
|------|----------|--------------|-------|
| Kaushik Shigvan | 12345 | MH 03 4646 | 9229101023 |
| Shubham Modi | 12345 | MH JH121234 | 9011108889 |
| Harshad Raurala | 12345 | MH 12JH9022 | 9011102220 |

---

## 🔐 Security Notes

**IMPORTANT: This is a development/educational project. For production:**

1. **Change database password** from empty to strong password
2. **Use prepared statements** in PHP (current code uses string concatenation - SQL injection risk)
3. **Hash passwords** using bcrypt (currently stored in plain text)
4. **Use HTTPS** instead of HTTP
5. **Add input validation** in all forms
6. **Implement proper authentication tokens**

---

## 📂 Project File Structure

```
Source_Code/
├── node.ino                          # ESP8266 WiFi module code
├── test.ino                          # Arduino Uno main controller code
├── Database/
│   └── 127_0_0_1.sql                # MySQL database dump
├── android_db_pool/                  # Main API folder
│   ├── Sensor_data.php              # Receives sensor data from Arduino
│   ├── status.php                   # Returns slot status to app
│   ├── login_maker.php              # User authentication
│   └── dbinfo.txt                   # Database credentials
├── LogIn-SignUp-master/             # User management APIs
│   ├── DataBaseConfig.php           # User registration
│   ├── login.php                    # Alternative login (unused)
│   ├── signup.php                   # Alternative signup (unused)
│   ├── Slot_1.php                   # Book slot 1
│   ├── Slot_2.php                   # Book slot 2
│   ├── Slot_3.php                   # Book slot 3
│   ├── CancleBooking.php            # Cancel booking
│   └── DataBase.php                 # Database helper class
└── SmartParking/                    # Android app
    └── app/src/main/java/com/example/smartparking/
        ├── MainActivity.java        # Login screen
        ├── Register.java            # Registration screen
        ├── Dashboard.java           # Main dashboard
        ├── Parking.java             # Book parking
        ├── Status.java              # View status
        ├── CancelParking.java       # Cancel booking
        ├── Profile.java             # User profile
        └── Logout.java              # Logout function
```

---

## ✅ Next Steps After Setup

1. **Test each component individually first**
2. **Test hardware → backend communication**
3. **Test app → backend communication**
4. **Finally test complete end-to-end flow**

---

## 📞 Need Help?

If you encounter any issues:
1. Check the troubleshooting section above
2. Review XAMPP error logs
3. Check Arduino Serial Monitor output
4. Test each component separately to isolate the issue

---

## 🎉 System Features

✅ Real-time parking slot monitoring
✅ Android app for booking and management
✅ Automated barrier gate control
✅ User registration and authentication
✅ Booking management (book/cancel)
✅ Live status updates
✅ 3 parking slots with individual monitoring

---

*Last Updated: March 2026*
*Project: Smart Parking System IoT*
