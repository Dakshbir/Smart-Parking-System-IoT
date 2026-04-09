---
name: Smart Parking System IoT - Development Workflow
description: Complete workflow orchestration guide for Smart Parking System IoT project development, including planning, verification, and implementation principles.
tags: [workflow, planning, verification, arduino, php-backend, android-app]
applyTo: ["**/*.ino", "**/*.php", "**/*.java", "*.md"]
---

# Smart Parking System IoT - Development Workflow

## Project Overview
- **Type:** IoT Parking Management System
- **Hardware:** Arduino Uno + ESP8266 NodeMCU + 3 IR Sensors + Stepper Motor
- **Backend:** PHP with MySQL database
- **Frontend:** Android mobile app
- **Network:** WiFi-based communication at 10.100.70.36

---

## Workflow Orchestration

### 1. Plan Node Default
- **Enter plan mode for ANY non-trivial task** (3+ steps or architectural decisions)
- If something goes sideways, **STOP and re-plan immediately** - don't keep pushing forward
- Use plan mode for verification steps, not just building
- Write detailed specs upfront to reduce ambiguity and prevent rework

### 2. Subagent Strategy
- Use subagents liberally to keep main context window clean
- Offload research, exploration, and parallel analysis to subagents
- For complex problems, throw more compute at it via subagents
- One tack per subagent for focused execution

### 3. Self-Improvement Loop
- After ANY correction from the user: update this file or create `tasks/lessons.md` with the pattern
- Write rules for yourself that prevent the same mistake
- Ruthlessly iterate on these lessons until mistake rate drops
- Review lessons at session start for relevant project context

### 4. Verification Before Done
- **NEVER mark a task complete without proving it works**
- Diff behavior between main and changes when relevant
- Ask yourself: "Would a staff engineer approve this?"
- Run tests, check logs, demonstrate correctness
- For this project: Test API endpoints, verify sensor reads, confirm database entries

### 5. Demand Elegance (Balanced)
- For non-trivial changes: pause and ask "is there a more elegant way?"
- If a fix feels hacky: "Knowing everything I know now, implement the elegant solution"
- Skip this for simple, obvious fixes - don't over-engineer
- Challenge your own work before presenting it

### 6. Autonomous Bug Fixing
- When given a bug report: just fix it. Don't ask for hand-holding
- Point at logs, errors, failing tests - then resolve them
- Zero context switching required from the user
- Go fix failing CI tests without being told how

---

## Task Management

### 1. Plan First
Write plan to `tasks/todo.md` with checkable items before starting implementation

### 2. Verify Plan
Check in with user before starting major implementation

### 3. Track Progress
Mark items complete as you go using `manage_todo_list` tool

### 4. Explain Changes
High-level summary at each step - keep user informed

### 5. Document Results
Add review section to `tasks/todo.md` after completion

### 6. Capture Lessons
Update `tasks/lessons.md` after corrections from user

---

## Core Principles

### 1. Simplicity First
- Make every change as simple as possible
- Impact minimal code
- Prefer existing patterns over new abstractions

### 2. No Laziness
- Find root causes, don't apply temporary fixes
- Maintain senior developer standards
- Refactor when pattern emerges, not on first occurrence

### 3. Minimal Impact
- Changes should only touch what's necessary
- Avoid introducing bugs in unrelated code
- Test ripple effects before committing

---

## Project-Specific Architecture

### Hardware Communication Flow
```
Arduino Uno (Sensors/Motor) 
    ↓ Serial (9600 baud)
ESP8266 NodeMCU (WiFi)
    ↓ HTTP POST (WiFi)
PHP Backend (10.100.70.36:80)
    ↓ Query/Insert
MySQL Database (car_parking)
```

### Database Schema (9 tables)
- `users` - Login credentials
- `booking` - Parking bookings
- `booking_up` - Booking updates
- `data` - Raw sensor data
- `slot_1/2/3` - Individual slot status
- `motor` - Motor control log
- `d_book` - Canceled bookings

### Sensor Mapping (Critical!)
**Database→Slot Mapping (reversed logic):**
- Sensor1 (id) → Slot 3
- Sensor2 (id_2) → Slot 2  
- Sensor3 (id_3) → Slot 1

**Why:** Sensor_data.php assigns sequentially; always verify in Slot_*.php which column you're reading.

---

## Code Conventions

### Arduino (.ino files)
```cpp
// Use 115200 baud for serial communication
Serial.begin(115200);

// WiFi configuration (already updated to "Nord 3"/"daksh0708")
WiFi.begin("SSID", "PASSWORD");

// Server IP (use 10.100.70.36 for all HTTP calls)
const char* serverName = "http://10.100.70.36/parking/...";

// Sensor pins (Arduino Uno)
// Slot 1 sensor: pin 4 (Sensor3 in database)
// Slot 2 sensor: pin 5 (Sensor2)
// Slot 3 sensor: pin 6 (Sensor1)
// Motor control: pins 9, 10, 11, 12
```

### PHP Backend
```php
// CRITICAL: Always use == for comparison, not =
if ($status == 'vacant') { ... }  // ✅ Correct
if ($status = 'vacant') { ... }   // ❌ Bug: assigns instead of compares

// Always use centralized IP config
define('SERVER_IP', '10.100.70.36');

// Database operations
$conn = new mysqli(SERVER_IP, 'root', '', 'car_parking');
$id = mysqli_real_escape_string($conn, $_GET['id']);
```

### Android Java
```java
// Update all server URLs to match:
String url = "http://10.100.70.36/parking/android_db_pool/login_maker.php";

// Use AsyncTask for HTTP operations
```

---

## Common Development Tasks

### Adding New API Endpoint
1. Create `.php` file in appropriate directory (LogIn-SignUp-master/ or android_db_pool/)
2. Test with config.php?info=show to verify connectivity
3. Update Android Java file with new URL
4. Test with Android emulator or device

### Fixing Sensor Issues  
1. Check sensor mapping (Sensor1=Slot3, not Slot1!)
2. Verify Arduino pin configuration matches wiring
3. Read serial output: `Serial.println(sensorValue)`
4. Check database `data` table for raw readings

### Database Changes
1. Always backup first: `mysqldump -u root car_parking > backup.sql`
2. Apply changes via phpMyAdmin
3. Verify with SELECT query
4. Update all related .php files

---

## Build & Test Commands

### Backend Testing
```bash
# Test configuration page (shows all green checkmarks)
http://localhost/parking/config.php?info=show

# Monitor raw sensor data
http://localhost/parking/Source_Code/Final_Year_Project/Sensor_data.php

# Check slot status
http://localhost/parking/Source_Code/android_db_pool/status.php
```

### Arduino IDE Commands
- **Upload to Uno:** Select Board: Arduino Uno → Upload test.ino
- **Upload to ESP8266:** Select Board: NodeMCU 1.0 (ESP-12E) → Upload speed: 115200 → Upload node.ino
- **Monitor serial:** Tools → Serial Monitor → 115200 baud for debug output

### Android Build
```bash
# In Android Studio
Build → Make Project  # Compiles code
Run → Run 'app'       # Deploy to emulator/device
```

---

## Common Pitfalls & Solutions

| Issue | Cause | Solution |
|-------|-------|----------|
| Motor doesn't activate | Wrong sensor slot mapping | Verify reading Sensor3 for Slot1, Sensor1 for Slot3 |
| ESP8266 won't connect | WiFi SSID/password wrong | Update WiFi.begin() with "Nord 3"/"daksh0708" |
| Android app crashes | Wrong IP in URL strings | Search for "192.168" and ensure all replaced with "10.100.70.36" |
| Database errors | pma__ tables conflict | Use `--force` flag during import |
| Assignment instead of comparison | Typo in PHP | Always use `==` not `=` for if conditions |
| Sensor data missing | Serial communication broken | Check Arduino Uno↔ESP8266 wiring at pins marked SoftwareSerial |

---

## Implementation Checklist

**Previously Completed:**
- [x] Full codebase analysis
- [x] Critical bug fixes (5 bugs corrected)
- [x] XAMPP installation with Apache + MySQL
- [x] Arduino IDE installation with ESP8266 board support
- [x] Android Studio installation
- [x] MySQL database (car_parking) imported
- [x] PHP backend deployed to C:\xampp\htdocs\parking
- [x] IP addresses updated from 192.168.43.199 → 10.100.70.36 (8 files)
- [x] WiFi credentials updated (WiFi.begin -> "Nord 3"/"daksh0708")
- [x] ArduinoJson 5.13.5 library installed

**Currently In Progress:**
- [ ] Upload test.ino to Arduino Uno
- [ ] Upload node.ino to ESP8266 NodeMCU
- [ ] Hardware wiring (sensors + motor)
- [ ] Test WiFi connection & data transmission
- [ ] Build Android app in Android Studio
- [ ] Install Android app on device/emulator
- [ ] End-to-end system testing

---

## References

- **Hardware Setup:** See [SETUP_GUIDE.md](./SETUP_GUIDE.md)
- **Bug Fixes Applied:** See [BUG_FIXES.md](./BUG_FIXES.md)
- **Quick Start:** See [QUICKSTART.md](./QUICKSTART.md)
- **Original Workflow:** See [CLAUDE.md](./CLAUDE.md)

---

## When to Update This File

Update this file when:
- A new pattern emerges that should be a convention
- A common pitfall is discovered
- Architecture decisions change
- New best practices are identified
- Sensor/database mappings are corrected

Do NOT update for:
- One-off fixes (capture in BUG_FIXES.md instead)
- Temporary workarounds (capture in tasks/lessons.md)
- Project-specific credentials (use environment variables)
