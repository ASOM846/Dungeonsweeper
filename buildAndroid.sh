#!/bin/bash
# build_and_deploy.sh

# ================================
# KONFIGURACJA
# ================================
PROJECT_ROOT="$(pwd)" # katalog główny repo
ANDROID_CPP_DIR="$PROJECT_ROOT/android/app/src/main/cpp"
APK_PACKAGE_NAME="com.raylib.raymob" # zmień na swój package
GRADLEW="$PROJECT_ROOT/android/gradlew"

# ================================
# KROK 1: Kopiowanie folderów
# ================================
echo "Kopiowanie folderów src/ i assets/..."
rm -rf "$ANDROID_CPP_DIR/src" "$ANDROID_CPP_DIR/assets"
cp -r "$PROJECT_ROOT/src" "$ANDROID_CPP_DIR/"
cp -r "$PROJECT_ROOT/assets" "$ANDROID_CPP_DIR/"

# ================================
# KROK 2: Budowanie APK
# ================================
echo "Budowanie APK..."
cd "$PROJECT_ROOT/android"
$GRADLEW clean assembleDebug
if [ $? -ne 0 ]; then
  echo "Błąd przy buildzie APK!"
  exit 1
fi

# ================================
# KROK 3: Wypchnięcie APK przez ADB
# ================================
APK_PATH="$PROJECT_ROOT/android/app/build/outputs/apk/debug/app-debug.apk"

echo "Odinstalowywanie poprzedniej wersji APK..."
adb uninstall $APK_PACKAGE_NAME

echo "Instalowanie nowego APK..."
adb install "$APK_PATH"

echo "APK zainstalowane na urządzeniu!"
