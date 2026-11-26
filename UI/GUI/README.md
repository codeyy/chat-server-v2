# NexusForge GUI Client (Flutter) 🚀

A modern, high-performance, polished cross-platform GUI client for **NexusForge Chat Server v2**, supporting **every major operating system** (Windows, Linux, macOS, Android, iOS, and Web).

---

## 🌟 Key Features

- **Cross-Platform by Design:** Native binaries for **Windows**, **Linux**, and **macOS**, plus first-class support for **Android**, **iOS**, and **Web**.
- **Adaptive Responsive Layout:**
  - **Desktop / Tablet / Web:** Master-detail split view with persistent sidebar, real-time contact list, search filtering, and live socket console.
  - **Mobile:** Adaptive navigation with single-pane chat views and back navigation.
- **Accurate Protocol Framing:**
  - Full adherence to the C++ server's network-byte-order (big-endian) 4-byte payload length header (`struct.pack('!I', len(payload))`).
  - Wire format: `[4-byte big-endian length] + UTF-8("<recipient>><message>><sender>>")`.
  - Stream buffering handles TCP packet chunking and concatenation seamlessly.
- **Username Configuration & State Flow:**
  - Integrated username setup matching `Pyui/main.py` (`Server><username>>_>`).
  - Real-time feedback for `"username configured successfully"`, `"username already exists"`, and `"its YOUR username"`.
  - Server alert notifications when a recipient is not found on the server.
- **Sleek Theming:**
  - Modern obsidian dark theme with cyber cyan/indigo accents + crisp light theme.
  - One-click theme toggle, timestamps, status ticks (sending, sent, failed), and message copy support.
- **Protocol & Socket Console:**
  - Built-in terminal log dialog showing raw sent and received packets, connection transitions, and errors.

---

## 📁 Project Structure

```
C:\programs\chat-server-v2\UI\GUI\
├── pubspec.yaml                  # Flutter package configuration & metadata
├── analysis_options.yaml         # Linter rules
├── README.md                     # Documentation & setup guide
├── lib\
│   ├── main.dart                 # Application entry point
│   ├── models\
│   │   ├── chat_message.dart     # Message model (isMine, isServer, status, timestamps)
│   │   ├── conversation.dart     # Conversation thread & unread counter model
│   │   └── connection_state.dart # Connection state enum
│   ├── services\
│   │   ├── protocol_codec.dart   # Big-endian 4-byte packet codec & stream parser
│   │   └── chat_socket_service.dart # Raw TCP socket manager with reconnection & listeners
│   ├── controllers\
│   │   └── chat_controller.dart  # Reactive state management
│   ├── theme\
│   │   └── app_theme.dart        # Dark & Light Material 3 theme configurations
│   ├── widgets\
│   │   ├── chat_bubble.dart      # Custom speech bubbles with context menu
│   │   ├── message_composer.dart # Keyboard-aware input field (Enter to send)
│   │   ├── sidebar.dart          # Searchable conversation list & user profile
│   │   └── status_badge.dart     # Pulsing connection status indicator
│   ├── dialogs\
│   │   ├── connection_dialog.dart# Server IP, port, and username setup modal
│   │   ├── new_chat_dialog.dart  # Start chat with @user
│   │   └── server_logs_dialog.dart # Real-time protocol packet inspector
│   └── screens\
│       └── chat_home_screen.dart # Adaptive split-screen / mobile responsive layout
├── windows\                      # Windows desktop runner (C++ / Win32)
├── linux\                        # Linux desktop runner (C++ / GTK)
├── macos\                        # macOS desktop runner (Swift / AppKit)
├── android\                      # Android mobile runner (Kotlin)
├── ios\                          # iOS mobile runner (Swift / UIKit)
└── web\                          # Web browser runner (HTML5 / PWA)
```

---

## 🛠️ How to Run & Build for Every OS

Ensure you have [Flutter SDK](https://flutter.dev/docs/get-started/install) installed.

### 1. Get Dependencies
From the `C:\programs\chat-server-v2\UI\GUI` directory:
```bash
flutter pub get
```

### 2. Run on Your Current Platform

#### 🪟 Windows (Desktop)
```bash
flutter run -d windows
```

#### 🐧 Linux (Desktop)
```bash
flutter run -d linux
```

#### 🍎 macOS (Desktop)
```bash
flutter run -d macos
```

#### 📱 Android (Phone / Emulator)
```bash
flutter run -d android
```

#### 📱 iOS (Simulator / Device)
```bash
flutter run -d ios
```

#### 🌐 Web (Chrome / Edge)
```bash
flutter run -d chrome
```

---

## 📦 Building Standalone Release Binaries

To produce standalone executables and packages for distribution:

- **Windows:**
  ```bash
  flutter build windows
  # Output: build/windows/x64/runner/Release/
  ```

- **Linux:**
  ```bash
  flutter build linux
  # Output: build/linux/x64/release/bundle/
  ```

- **macOS:**
  ```bash
  flutter build macos
  # Output: build/macos/Build/Products/Release/
  ```

- **Android:**
  ```bash
  flutter build apk --release
  # or: flutter build appbundle --release
  ```

- **iOS:**
  ```bash
  flutter build ipa
  ```

- **Web:**
  ```bash
  flutter build web --release
  ```

---

## 📡 Protocol Reference (Matching Pyui & C++ Server)

### 1. Wire Framing (Client -> Server)
Every message from client to server contains a 4-byte length prefix:
```
+---------------------------+-----------------------------------------------+
| 4 Bytes (Big-Endian uint) | UTF-8 Payload: "<recipient>><message>><sender>>" |
+---------------------------+-----------------------------------------------+
```

### 2. Username Configuration
```
Packet: "Server><desired_username>>_>" (with 4-byte big-endian length prefix)
```
- Server responds with:
  - `<desired_username>>username configured successfully>Server>` -> **Confirmed**
  - `<desired_username>>its YOUR username>Server>` -> **Confirmed**
  - `_>username already exists>Server>` -> **Conflict / Retry**

### 3. Direct Chat Message
```
Packet: "<recipient>><message_text>><sender>>" (with 4-byte big-endian length prefix)
```
- Server relays to recipient:
  - `<recipient>><message_text>><sender>>`
- If recipient does not exist:
  - `<sender>>recipient: "<recipient>" Doesn't exist>Server>`
