# ESP32 Wireless OTA Updater

Ever wished you could update your ESP32 project without hunting for a USB cable? That's exactly what this project does. Flash your firmware once over USB, and from that point on, every future update happens over Wi-Fi — completely wirelessly, from a browser dashboard on your PC.

No more unplugging your device from wherever it's mounted. No more opening Arduino IDE just to push a small change. Just upload a `.bin` file to the dashboard, and your ESP32 handles the rest on its own.

---

## How it works

There are two parts to this project:

**The ESP32 sketch** runs on your microcontroller. Every 30 seconds it quietly checks your PC's server for a newer firmware version. If it finds one, it downloads and flashes itself, then reboots into the new firmware — all without any human involvement.

**The Flask server** runs on your PC. It hosts your firmware files and gives you a clean web dashboard where you can drag and drop a new `.bin` file, set a version number, and hit Upload. That's the entire update process.

```
You change code in Arduino IDE
        ↓
Export .bin → drag to dashboard → set version
        ↓
ESP32 detects new version on next check
        ↓
Downloads, flashes, reboots automatically
        ↓
Running new firmware — no USB touched
```

---

## Project structure

```
esp32-wireless-ota/
│
├── esp32_firmware/
│   └── esp32_ota.ino        ← Arduino sketch for the ESP32
│
├── server/
│   ├── server.py            ← Flask server + web dashboard
│   └── requirements.txt     ← Python dependencies
│
├── .gitignore
└── README.md
```

---

## Getting started

### What you need

- ESP32 development board
- Arduino IDE with ESP32 board package installed
- Python 3.x on your PC
- Both your PC and ESP32 on the same Wi-Fi network

---

### Step 1 — Set up the server

Open a terminal, navigate to the `server` folder, and run:

```bash
pip install flask
python server.py
```

You'll see your local IP printed in the terminal, something like:

```
Dashboard : http://192.168.1.5:5000
```

Open that URL in your browser and you'll see the dashboard.

---

### Step 2 — Configure the ESP32 sketch

Open `esp32_firmware/esp32_ota.ino` in Arduino IDE and fill in your details at the top of the file:

```cpp
const char* ssid         = "YOUR_WIFI_SSID";
const char* password     = "YOUR_WIFI_PASSWORD";
const char* ota_hostname = "Annis-ESP32";       // name for your device
const char* ota_password = "wirelessESP32";     // OTA upload password

const char* version_url  = "http://YOUR_PC_IP:5000/version";
const char* firmware_url = "http://YOUR_PC_IP:5000/firmware";
```

Replace `YOUR_PC_IP` with the IP address shown when you started the server.

---

### Step 3 — First upload (USB only)

This is the one and only time you'll need a USB cable.

Connect your ESP32, select the correct port in Arduino IDE, and click Upload. After this upload the ESP32 has the OTA listener baked in and can receive all future updates wirelessly.

---

### Step 4 — Push your first wireless update

1. Make any change to the sketch — for example bump `FW_VERSION` from `1.0.0` to `1.0.1`
2. In Arduino IDE go to **Sketch → Export Compiled Binary**
3. Then **Sketch → Show Sketch Folder** — find the `.bin` file inside
4. Drag the `.bin` onto the dashboard, type `1.0.1` as the version, click Upload
5. Wait up to 30 seconds — watch Serial Monitor and you'll see the ESP32 detect, download, flash, and reboot on its own

From this point on, this four-step process is all you ever need to update your device.

---

## The update workflow (every time after setup)

```
1. Edit your sketch in Arduino IDE
2. Sketch → Export Compiled Binary
3. Drag .bin to dashboard → set new version → Upload
4. ESP32 self-updates within 30 seconds
```

---

## Dashboard features

- Live current version display
- Drag and drop `.bin` upload with progress bar
- Upload history with file size and MD5 checksum
- All three ESP32 endpoints shown for reference

---

## Serial Monitor output

When everything is working you'll see this pattern on boot:

```
[WiFi] Connecting to YourNetwork
[WiFi] Connected! IP: 192.168.1.17
[BOOT] Firmware v1.0.0 running
[HTTP-OTA] Current: 1.0.0 — checking server...
[HTTP-OTA] Server version: 1.0.0
[HTTP-OTA] Already up to date
```

And when a new version is available:

```
[HTTP-OTA] Current: 1.0.0 — checking server...
[HTTP-OTA] Server version: 1.0.1
[HTTP-OTA] Downloading firmware...
[HTTP-OTA] Flash started
[HTTP-OTA] 702237 / 702237 bytes
[HTTP-OTA] Done! Rebooting...
[BOOT] Firmware v1.0.1 running
```

---

## Important notes

**Keep `ArduinoOTA.handle()` in every loop.** The sketch supports both HTTP OTA (server-based, automatic) and ArduinoOTA (IDE-based, manual). As long as `ArduinoOTA.handle()` stays in `loop()`, you can also push updates directly from Arduino IDE over the network port.

**Keep the server running while the ESP32 is active.** The ESP32 checks every 30 seconds. If the server is off, it just skips and tries again next cycle — nothing breaks.

**Both devices must be on the same Wi-Fi network.** This is a local network solution. The ESP32 and your PC need to be on the same router.

**Bump the version number for every update.** The ESP32 compares version strings. If the version on the server matches what's running, it won't flash. Always increment the version when you upload new firmware.

---

## Built with

- ESP32 (Arduino framework)
- Python Flask
- HTTPUpdate library (ESP32)
- ArduinoOTA library (ESP32)

---

## License

MIT — do whatever you want with it.
