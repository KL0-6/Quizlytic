# Quizlytic

Quizlytic is a full-stack AI Flashcard application I've built to help people generate study material quickly. The app is built using modern web technologies including React, TypeScript, and Tailwind CSS, with a backend powered by Drogon and C++.

## Features

- **AI-Powered Flashcard Creation**: Automatically generate answers to your questions using AI.
- **User Authentication**: Secure login and user management using Clerk.
- **Flashcard Set Selection**: Easily browse and select from different flashcard sets.
- **Responsive Design**: Optimized for both desktop and mobile experiences.

## Prerequisites

- Node.js (>=14.x)
- npm
- CMake (>=3.8)
- A C++ compiler that supports C++17
- [Drogon](https://github.com/drogonframework/drogon) framework **WITH MYSQL SUPPORT**

## Build (Linux)

### Frontend

#### Setting up
Before running the frontend, ensure that you have a `.env.local` file in the `frontend` directory with the following content:

```env
VITE_CLERK_PUBLISHABLE_KEY=YOUR_CLERK_PUBLISHABLE_KEY
```

#### Running frontend

```bash
cd frontend
npm install
npm run dev
```

### Backend

The backend depends on several shared libraries. Ensure these are installed on your Linux system:

#### Installation of Shared Libraries

You can install these libraries on a Debian-based system (like Ubuntu) using the following command:

```bash
sudo apt-get update
sudo apt-get install libssl-dev libcrypto++-dev libjsoncpp-dev uuid-dev libbrotli-dev libmariadb-dev zlib1g-dev build-essential
```

#### Building backend

```bash
cd backend
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
```

#### Running backend

Once the backend is built, it requires a json file path to be passed as a command line argument. This file should contain the necassary configuration settings for Drogon, Groq, and the Database. Here is an example of how it should look like:
```json
{
{
    "db_clients":
    {
        "default":
        {
            "rdbms": "mysql",
            "host": "localhost",
            "port": 3306,
            "dbname": "YOUR_DB_NAME",
            "user": "USERNAME",
            "passwd": "PASSWORD",
            "connection_number": 10
        }
    },
    "env":
    {
        "drogonIp": "127.0.0.1",
        "drogonPort": 8848,
        "drogonThreads": 0,

        "groqKey": "GROQ_KEY_HERE"
    }
}
```
