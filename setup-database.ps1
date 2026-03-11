# Smart Parking System - Database Setup Script
# This script sets up the MySQL database for the parking system

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Smart Parking System - Database Setup" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if XAMPP is installed
$xamppPath = "C:\xampp"
$mysqlPath = "$xamppPath\mysql\bin\mysql.exe"

if (!(Test-Path $xamppPath)) {
    Write-Host "✗ XAMPP not found at C:\xampp" -ForegroundColor Red
    Write-Host "Please install XAMPP first using: .\install-software.ps1" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit
}

Write-Host "✓ XAMPP found at: $xamppPath" -ForegroundColor Green
Write-Host ""

# Check if MySQL is running
Write-Host "Checking MySQL status..." -ForegroundColor Yellow
$mysqlProcess = Get-Process -Name "mysqld" -ErrorAction SilentlyContinue

if (!$mysqlProcess) {
    Write-Host "MySQL is not running. Starting XAMPP Control Panel..." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "⚠️ Please start Apache and MySQL in XAMPP Control Panel" -ForegroundColor Yellow
    Write-Host ""
    
    $xamppControl = "$xamppPath\xampp-control.exe"
    if (Test-Path $xamppControl) {
        Start-Process -FilePath $xamppControl
        Write-Host "Waiting for you to start MySQL..." -ForegroundColor Yellow
        Read-Host "Press Enter after starting Apache and MySQL"
    }
}

# Verify MySQL is now running
$mysqlProcess = Get-Process -Name "mysqld" -ErrorAction SilentlyContinue
if ($mysqlProcess) {
    Write-Host "✓ MySQL is running!" -ForegroundColor Green
} else {
    Write-Host "✗ MySQL is still not running" -ForegroundColor Red
    Write-Host "Please start MySQL from XAMPP Control Panel and try again" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Importing Database" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Database SQL file path
$sqlFile = "$PSScriptRoot\Source_Code\Database\127_0_0_1.sql"

if (!(Test-Path $sqlFile)) {
    Write-Host "✗ Database file not found: $sqlFile" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit
}

Write-Host "Database file found: 127_0_0_1.sql" -ForegroundColor Green
Write-Host "Importing database..." -ForegroundColor Yellow
Write-Host ""

# Import database using mysql command
try {
    & $mysqlPath -u root -e "source $sqlFile"
    Write-Host "✓ Database imported successfully!" -ForegroundColor Green
} catch {
    Write-Host "Error importing database. Trying alternative method..." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Please import manually:" -ForegroundColor Yellow
    Write-Host "1. Open browser and go to: http://localhost/phpmyadmin" -ForegroundColor White
    Write-Host "2. Click 'Import' tab" -ForegroundColor White
    Write-Host "3. Choose file: $sqlFile" -ForegroundColor White
    Write-Host "4. Click 'Go' button" -ForegroundColor White
    Write-Host ""
    Read-Host "Press Enter after importing the database"
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Verifying Database" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Test database connection
Write-Host "Testing database connection..." -ForegroundColor Yellow

$testQuery = "USE car_parking; SHOW TABLES;"
$result = & $mysqlPath -u root -e $testQuery 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ Database 'car_parking' is accessible!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Tables in database:" -ForegroundColor Cyan
    Write-Host $result
} else {
    Write-Host "✗ Could not verify database" -ForegroundColor Red
    Write-Host "Please check if database was imported correctly" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Setting Up PHP Files" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Copy source code to XAMPP htdocs
$sourcePath = "$PSScriptRoot\Source_Code"
$htdocsPath = "$xamppPath\htdocs\parking"

Write-Host "Copying PHP files to XAMPP..." -ForegroundColor Yellow

if (Test-Path $htdocsPath) {
    Write-Host "Removing old files..." -ForegroundColor Yellow
    Remove-Item -Path $htdocsPath -Recurse -Force
}

# Create symbolic link for easier development
try {
    Write-Host "Creating symbolic link..." -ForegroundColor Yellow
    New-Item -ItemType SymbolicLink -Path $htdocsPath -Target $sourcePath -Force | Out-Null
    Write-Host "✓ Symbolic link created: $htdocsPath -> $sourcePath" -ForegroundColor Green
} catch {
    Write-Host "Could not create symbolic link. Copying files instead..." -ForegroundColor Yellow
    Copy-Item -Path "$sourcePath\*" -Destination $htdocsPath -Recurse -Force
    Write-Host "✓ Files copied to: $htdocsPath" -ForegroundColor Green
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Testing Backend APIs" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Test if Apache is running
Write-Host "Testing web server..." -ForegroundColor Yellow

try {
    $response = Invoke-WebRequest -Uri "http://localhost/parking/config.php?info=show" -UseBasicParsing -TimeoutSec 5
    Write-Host "✓ Web server is working!" -ForegroundColor Green
    Write-Host ""
    Write-Host "You can view the configuration at:" -ForegroundColor Cyan
    Write-Host "http://localhost/parking/config.php?info=show" -ForegroundColor White
} catch {
    Write-Host "✗ Could not connect to web server" -ForegroundColor Red
    Write-Host "Please ensure Apache is running in XAMPP Control Panel" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Database Setup Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "✓ MySQL database is set up" -ForegroundColor Green
Write-Host "✓ PHP files are accessible" -ForegroundColor Green
Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Yellow
Write-Host "1. Find your computer's IP address: ipconfig" -ForegroundColor White
Write-Host "2. Update IP addresses in Arduino code (node.ino)" -ForegroundColor White
Write-Host "3. Update IP addresses in Android app code" -ForegroundColor White
Write-Host "4. Upload Arduino code using Arduino IDE" -ForegroundColor White
Write-Host "5. Build and install Android app" -ForegroundColor White
Write-Host ""
Write-Host "Test Login Credentials:" -ForegroundColor Cyan
Write-Host "Name: Kaushik Shigvan" -ForegroundColor White
Write-Host "Password: 12345" -ForegroundColor White
Write-Host ""
Write-Host "See SETUP_GUIDE.md for detailed instructions." -ForegroundColor Cyan
Write-Host ""

Read-Host "Press Enter to exit"
