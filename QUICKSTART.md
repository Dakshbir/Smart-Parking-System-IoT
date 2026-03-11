# ⚡ Quick Start Guide - Smart Parking System

This is a **fast-track** setup guide. For detailed instructions, see [SETUP_GUIDE.md](SETUP_GUIDE.md).

---

## ✅ What's Been Fixed

All critical bugs have been **automatically fixed**:
- ✅ Fixed PHP comparison operators (= to ==)
- ✅ Fixed sensor variable mappings
- ✅ Created proper registration endpoint (register.php)
- ✅ Updated Android app to use correct API
- ✅ Created configuration helper (config.php)

---

## 🚀 Quick Setup (6 Steps)

### **Step 1: Install Software** ⏱️ 20-30 minutes

Run the installation script:
```powershell
.\install-software.ps1
```

**Or install manually:**
1. **XAMPP** (Apache + MySQL): https://www.apachefriends.org/download.html
2. **Arduino IDE**: https://www.arduino.cc/en/software
3. **Android Studio**: https://developer.android.com/studio

---

### **Step 2: Setup Database** ⏱️ 5 minutes

Run the database setup script:
```powershell
.\setup-database.ps1
```

**What it does:**
- Starts XAMPP Apache and MySQL
- Imports database from `Source_Code/Database/127_0_0_1.sql`
- Copies PHP files to `C:\xampp\htdocs\parking\`
- Tests that everything works

**Or do manually:**
1. Start XAMPP Control Panel
2. Start Apache and MySQL
3. Open http://localhost/phpmyadmin
4. Import: `Source_Code/Database/127_0_0_1.sql`

---

### **Step 3: Update IP Addresses** ⏱️ 2 minutes

Find your IP address:
```powershell
ipconfig
```
Look for **IPv4 Address** (e.g., 192.168.1.100)

Run the IP update script:
```powershell
.\update-ip.ps1
```

**What it updates:**
- Arduino code (node.ino)
- Android app (5 Java files)
- PHP config

---

### **Step 4: Configure WiFi** ⏱️ 1 minute

Edit `Source_Code/node.ino` (line 22):
```cpp
WiFi.begin("YOUR_WIFI_NAME","YOUR_WIFI_PASSWORD");
```

**Important:** Use the same WiFi network your computer is connected to!

---

### **Step 5: Upload Arduino Code** ⏱️ 10 minutes

#### Install ESP8266 Board Support:
1. Arduino IDE → File → Preferences
2. Add to "Additional Board Manager URLs":
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Tools → Board → Boards Manager
4. Search "ESP8266" → Install

#### Install ArduinoJson Library:
1. Tools → Manage Libraries
2. Search "ArduinoJson"
3. Install **version 5.13.5** (NOT version 6.x)

#### Upload to Arduino Uno:
1. Open `Source_Code/test.ino`
2. Tools → Board → Arduino Uno
3. Tools → Port → (select your Arduino)
4. Click **Upload** ✓

#### Upload to ESP8266 NodeMCU:
1. Open `Source_Code/node.ino`
2. Tools → Board → NodeMCU 1.0 (ESP-12E Module)
3. Tools → Port → (select your NodeMCU)
4. Click **Upload** ✓

#### Test Arduino:
1. Tools → Serial Monitor
2. Set baud rate: **115200**
3. Should see:
   ```
   NodeMCU is connected
   192.168.x.x
   HTTP Response code: 200
   ```

---

### **Step 6: Build Android App** ⏱️ 15 minutes

1. Open Android Studio
2. File → Open → Select `Source_Code/SmartParking/`
3. Wait for Gradle sync
4. Connect Android phone via USB (enable USB Debugging)
   - **Or** create emulator: Tools → AVD Manager
5. Click **Run** ▶️ button
6. Wait for app to install

---

## 🎯 Test the System

### Test 1: Login to App
1. Open app on phone
2. Use test credentials:
   - **Name:** Kaushik Shigvan
   - **Password:** 12345
3. Should login successfully

### Test 2: Check Slot Status
1. In app, click "Check Parking Status"
2. Should show 3 slots with current status

### Test 3: Book a Slot
1. Click "Book Parking"
2. Select a slot
3. Click "Book"
4. ✨ Arduino should activate motor!

### Test 4: Sensor Detection
1. Place object in front of IR sensor
2. Check Serial Monitor - should show "p1s1on"
3. Check app - status should update

---

## 🐛 Quick Troubleshooting

### Arduino won't connect to WiFi
- Check WiFi name/password in node.ino
- Use 2.4GHz WiFi (ESP8266 doesn't support 5GHz)
- Try mobile hotspot instead

### App shows "Unable to connect"
- Ensure phone and PC on SAME WiFi
- Verify IP address: `ipconfig`
- Test URL in phone's browser: `http://YOUR_IP/parking/config.php?info=show`
- Disable Windows Firewall temporarily

### Database connection failed
- Check XAMPP MySQL is running
- Open http://localhost/phpmyadmin
- Verify database "car_parking" exists

### "404 Not Found" for PHP files
- Verify files in `C:\xampp\htdocs\parking\`
- Check XAMPP Apache is running
- Try: http://localhost/parking/

---

## 📦 Hardware Shopping List

If you need to buy components:

### Arduino Boards:
- 1x Arduino Uno R3 (~$20)
- 1x ESP8266 NodeMCU (~$5)

### Sensors & Motors:
- 3x IR Obstacle Sensor Module (~$3 each)
- 1x 28BYJ-48 Stepper Motor with ULN2003 Driver (~$5)

### Accessories:
- Breadboard
- Jumper wires (Male-Male, Male-Female)
- 2x USB cables (for Arduino power/programming)

**Total Cost:** ~$50-60

---

## 🎓 System Architecture

```
[3x IR Sensors] → [Arduino Uno] ←Serial→ [ESP8266 NodeMCU]
      │              + Stepper Motor              │ WiFi
      │                                           ↓
      └────────────────────────────────→ [PHP Backend + MySQL]
                                                  ↕ HTTP
                                         [Android App]
```

---

## 📚 Additional Resources

- **Full Setup Guide:** [SETUP_GUIDE.md](SETUP_GUIDE.md)
- **Bug Fixes List:** [FIXES_REQUIRED.md](FIXES_REQUIRED.md)
- **Configuration Helper:** http://localhost/parking/config.php?info=show

---

## ✨ Features

✅ Real-time parking slot monitoring with IR sensors  
✅ Mobile app for booking and management  
✅ Automated barrier gate with stepper motor  
✅ User registration and authentication  
✅ Live status updates  
✅ 3 parking slots with individual tracking  

---

## 💡 Pro Tips

1. **Test incrementally** - Don't do everything at once
2. **Serial Monitor is your friend** - Always check for errors
3. **Backup your WiFi config** - So you don't have to re-enter it
4. **Use mobile hotspot** - If router WiFi doesn't work
5. **Check firewall** - It often blocks local network access

---

## 🎉 You're Ready!

Everything is set up and ready to test. Follow the 6 steps above and you'll have a working parking system in about **1 hour**!

**Good luck! 🚀**

---

*Created: March 2026*  
*Smart Parking System IoT Project*
