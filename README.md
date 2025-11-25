# 🚚 LogistSim  
A Package Delivery Simulation System

LogistSim is a simulation platform inspired by large-scale logistics systems (like Amazon).  
Users can configure key operational parameters such as:

- Warehouse placement and quantity  
- Inventory catalog and stock levels  
- Number of trucks and employees per warehouse  
- Delivery orders to be dispatched  

The simulation processes these inputs and computes **optimal delivery routes** for each package using path-planning logic.

---

## 📁 Project Structure

The project is organized into clear directories to separate logic, UI, and resources:

| Directory | Content | Description |
|----------|---------|-------------|
| **Include/** | Header Files (`.h`) | Contains all C++ class interfaces and data structures (e.g., `City.h`, `Truck.h`, `Simulation.h`). |
| **Source/** | Implementation Files (`.cpp`) | Contains the core logic implementation, including the main function and class definitions. |
| **Forms/** | UI Files (`.ui`) | Qt Designer XML files defining the graphical interface (e.g., `MainWindow.ui`). |
| **res.qrc** | Resource File | Bundles icons, images, and internal assets used by the application. |

---

## 🧰 Prerequisites

Before building the project, ensure you have a complete C++ and Qt development environment.

### **Required Tools**
- **C++ Compiler** supporting C++17  
  (GCC 7+, Clang 5+, or MSVC 2017+)
- **Qt Framework 6.x**
- **Qt Creator IDE**
- **qmake** (included with Qt)

### **Required External Libraries**
These libraries must be installed prior to building the project:

| Library | Purpose | Notes |
|--------|---------|-------|
| **DeSiGNAR 2.0.0** | Geospatial & simulation utilities | Install from: https://github.com/R3mmurd/DeSiGNAR |
| **GDAL** | Geospatial data processing | Link with `-lgdal` |
| **Protocol Buffers** | Data serialization | Link with `-lprotobuf` |
| **Boost Iostreams** | Data stream handling | Link with `-lboost_iostreams` |
| **zlib** | Compression | Link with `-lz` |
| **bzip2** | Compression | Link with `-lbz2` |
| **Expat** | XML parsing | Link with `-lexpat` |

On Ubuntu/Debian, most can be installed via:

```bash
sudo apt install libgdal-dev libprotobuf-dev libboost-iostreams-dev zlib1g-dev libbz2-dev libexpat1-dev
```

## 🗂️ Setting Up DeSiGNAR Path

After installing **DeSiGNAR**, locate its installation root directory.  
Your `.pro` file must include:

```bash
# CHANGE THIS TO MATCH YOUR INSTALLATION DIRECTORY
DSG = /path/to/DeSiGNAR-2.0.0
```

## 🛠️ Building the Project

LogistSim can be built using either the **terminal** or **Qt Creator**.

---

### 🔹 Option A — Build Using Command Line (qmake)

```bash
# 1. Navigate to the project directory
cd /path/to/LogistSim

# 2. Generate the Makefile
qmake LogistSim.pro

# 3. Compile the project
make

# (The executable will be located in the build directory)
```

### 🔹 Option B — Build Using Qt Creator

1. Open **Qt Creator**  
2. Go to **File > Open File or Project…**  
3. Select **LogistSim.pro**  
4. Ensure the required Qt modules are installed:

### ✅ Required Qt Modules
- Qt Core  
- Qt GUI  
- Qt Widgets  
- Qt Quick Widgets  
- Qt Positioning  
- Qt Location  

If modules are missing, install them using the Qt Maintenance Tool  
or via terminal:

```bash
sudo apt install \
    qml-module-qtquick-controls \
    qml-module-qtpositioning \
    qml-module-qtlocation
```
5. Configure your build kit (must support C++17)
6. Click Build Project (Ctrl+B)
7. Run the app using the Run button
