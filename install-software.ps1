# Smart Parking System - Installation Script
# Run this script to download and install all required software

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Smart Parking System - Auto Installer" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Create temp download directory
$downloadPath = "$env:TEMP\SmartParkingSetup"
if (!(Test-Path $downloadPath)) {
    New-Item -ItemType Directory -Path $downloadPath | Out-Null
}

Write-Host "Download directory: $downloadPath" -ForegroundColor Yellow
Write-Host ""

# Function to download files
function Download-File {
    param (
        [string]$Url,
        [string]$OutputPath,
        [string]$Name
    )
    
    Write-Host "Downloading $Name..." -ForegroundColor Green
    try {
        $ProgressPreference = 'SilentlyContinue'
        Invoke-WebRequest -Uri $Url -OutFile $OutputPath -UseBasicParsing
        Write-Host "✓ Downloaded: $Name" -ForegroundColor Green
        return $true
    } catch {
        Write-Host "✗ Failed to download: $Name" -ForegroundColor Red
        Write-Host "  Error: $_" -ForegroundColor Red
        return $false
    }
}

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "STEP 1: XAMPP Installation" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# XAMPP Download
$xamppUrl = "https://sourceforge.net/projects/xampp/files/XAMPP%20Windows/8.2.12/xampp-windows-x64-8.2.12-0-VS16-installer.exe"
$xamppInstaller = "$downloadPath\xampp-installer.exe"

Write-Host "XAMPP provides Apache web server and MySQL database." -ForegroundColor Yellow
Write-Host ""
Write-Host "Option 1: Automatic Download (may be slow)" -ForegroundColor Yellow
Write-Host "Option 2: Manual Download (faster)" -ForegroundColor Yellow
Write-Host ""
$xamppChoice = Read-Host "Choose option (1 or 2)"

if ($xamppChoice -eq "1") {
    if (Download-File -Url $xamppUrl -OutputPath $xamppInstaller -Name "XAMPP") {
        Write-Host ""
        Write-Host "Starting XAMPP installation..." -ForegroundColor Green
        Write-Host "⚠️ IMPORTANT: Install to C:\xampp (default location)" -ForegroundColor Yellow
        Start-Process -FilePath $xamppInstaller -Wait
    }
} else {
    Write-Host ""
    Write-Host "Please download XAMPP manually:" -ForegroundColor Yellow
    Write-Host "1. Open browser and go to: https://www.apachefriends.org/download.html" -ForegroundColor White
    Write-Host "2. Download: XAMPP for Windows (PHP 7.4 or higher)" -ForegroundColor White
    Write-Host "3. Run the installer" -ForegroundColor White
    Write-Host "4. Install to: C:\xampp" -ForegroundColor White
    Write-Host ""
    Read-Host "Press Enter after you've installed XAMPP"
}

# Verify XAMPP installation
if (Test-Path "C:\xampp") {
    Write-Host "✓ XAMPP is installed!" -ForegroundColor Green
} else {
    Write-Host "✗ XAMPP not found at C:\xampp" -ForegroundColor Red
    Write-Host "Please install XAMPP before continuing." -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "STEP 2: Arduino IDE Installation" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$arduinoUrl = "https://downloads.arduino.cc/arduino-1.8.19-windows.exe"
$arduinoInstaller = "$downloadPath\arduino-installer.exe"

Write-Host "Arduino IDE is needed to program the hardware boards." -ForegroundColor Yellow
Write-Host ""
Write-Host "Option 1: Automatic Download" -ForegroundColor Yellow
Write-Host "Option 2: Manual Download" -ForegroundColor Yellow
Write-Host ""
$arduinoChoice = Read-Host "Choose option (1 or 2)"

if ($arduinoChoice -eq "1") {
    if (Download-File -Url $arduinoUrl -OutputPath $arduinoInstaller -Name "Arduino IDE") {
        Write-Host ""
        Write-Host "Starting Arduino IDE installation..." -ForegroundColor Green
        Start-Process -FilePath $arduinoInstaller -Wait
    }
} else {
    Write-Host ""
    Write-Host "Please download Arduino IDE manually:" -ForegroundColor Yellow
    Write-Host "1. Open browser and go to: https://www.arduino.cc/en/software" -ForegroundColor White
    Write-Host "2. Download: Windows Win 7 and newer" -ForegroundColor White
    Write-Host "3. Run the installer" -ForegroundColor White
    Write-Host ""
    Read-Host "Press Enter after you've installed Arduino IDE"
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "STEP 3: Android Studio Installation" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "Android Studio is required to build and install the mobile app." -ForegroundColor Yellow
Write-Host ""
Write-Host "⚠️ Note: Android Studio is a large download (~1GB)" -ForegroundColor Yellow
Write-Host ""
Write-Host "Please download Android Studio manually:" -ForegroundColor Yellow
Write-Host "1. Open browser and go to: https://developer.android.com/studio" -ForegroundColor White
Write-Host "2. Download: Android Studio for Windows" -ForegroundColor White
Write-Host "3. Run the installer (follow default options)" -ForegroundColor White
Write-Host "4. Install Android SDK when prompted" -ForegroundColor White
Write-Host ""
Read-Host "Press Enter after you've installed Android Studio"

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Installation Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

Write-Host "✓ Software installation phase completed" -ForegroundColor Green
Write-Host ""
Write-Host "Next Steps:" -ForegroundColor Yellow
Write-Host "1. Run: .\setup-database.ps1 (to set up database)" -ForegroundColor White
Write-Host "2. Configure WiFi and IP addresses in code" -ForegroundColor White
Write-Host "3. Upload Arduino code" -ForegroundColor White
Write-Host "4. Build Android app" -ForegroundColor White
Write-Host ""
Write-Host "See SETUP_GUIDE.md for complete instructions." -ForegroundColor Cyan
Write-Host ""

Read-Host "Press Enter to exit"
