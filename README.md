# 🚖 Ride Matching App

A high-performance ride-matching application featuring a **React** frontend, a **Node.js** API, and a custom **C-based KD-Tree** backend for lightning-fast spatial searches.

---

## 🏗️ Architecture Overview

-   **Frontend**: React (Vite) for a modern, responsive user interface.
-   **API Backend**: Node.js & Express serving as the orchestrator.
-   **Computational Engine**: C-based KD-Tree implementation for efficient spatial queries (k-Nearest Neighbors and Range Search).

---

## ⚡ Quick Start

If you have **Node.js** and **GCC** installed, run these commands to get started immediately:

```bash
# 1. Install all dependencies
npm install && cd backend && npm install && cd ../frontend && npm install && cd ..

# 2. Build the C engine
npm run build-c

# 3. Run the app (Frontend + Backend)
npm run dev
```

---

## 🛠️ Prerequisites

Before you begin, ensure you have the following installed:

1.  **Node.js** (v18.0.0 or higher) & **npm**
2.  **GCC Compiler** (e.g., MinGW-w64 for Windows) - added to your system PATH.

---

## 📥 Installation

### 1. Clone the Repository
```bash
git clone https://github.com/25mcmc21-dakshdua/ds_project.git
cd ride-matching-app
```

### 2. Install Node.js Dependencies
Install dependencies for the root, frontend, and backend:

**Root & Concurrent Runner:**
```bash
npm install
```

**API Backend:**
```bash
cd backend
npm install
cd ..
```

**Frontend:**
```bash
cd frontend
npm install
cd ..
```

---

## ⚙️ Compilation (C Backend)

The spatial search engine needs to be compiled into an executable.

**On Windows:**
```bash
npm run build-c
```
*Alternatively, run `cd c-backend && build.bat`*

---

## 🚀 Running the Program

### The Fast Way (Concurrent)
Start both the backend API and the frontend development server with a single command from the root:

```bash
npm run dev
```

### The Individual Way

If you prefer to run components separately:

**1. Start the API Backend:**
```bash
npm run backend
```
*Server will start on `http://localhost:3001` (check `backend/server.js` for port details).*

**2. Start the Frontend:**
```bash
npm run frontend
```
*The app will be available at `http://localhost:5173`.*

---

## 📁 Project Structure

```text
ride-matching-app/
├── backend/            # Node.js Express API
│   ├── server.js       # Main entry point
│   └── package.json
├── c-backend/          # C-based KD-Tree implementation
│   ├── kdtree.c        # KD-Tree logic
│   ├── kdtree_server.c # Stdin/Stdout interface for Node.js
│   └── build.bat       # Windows build script
├── frontend/           # React + Vite application
│   ├── src/            # Components, Hooks, and Styles
│   └── package.json
└── package.json        # Root workspace configuration
```

---

## 🧪 How it Works
1. The **React Frontend** sends ride requests or driver updates to the Node.js API.
2. The **Node.js API** spawns/communicates with the compiled `kdtree_server.exe` via stdin/stdout.
3. The **C Engine** performs complex spatial calculations in microseconds and returns results.
4. The **Frontend** visualizes the matches on an interactive interface.

---

## 📝 License
[MIT](LICENSE)
