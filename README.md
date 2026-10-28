# AresNexus v1.0.0


A chating system with high performance server with sleek and minimalist Terminal UI. 

---

## 🚀 Features

- **High Performance:** The server is coded in C++ for ultra high perfomance and minimal latency.
- **Sleek UI:** Work on UI/UX has been extensive and future updates will introduce many advancements.
- **Easy to setup:** Kept the diployment process as simple as possible with proper tools.

## 🛠️ Tech Stack

- **Frontend:** Python
- **Backend:** C++

## 📦 Installation

### step-by-step setup guide.
>***NOTE:***
>If you only want to run this applicationg ***refer to latest release*** where you'll find latest binaries for linux.

### for full development setup

prerequisites:
- git
- cmake
- g++
- and internet ofc


go to the terminal and
from root dir i.e. ***chat-server-v2/***
copy paste these

```bash

git clone https://github.com/codeyy/chat-server-v2

cd chat-server-v2
```
<br>

### chose either:

<br>

with cmake:
```bash
cmake -B build 

cmake --build build

./build/chat-server
```
<br>

with just compiler (g++)
```bash
g++ -std=c++20 -O3 Core-Server/main.cpp -o chat_server -lpthread

./chat-server
```



<br>
<br>

## 💻 Client side usage instructions.

from root dir i.e. chat-server-v2/
copy paste this

```bash
# Start server
cd UI/Pyui

python -m venv venv

source venv/bin/activate

pip install -r requirement.txt

python main.py
```

> [!NOTE]():    
> if you are on **windows** use 
***./venv/bin/activate*** instead of 
"source venv/bin/activate"



<br>

## 🤝 Contributing

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

Distributed under the MIT License. See `LICENSE` for more information.

## 👥 Authors

- **Agam** - [@codeyy](https://github.com/codeyy)
