# Smart Parking System - IP Address Update Script
# This script helps update IP addresses across all files

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Smart Parking IP Address Update Tool" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "This tool will update IP addresses in:" -ForegroundColor Yellow
Write-Host "- Arduino code (node.ino)" -ForegroundColor White
Write-Host "- Android app (5 Java files)" -ForegroundColor White
Write-Host "- PHP configuration" -ForegroundColor White
Write-Host ""

# Get current IP address
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Step 1: Find Your IP Address" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "Finding your network IP addresses..." -ForegroundColor Yellow
Write-Host ""

$ipAddresses = Get-NetIPAddress -AddressFamily IPv4 | Where-Object {$_.IPAddress -ne "127.0.0.1"} | Select-Object IPAddress, InterfaceAlias

if ($ipAddresses) {
    Write-Host "Available IP addresses:" -ForegroundColor Green
    $ipAddresses | Format-Table -AutoSize
} else {
    Write-Host "Could not detect IP addresses automatically" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "To find your IP manually:" -ForegroundColor Cyan
Write-Host "1. Run: ipconfig" -ForegroundColor White
Write-Host "2. Look for 'IPv4 Address' under your active network" -ForegroundColor White
Write-Host "3. It should look like: 192.168.x.x" -ForegroundColor White
Write-Host ""

# Get IP from user
$currentIP = "192.168.43.199"
Write-Host "Current IP in code: $currentIP" -ForegroundColor Yellow
Write-Host ""

$newIP = Read-Host "Enter your computer's IP address (e.g., 192.168.1.100)"

if ([string]::IsNullOrWhiteSpace($newIP)) {
    Write-Host "✗ No IP address provided. Exiting..." -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit
}

# Validate IP format
if ($newIP -notmatch '^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$') {
    Write-Host "✗ Invalid IP address format" -ForegroundColor Red
    Write-Host "Expected format: xxx.xxx.xxx.xxx (e.g., 192.168.1.100)" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit
}

Write-Host ""
Write-Host "Will update IP from: $currentIP" -ForegroundColor Yellow
Write-Host "               to: $newIP" -ForegroundColor Green
Write-Host ""
$confirm = Read-Host "Proceed with update? (y/n)"

if ($confirm -ne "y") {
    Write-Host "Update cancelled" -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Step 2: Updating Arduino Code" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Update node.ino
$nodeIno = "$PSScriptRoot\Source_Code\node.ino"
if (Test-Path $nodeIno) {
    $content = Get-Content $nodeIno -Raw
    $content = $content -replace "http://$currentIP", "http://$newIP"
    Set-Content -Path $nodeIno -Value $content
    Write-Host "✓ Updated: node.ino" -ForegroundColor Green
} else {
    Write-Host "✗ File not found: node.ino" -ForegroundColor Red
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Step 3: Updating Android App" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$javaFiles = @(
    "MainActivity.java",
    "Register.java",
    "Status.java",
    "Parking.java",
    "CancelParking.java"
)

$javaPath = "$PSScriptRoot\Source_Code\SmartParking\app\src\main\java\com\example\smartparking"

foreach ($file in $javaFiles) {
    $filePath = "$javaPath\$file"
    if (Test-Path $filePath) {
        $content = Get-Content $filePath -Raw
        $content = $content -replace "http://$currentIP", "http://$newIP"
        Set-Content -Path $filePath -Value $content
        Write-Host "✓ Updated: $file" -ForegroundColor Green
    } else {
        Write-Host "✗ File not found: $file" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Step 4: Updating PHP Configuration" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$configPhp = "$PSScriptRoot\Source_Code\config.php"
if (Test-Path $configPhp) {
    $content = Get-Content $configPhp -Raw
    $content = $content -replace "define\('SERVER_IP', '$currentIP'\)", "define('SERVER_IP', '$newIP')"
    Set-Content -Path $configPhp -Value $content
    Write-Host "✓ Updated: config.php" -ForegroundColor Green
} else {
    Write-Host "✗ File not found: config.php" -ForegroundColor Red
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "IP Update Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "Updated IP address in all files:" -ForegroundColor Green
Write-Host "• node.ino (Arduino ESP8266)" -ForegroundColor White
Write-Host "• 5 Android app Java files" -ForegroundColor White
Write-Host "• config.php" -ForegroundColor White
Write-Host ""

Write-Host "Next Steps:" -ForegroundColor Yellow
Write-Host "1. Update WiFi credentials in node.ino" -ForegroundColor White
Write-Host "2. Upload node.ino to ESP8266 NodeMCU" -ForegroundColor White
Write-Host "3. Upload test.ino to Arduino Uno" -ForegroundColor White
Write-Host "4. Build and install Android app" -ForegroundColor White
Write-Host ""
Write-Host "See SETUP_GUIDE.md for complete instructions." -ForegroundColor Cyan
Write-Host ""

Read-Host "Press Enter to exit"
