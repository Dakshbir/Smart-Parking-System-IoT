# Required Code Fixes Before Testing

## 🔴 Critical Issues That Must Be Fixed

### 1. Android App - Register.java - Wrong API URL

**File:** `Source_Code/SmartParking/app/src/main/java/com/example/smartparking/Register.java`

**Current (Line 25):**
```java
private final String url = "http://192.168.43.199/Login-Signup-master/DataBaseConfig.php";
```

**Problem:** DataBaseConfig.php is a configuration file, not a registration endpoint!

**Should be:**
```java
private final String url = "http://192.168.43.199/LogIn-SignUp-master/DataBaseConfig.php";
```

**Actually, this should point to a proper registration API. Looking at the code, there's a separate registration script.**

**BEST FIX - Create new registration endpoint or use existing one:**

The app currently sends data to DataBaseConfig.php with parameters:
- R_name, R_Phone_no, R_Email_id, R_Number_plate, Pass

We need to check if there's a proper registration endpoint. Based on analysis, we should create one or fix the existing path.

---

### 2. PHP Slot Files - Logic Bug (Assignment vs Comparison)

**Files:** 
- `Source_Code/LogIn-SignUp-master/Slot_1.php` (Line 47)
- `Source_Code/LogIn-SignUp-master/Slot_2.php` (Line 47)
- `Source_Code/LogIn-SignUp-master/Slot_3.php` (Line 47)

**Problem:** Using assignment operator `=` instead of comparison operator `==`

**Slot_1.php - Line 47:**
```php
if($id = 'p1s3off'){  // WRONG - this assigns, doesn't compare!
```

**Should be:**
```php
if($id == 'p1s3off'){  // Correct comparison
```

**And Line 70:**
```php
else if($id = 'p1s3on'){  // WRONG
```

**Should be:**
```php
else if($id == 'p1s3on'){  // Correct
```

**Same issue in Slot_2.php and Slot_3.php with their respective sensor checks.**

---

### 3. PHP Slot Files - Wrong Sensor Variable

**Slot_1.php:**
Currently checks `$id` which is `Sensor1` but should check `Sensor3` for slot 1.

Looking at the database structure:
- `Sensor1` = p1s3xxx (Slot 3)
- `Sensor2` = p1s2xxx (Slot 2)  
- `Sensor3` = p1s1xxx (Slot 1)

**The mapping is reversed!**

**In Slot_1.php, it should check:**
```php
$id = $data_1['Sensor3'];  // Currently checks Sensor1
```

**In Slot_2.php:** Already correct - checks `$id_2` (Sensor2)

**In Slot_3.php:** Should check Sensor1 instead of Sensor3
```php
$id = $data_1['Sensor1'];  // Currently checks Sensor3
```

---

### 4. Missing Registration API File

**Problem:** The Android app tries to register users, but the endpoint doesn't exist properly.

**Current Registration Flow:**
- App sends data to: `LogIn-SignUp-master/DataBaseConfig.php`
- But DataBaseConfig.php is just a PHP class file!

**Existing Working Registration:**
There IS a working registration at:
`android_db_pool/` folder - but no clear registration endpoint there either.

**Solution:** Create a proper registration endpoint or fix the Register.java to use the correct path.

Looking at the code more carefully, DataBaseConfig.php is actually being called directly and it should work, but we need to verify.

Actually, I see the issue - the app sends GET parameters but DataBaseConfig.php expects to be included, not called directly.

**We need to create: `LogIn-SignUp-master/register.php`**

---

### 5. Database Credentials Mismatch

**File:** `Source_Code/android_db_pool/dbinfo.txt`

**Current:**
```
and_db
and_db_user
and_db_user#@!
```

**But actual PHP files use:**
```php
$host = "localhost";
$username = "root";
$password = "";
$dbname = "car_parking";
```

**Fix:** Either update dbinfo.txt or update PHP files to read from dbinfo.txt.

For simplicity, we'll keep using the hardcoded values in PHP files since that's what's actually being used.

---

## ⚠️ Non-Critical Issues (Can work, but should be improved)

### 6. SQL Injection Vulnerabilities

**All PHP files use direct string concatenation:**
```php
$qry = "SELECT * FROM `users` WHERE `name` LIKE '$uname' AND `password` LIKE '$pwd'";
```

**Should use prepared statements:**
```php
$stmt = $conn->prepare("SELECT * FROM users WHERE name = ? AND password = ?");
$stmt->bind_param("ss", $uname, $pwd);
```

**Status:** Will work for testing, but should be fixed for production.

---

### 7. Plain Text Passwords

**Problem:** Passwords are stored in plain text in database.

**Fix:** Use password hashing:
```php
// When registering:
$hashed_password = password_hash($password, PASSWORD_DEFAULT);

// When logging in:
password_verify($password, $hashed_password);
```

**Status:** Will work for testing, but should be fixed for production.

---

### 8. Hardcoded IP Address in Multiple Locations

**Files to update when IP changes:**
1. `node.ino` - Lines 8, 61  
2. `MainActivity.java` - Line 24
3. `Register.java` - Line 25
4. `Status.java` - Line 24
5. `Parking.java` - Lines 24-26
6. `CancelParking.java` - Line 24

**Better Solution:** Create a configuration file or use a constant.

**Status:** Known issue, must be updated with actual IP before testing.

---

## 🔧 Files That Need Modification

### Must Fix Now:
1. ✅ `Slot_1.php` - Fix comparison operators and sensor variable
2. ✅ `Slot_2.php` - Fix comparison operators
3. ✅ `Slot_3.php` - Fix comparison operators and sensor variable
4. ✅ Create `register.php` for proper user registration
5. ⏳ Update all IP addresses (do this during setup)

### Should Fix (But Not Blocking):
- SQL injection protection (add prepared statements)
- Password hashing
- Error handling
- Input validation

---

## 📝 Summary of Changes Needed

| Priority | File | Issue | Line(s) |
|----------|------|-------|---------|
| 🔴 HIGH | Slot_1.php | `=` instead of `==` | 47, 70 |
| 🔴 HIGH | Slot_1.php | Wrong sensor variable | 25 |
| 🔴 HIGH | Slot_2.php | `=` instead of `==` | 47, 71 |
| 🔴 HIGH | Slot_3.php | `=` instead of `==` | 47, 72 |
| 🔴 HIGH | Slot_3.php | Wrong sensor variable | 25 |
| 🔴 HIGH | Create register.php | Missing endpoint | N/A |
| 🟡 MEDIUM | All Java files | Update IP addresses | Various |
| 🟡 MEDIUM | node.ino | Update WiFi + IP | 22, 8, 61 |
| 🟢 LOW | All PHP files | SQL injection | All queries |
| 🟢 LOW | All PHP files | Password hashing | Login/Register |

---

## ✅ Apply Fixes

Would you like me to automatically apply all the critical fixes now?

I can fix:
1. All PHP comparison operator bugs
2. Sensor variable corrections  
3. Create the missing registration endpoint
4. Add a configuration helper for IP addresses

Then you'll just need to update WiFi credentials and IP addresses before testing!
